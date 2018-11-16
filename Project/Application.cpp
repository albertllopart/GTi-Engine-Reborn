#include "Application.h"
#include "parson/parson.h"

#include "Timer.h"

Application::Application()
{
	frames = 0;
	lastMs = -1;
	lastFPS = -1;
	capped_ms = 1000 / 60;
	fps_counter = 0;


	window = new ModuleWindow(this);
	input = new ModuleInput(this);
	renderer3D = new ModuleRenderer3D(this);
	camera = new ModuleCamera3D(this);
	imgui = new ModuleImGui(this);
	import = new ModuleImporter(this);
	rng = new ModuleRNG(this);
	editor = new ModuleSceneEditor(this);
	textures = new ModuleTextures(this);
	e_windows = new ModuleEngineWindows(this);
	filesystem = new ModuleFileSystem(this);
	time = new ModuleTimeManager(this);
	resource = new ModuleResourceManager(this);

	// The order of calls is very important!
	// Modules will Init() Start() and Update in this order
	// They will CleanUp() in reverse order

	// Main Modules
	AddModule(window);
	AddModule(editor);
	AddModule(imgui);
	AddModule(e_windows);
	AddModule(camera);
	AddModule(input);
	AddModule(rng);
	
	AddModule(textures);
	AddModule(import);
	AddModule(time);
	AddModule(filesystem);
	AddModule(resource);

	// Renderer last!
	AddModule(renderer3D);
}

Application::~Application()
{
	for (std::list<Module*>::const_reverse_iterator item = list_modules.rbegin(); item != list_modules.rend(); ++item)
	{
		delete *item;
	}
}

bool Application::Init()
{
	bool ret = true;

	// Call Init() in all modules

	// First Json test
	JSON_Value * config_file = json_parse_file("config.json");
	JSON_Object * config;
	JSON_Object* config_node;


	if (config_file != nullptr)
	{
		// Read variables from config.json
		config = json_value_get_object(config_file);
		config_node = json_object_get_object(config, "Application");

		max_fps = json_object_get_number(config_node, "Max FPS");

		appname = json_object_get_string(config_node, "name");
		organization = json_object_get_string(config_node, "organization");

		if (max_fps > 0)
		{
			capped_ms = 1000 / max_fps;
		}

		for (std::list<Module*>::const_iterator item = list_modules.begin(); item != list_modules.end() && ret; ++item)
		{
			config_node = json_object_get_object(config, (*item)->name);
			ret = (*item)->Init(config_node);
		}
	}
	else
	{
		for (std::list<Module*>::const_iterator item = list_modules.begin(); item != list_modules.end() && ret; ++item)
		{
			ret = (*item)->Init(nullptr);
		}
	}
	

	// After all Init calls we call Start() in all modules
	LOG("Application Start --------------");
	for (std::list<Module*>::const_iterator item = list_modules.begin(); item != list_modules.end() && ret; ++item)
	{
		ret = (*item)->Start();
	}
	
	frame_time.Start();
	
	//time manager
	time->StartRealTime();

	return ret;
}

// ---------------------------------------------
void Application::PrepareUpdate()
{
	dt = (float)frame_time.Read() / 1000.0f;
	ms_time.Start();

	if (max_fps > 0)
		capped_ms = 1000 / max_fps;
}

// ---------------------------------------------
void Application::FinishUpdate()
{
	++frames;
	++fps_counter;

	if (frame_time.Read() > 1000)
	{
		lastFPS = fps_counter;
		fps_counter = 0;
		frame_time.Start();
	}

	lastMs = ms_time.Read();

	if (capped_ms > 0 && lastMs < capped_ms)
	{
		SDL_Delay(capped_ms - lastMs);
		//LOG("We waited for %d milliseconds", capped_ms - lastMs);
	}

	dt = (float)ms_time.Read() / 1000.0f - startTime;
	lastFPS = 1.0f / dt;
	lastMs = (float)ms_time.Read();
	//Time Manager
	time->SetFrameCount(1);	
}

// Call PreUpdate, Update and PostUpdate on all modules
update_status Application::Update()
{
	update_status ret = UPDATE_CONTINUE;
	PrepareUpdate();
	
	std::list<Module*>::const_iterator item = list_modules.begin();

	while(item != list_modules.end() && ret == UPDATE_CONTINUE)
	{
		ret = (*item)->PreUpdate(dt);
		item++;
	}

	item = list_modules.begin();

	while (item != list_modules.end() && ret == UPDATE_CONTINUE)
	{
		ret = (*item)->Update(dt);
		item++;
	}
	
	if (ret == UPDATE_CONTINUE)
	{
		ret = OnEditor();
	}

	item = list_modules.begin();

	while (item != list_modules.end() && ret == UPDATE_CONTINUE)
	{
		ret = (*item)->PostUpdate(dt);
		item++;
	}

	FinishUpdate();

	if (want_to_save)
		SaveEngine();
	if (want_to_load)
		LoadEngine();

	return ret;
}

update_status Application::OnEditor()
{
	update_status ret = UPDATE_CONTINUE;

	std::list<Module*>::iterator item = list_modules.begin();
	while (item != list_modules.end() && ret == UPDATE_CONTINUE)
	{
		ret = (*item)->OnEditor();
		item++;
	}

	return ret;
}

bool Application::CleanUp()
{
	bool ret = true;

	for (std::list<Module*>::const_reverse_iterator item = list_modules.rbegin(); item != list_modules.rend() && ret; ++item)
	{
		ret = (*item)->CleanUp();
	}
	
	return ret;
}

void Application::Save()
{
	want_to_save = true;
}

void Application::Load()
{
	want_to_load = true;
}

void Application::SaveEngine() const
{

	LOG("SAVING CONFIG TO FILE -----------------------")

	JSON_Value* config_file = json_parse_file("config.json");
	JSON_Object* config;
	JSON_Object* config_node;

	if (config_file != nullptr)
	{
		config = json_value_get_object(config_file);
		config_node = json_object_get_object(config, "Application");

		json_object_set_number(config_node, "Max FPS", max_fps); // asigning max fps value
	}

	for (std::list<Module*>::const_iterator item = list_modules.begin(); item != list_modules.end(); ++item)
	{
		config_node = json_object_get_object(config, (*item)->name);
		(*item)->Save(config_node);
	}

	json_serialize_to_file_pretty(config_file, "config.json");

	want_to_save = false;
}

void Application::LoadEngine() const
{
	LOG("LOADING CONFIG TO FILE -----------------------")

	JSON_Value* config_file = json_parse_file("config.json");
	JSON_Object* config;
	JSON_Object* config_node;

	if (config_file != nullptr)
	{
		config = json_value_get_object(config_file);
		config_node = json_object_get_object(config, "Application");

		max_fps = json_object_get_number(config_node, "Max FPS");
	}

	for (std::list<Module*>::const_iterator item = list_modules.begin(); item != list_modules.end(); ++item)
	{
		config_node = json_object_get_object(config, (*item)->name);
		(*item)->Load(config_node);
	}

	want_to_load = false;
}

void Application::AddModule(Module* mod)
{
	list_modules.push_back(mod);
}

float Application::GetFPS()
{
	return lastFPS;
}

float Application::GetMs()
{
	return lastMs;
} 

int* Application::GetMaxFPS()
{
	return &max_fps;
}