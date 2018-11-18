#ifndef __MODULE_ENGINE_WINDOWS__
#define __MODULE_ENGINE_WINDOWS__

#include "Application.h"
#include "Globals.h"
#include "Module.h"

enum LoadFile
{
	LOAD_NONE,
	LOAD_MESH,
	LOAD_TEXTURE,
	LOAD_SCENE
};

class E_Windows;
class E_Hierarchy;
class E_Inspector;
class E_Folder;

class ModuleEngineWindows : public Module
{
public:

	ModuleEngineWindows(Application* app, bool start_enabled = true);
	~ModuleEngineWindows();

	bool Start();
	update_status PreUpdate(float dt);
	update_status Update(float dt);
	update_status PostUpdate(float dt);
	update_status OnEditor();

	void WantToLoad(const char* path);
	bool CleanUp();
	//void SetSelectedResource(const char * path);

	LoadFile DetermineFileFromPath(const char * path);

private:
	std::vector<E_Windows*> e_windows;
	E_Hierarchy* e_hierarchy = nullptr;
	E_Inspector* e_inspector = nullptr;
	E_Folder* e_folder = nullptr;

	bool want_to_load = false;
	LoadFile next_load = LOAD_NONE;
	std::string path_to_load;
};


#endif // !__MODULE_ENGINE_WINDOWS__
