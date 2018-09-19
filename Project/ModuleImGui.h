#ifndef __MODULEIMGUI_H__
#define __MODULEIMGUI_H__

#include "Module.h"
#include "Globals.h"

class ModuleImGui : public Module
{
public:
	ModuleImGui(Application* app, bool start_enabled = true);
	~ModuleImGui();

	bool Start();
	bool CleanUp();

	update_status Update(float dt);
	update_status PreUpdate(float dt);

	bool demo = false;
public:

};

#endif // __MODULEIMGUI_H__