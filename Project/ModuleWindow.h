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

	bool Init();
	bool CleanUp();

	SDL_Window* GetWindowPtr();
	void SetTitle(const char* title);
	void SetFullscreen(bool fullscreen);
	void ResizeWindow(int width, int height);
	void SetBrightness(float value);
	void SetBorderless(bool borderless);
	void SetFullDesktop(bool fullDesktop);

public:
	//render window
	SDL_Window* window;
	//The surface contained by the window
	SDL_Surface* screen_surface;


	int width;
	int height;

};

#endif // __ModuleWindow_H__