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

	void ShowConfigurationMenu(bool* opened = NULL);
	void PerformanceGraphCalc(float fps, float ms);
	void AboutWindow();
public:

	bool demo = false;
	bool showconsole = false;
	bool config_menu = false;
	bool about = false;

	std::vector<float> FPSvec;
	std::vector<float> MSvec;


};

#endif // __MODULEIMGUI_H__