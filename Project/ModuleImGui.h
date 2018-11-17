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
	void ShowConsole();
	void AddConsoleLog(const char* add);
	void AddConsoleLog(std::string add);
	void PerformanceGraphCalc(float fps, float ms);
	void AboutWindow();
	void Draw()const;
	bool Confirmation();

	std::vector<float> GetFramerateVec() const;

public:

	bool demo = false;
	bool showconsole = false;
	bool config_menu = false;
	bool about = false;
	bool show_mesh_info = false;
	bool hierarchy = true;
	bool confirmation = false;
	//
	bool text_import = false;
	bool mesh_import = false;

	std::vector<float> FPSvec;
	std::vector<float> MSvec;
	std::vector<std::string> consoleLogs;

};

#endif // __MODULEIMGUI_H__