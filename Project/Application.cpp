#include "Application.h"
#include "parson/parson.h"

Application::Application()
{
	window = new ModuleWindow(this);
	input = new ModuleInput(this);
	audio = new ModuleAudio(this, true);
	renderer3D = new ModuleRenderer3D(this);
	camera = new ModuleCamera3D(this);
	imgui = new ModuleImGui(this);
	rng = new ModuleRNG(this);

	// The order of calls is very important!
	// Modules will Init() Start() and Update in this order
	// They will CleanUp() in reverse order

	// Main Modules
	AddModule(window);
	AddModule(camera);
	AddModule(input);
	AddModule(audio);
	AddModule(imgui);
	AddModule(rng);


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
	}
	for (std::list<Module*>::const_iterator item = list_modules.begin(); item != list_modules.end() && ret; ++item)
	{
		config_node = json_object_get_object(config, (*item)->name);
		ret = (*item)->Init(config_node);
	}

	// After all Init calls we call Start() in all modules
	LOG("Application Start --------------");
	for (std::list<Module*>::const_iterator item = list_modules.begin(); item != list_modules.end() && ret; ++item)
	{
		ret = (*item)->Start();
	}
	
	ms_timer.Start();
	capped_ms = 1000 / max_fps;
	return ret;
}

// ---------------------------------------------
void Application::PrepareUpdate()
{
	dt = (float)ms_timer.Read() / 1000.0f;
	ms_timer.Start();

	if (max_fps > 0)
		capped_ms = 1000 / max_fps;
}

// ---------------------------------------------
void Application::FinishUpdate()
{
	dt = (float)ms_timer.Read() / 1000.0f - dt;
	lastFPS = 1.0f / dt;
	lastMs = (float)ms_timer.Read();

	if (capped_ms > 0 && lastMs < capped_ms)
	{
		SDL_Delay(capped_ms - lastMs);
		LOG("We waited for %d milliseconds", capped_ms - lastMs);
	}
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

	item = list_modules.begin();

	while (item != list_modules.end() && ret == UPDATE_CONTINUE)
	{
		ret = (*item)->PostUpdate(dt);
		item++;
	}

	FinishUpdate();
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

bool Application::SaveConfig()
{
	bool ret = false;

	LOG("SAVING CONFIG TO FILE -----------------------")

	JSON_Value* config_file = json_parse_file("config.json");
	JSON_Object* config;
	JSON_Object* config_node;

	if (config_file != nullptr)
	{
		ret = true;

		config = json_value_get_object(config_file);
		config_node = json_object_get_object(config, "Application");
		json_object_set_number(config_node, "Max FPS", max_fps); // asigning max fps value
	}

	return ret;
}

void Application::AddModule(Module* mod)
{
	list_modules.push_back(mod);
}

float Application::GetFPS()
{
	return lastFPS * -1;
}

float Application::GetMs()
{
	return lastMs;
} 

int* Application::GetMaxFPS()
{
	return &max_fps;
}