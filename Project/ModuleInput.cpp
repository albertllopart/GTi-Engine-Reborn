#include "Globals.h"
#include "Application.h"
#include "ModuleInput.h"
#include "ModuleRNG.h"
#include "parson/parson.h"
#include "ModuleSceneEditor.h"
#include "ImGui/imgui.h"
#include "ImGui/imgui_impl_sdl.h"
#include "ImGui/imgui_impl_opengl3.h"


#define MAX_KEYS 300

ModuleInput::ModuleInput(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	keyboard = new KEY_STATE[MAX_KEYS];
	memset(keyboard, KEY_IDLE, sizeof(KEY_STATE) * MAX_KEYS);
	memset(mouse_buttons, KEY_IDLE, sizeof(KEY_STATE) * MAX_MOUSE_BUTTONS);

	name = "Input";
}

// Destructor
ModuleInput::~ModuleInput()
{
	delete[] keyboard;
}

// Called before render is available
bool ModuleInput::Init(JSON_Object* node)
{
	LOG("Init SDL input event system");
	App->imgui->AddConsoleLog("Init SDL input event system");
	bool ret = true;
	SDL_Init(0);

	if(SDL_InitSubSystem(SDL_INIT_EVENTS) < 0)
	{
		LOG("SDL_EVENTS could not initialize! SDL_Error: %s\n", SDL_GetError());
		App->imgui->AddConsoleLog(("SDL_EVENTS could not initialize! SDL_Error: %s\n", SDL_GetError()));
		ret = false;
	}

	return ret;
}

