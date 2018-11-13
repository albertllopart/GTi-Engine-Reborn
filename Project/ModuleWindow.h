#ifndef __ModuleWindow_H__
#define __ModuleWindow_H__

#include "Module.h"
#include "SDL/include/SDL.h"

class Application;

class ModuleWindow : public Module
{
public:

	ModuleWindow(Application* app, bool start_enabled = true);

	virtual ~ModuleWindow();

	bool Init(JSON_Object* node);
	bool CleanUp();

	SDL_Window* GetWindowPtr();
	void SetTitle(const char* title);
	void SetFullscreen(bool fullscreen);
	void SetResizable(bool resizable);
	void ResizeWindow(int width, int height);
	void SetBrightness(float value);
	void SetBorderless(bool borderless);
	void SetFullDesktop(bool fullDesktop);
	void UpdateWindowParameters();

	void Save(JSON_Object* node) const;
	void Load(JSON_Object* node);

public:
	//render window
	SDL_Window* window;
	//The surface contained by the window
	SDL_Surface* screen_surface;


	int width;
	int height;

	bool fullscreen = false;
	bool fulldesktop = false;
	bool borderless = false;
	bool resizable = false;
	float brightness = 1.0f;

};

#endif // __ModuleWindow_H__