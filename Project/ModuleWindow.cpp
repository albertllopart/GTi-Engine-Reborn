#include "Globals.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "parson/parson.h"

ModuleWindow::ModuleWindow(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	window = NULL;
	screen_surface = NULL;

	name = "Window";
}

// Destructor
ModuleWindow::~ModuleWindow()
{
}

// Called before render is available
bool ModuleWindow::Init(JSON_Object* node)
{
	LOG("Init SDL window & surface");
	App->imgui->AddConsoleLog("Init SDL window & surface");
	bool ret = true;

	if(SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		LOG("SDL_VIDEO could not initialize! SDL_Error: %s\n", SDL_GetError());
		App->imgui->AddConsoleLog(("SDL_VIDEO could not initialize! SDL_Error: %s\n", SDL_GetError()));
		ret = false;
	}
	else
	{
		//Create window
		int width = json_object_get_number(node, "width") * json_object_get_number(node, "size");
		this->width = width;
		int height = json_object_get_number(node, "height") * json_object_get_number(node, "size");
		this->height = height;
		Uint32 flags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN;

		//Use OpenGL 2.1
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

		if(json_object_get_boolean(node, "fullscreen") == true)
		{
			fullscreen = true;
			flags |= SDL_WINDOW_FULLSCREEN;
		}

		if(json_object_get_boolean(node, "resizable") == true)
		{
			resizable = true;
			flags |= SDL_WINDOW_RESIZABLE;
		}

		if(json_object_get_boolean(node, "borderless") == true)
		{
			borderless = true;
			flags |= SDL_WINDOW_BORDERLESS;
		}

		if(json_object_get_boolean(node, "fulldesktop") == true)
		{
			fulldesktop = true;
			flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
		}

		window = SDL_CreateWindow(json_object_get_string(node, "title"), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, width, height, flags);

		if(window == NULL)
		{
			LOG("Window could not be created! SDL_Error: %s\n", SDL_GetError());
			App->imgui->AddConsoleLog(("Window could not be created! SDL_Error: %s\n", SDL_GetError()));
			ret = false;
		}
		else
		{
			//Get window surface
			screen_surface = SDL_GetWindowSurface(window);
		}
	}

	return ret;
}

// Called before quitting
bool ModuleWindow::CleanUp()
{
	LOG("Destroying SDL window and quitting all SDL systems");
	App->imgui->AddConsoleLog("Destroying SDL window and quitting all SDL systems");

	//Destroy window
	if(window != NULL)
	{
		SDL_DestroyWindow(window);
	}

	//Quit SDL subsystems
	SDL_Quit();
	return true;
}

void ModuleWindow::SetTitle(const char* title)
{
	SDL_SetWindowTitle(window, title);
}

SDL_Window * ModuleWindow::GetWindowPtr()
{
	return window;
}

void ModuleWindow::SetFullscreen(bool fullscreen)
{
	Uint32 flags;
	if (fullscreen)
	{
		flags |= SDL_WINDOW_FULLSCREEN;
	}
	SDL_SetWindowFullscreen(window, flags);
}

void ModuleWindow::SetResizable(bool resizable)
{
	if (resizable)
	{
		this->resizable = true;
	}
	else
	{
		this->resizable = false;
	}
}

void ModuleWindow::ResizeWindow(int width, int height)
{
	this->width = width;
	this->height = height;
	SDL_SetWindowSize(window, width, height);
}

void ModuleWindow::SetBrightness(float value)
{
	SDL_SetWindowBrightness(window, value);
}

void ModuleWindow::SetBorderless(bool borderless)
{
	if (borderless)
	{
		SDL_SetWindowBordered(window, SDL_FALSE);
	}
	else
	{
		SDL_SetWindowBordered(window, SDL_TRUE);
	}
}

void ModuleWindow::SetFullDesktop(bool fullDesktop)
{
	Uint32 flags;
	if (fullDesktop == true)
	{
		flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
	}
	SDL_SetWindowFullscreen(window, flags);
}

void ModuleWindow::Save(JSON_Object* node)
{
	LOG("Saving Window Module...");
	App->imgui->AddConsoleLog("Saving Window Module...");

	json_object_set_number(node, "width", width);
	json_object_set_number(node, "height", height);

	json_object_set_boolean(node, "fullscreen", fullscreen);
	json_object_set_boolean(node, "resizable", resizable);
	json_object_set_boolean(node, "borderless", borderless);
	json_object_set_boolean(node, "fulldesktop", fulldesktop);

}

void ModuleWindow::Load(JSON_Object* node)
{
	LOG("Loading Window Module...");
	App->imgui->AddConsoleLog("Loading Window Module...");

	width = json_object_get_number(node, "width");
	height = json_object_get_number(node, "height");
				
	fullscreen = json_object_get_boolean(node, "fullscreen"); SetFullscreen(fullscreen);
	resizable = json_object_get_boolean(node, "resizable"); SetResizable(resizable);
	borderless = json_object_get_boolean(node, "borderless"); SetBorderless(borderless);
	fulldesktop = json_object_get_boolean(node, "fulldesktop"); SetFullDesktop(fulldesktop);
}