// Called every draw update
update_status ModuleInput::PreUpdate(float dt)
{
	SDL_PumpEvents();

	const Uint8* keys = SDL_GetKeyboardState(NULL);
	
	for(int i = 0; i < MAX_KEYS; ++i)
	{
		if(keys[i] == 1)
		{
			if(keyboard[i] == KEY_IDLE)
				keyboard[i] = KEY_DOWN;
			else
				keyboard[i] = KEY_REPEAT;
		}
		else
		{
			if(keyboard[i] == KEY_REPEAT || keyboard[i] == KEY_DOWN)
				keyboard[i] = KEY_UP;
			else
				keyboard[i] = KEY_IDLE;
		}
	}

	Uint32 buttons = SDL_GetMouseState(&mouse_x, &mouse_y);

	mouse_x /= SCREEN_SIZE;
	mouse_y /= SCREEN_SIZE;
	mouse_z = 0;

	for(int i = 0; i < 5; ++i)
	{
		if(buttons & SDL_BUTTON(i))
		{
			if(mouse_buttons[i] == KEY_IDLE)
				mouse_buttons[i] = KEY_DOWN;
			else
				mouse_buttons[i] = KEY_REPEAT;
		}
		else
		{
			if(mouse_buttons[i] == KEY_REPEAT || mouse_buttons[i] == KEY_DOWN)
				mouse_buttons[i] = KEY_UP;
			else
				mouse_buttons[i] = KEY_IDLE;
		}
	}

	mouse_x_motion = mouse_y_motion = 0;

	bool quit = false;
	std::string file_dir; //drag&drop
	SDL_Event e; // event for directory (mesh)
	while(SDL_PollEvent(&e))
	{

		ImGui_ImplSDL2_ProcessEvent(&e);

		switch(e.type)
		{
			case SDL_MOUSEWHEEL:
			mouse_z = e.wheel.y;
			break;

			case SDL_MOUSEMOTION:
			mouse_x = e.motion.x / SCREEN_SIZE;
			mouse_y = e.motion.y / SCREEN_SIZE;

			mouse_x_motion = e.motion.xrel / SCREEN_SIZE;
			mouse_y_motion = e.motion.yrel / SCREEN_SIZE;
			break;

			case SDL_QUIT:
			quit = true;
			break;

			case SDL_DROPFILE:      //drop case
				file_dir = e.drop.file;
				// directory of the file
				if (file_dir.find(".fbx") != std::string::npos || file_dir.find(".obj") != std::string::npos || file_dir.find(".FBX") != std::string::npos || file_dir.find(".OBJ") != std::string::npos)
				{
					LOG("%s dropped on window.", file_dir);
					App->imgui->AddConsoleLog(("%s dropped on window", file_dir));

					App->import->ImportMesh(file_dir.c_str());
					std::string clean = App->import->CleanFileName(file_dir.c_str());
				}
				else if (file_dir.find(".png") != std::string::npos || file_dir.find(".dds") != std::string::npos || file_dir.find(".DDS") != std::string::npos || file_dir.find(".PNG") != std::string::npos)
				{
					LOG("%s dropped on window.", file_dir);
					App->imgui->AddConsoleLog(("%s dropped on window", file_dir));

					//get name
					std::string file_name = App->import->CleanFileName(file_dir.c_str());
					App->textures->importer->Import(file_dir.c_str(), file_name);

					if (App->editor->GetSelected() != nullptr)
					{
						ComponentMaterial* new_mat = App->editor->LoadComponentMaterial(file_name.c_str());
						App->editor->GetSelected()->AddComponent(new_mat);
					}
				}

				//export
				else if (file_dir.find(".GTImesh") != std::string::npos)
				{
					App->imgui->AddConsoleLog(("%s dropped on window", file_dir));
					if (App->editor->GetSelected() != nullptr)
					{
						ComponentMesh* new_mesh = App->import->LoadMesh(file_dir.c_str());
						App->editor->GetSelected()->AddComponent(new_mesh);
						App->editor->GetSelected()->UpdateBBox();
					}
					else
					{
						App->editor->CreateNewGameObject(file_dir.c_str());
					}
				}
				else if (file_dir.find(".dds") != std::string::npos)
				{
					App->imgui->AddConsoleLog(("%s dropped on window", file_dir));
					std::string file_name = App->import->CleanFileName(file_dir.c_str());
					App->editor->GetSelected()->AddComponent(App->editor->LoadComponentMaterial(file_name.c_str()));
				}
				else if (file_dir.find(".GTIscene") != std::string::npos)
				{
					App->imgui->AddConsoleLog(("%s dropped on window", file_dir));
					std::string file_name = App->import->CleanFileName(file_dir.c_str());
					App->editor->LoadScene(file_name.c_str());
				}

				break;

			case SDL_WINDOWEVENT:
			
				if(e.window.event == SDL_WINDOWEVENT_RESIZED)
					App->renderer3D->OnResize(e.window.data1, e.window.data2);
			break;
		}
	}

	if(quit == true || (keyboard[SDL_SCANCODE_LALT] == KEY_DOWN && keyboard[SDL_SCANCODE_F4] == KEY_DOWN))
		return UPDATE_STOP;

	if (keyboard[SDL_SCANCODE_0] == KEY_DOWN)
	{
		int random = App->rng->RandomInt(0, 3);
		LOG("random int = %i", random);
		//App->imgui->AddConsoleLog(("random int = %i", random));
	}

	if (keyboard[SDL_SCANCODE_9] == KEY_DOWN)
	{
		float random = App->rng->RandomFloat();
		LOG("random float = %f", random);
		//App->imgui->AddConsoleLog(("random float = %f", random));
	}

	//save load
	if (keyboard[SDL_SCANCODE_F5] == KEY_DOWN)
	{
		App->Save();
	}

	if (keyboard[SDL_SCANCODE_F6] == KEY_DOWN)
	{
		App->Load();
	}


	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleInput::CleanUp()
{
	LOG("Quitting SDL input event subsystem");
	App->imgui->AddConsoleLog("Quitting SDL input event subsystem");
	SDL_QuitSubSystem(SDL_INIT_EVENTS);
	return true;
}