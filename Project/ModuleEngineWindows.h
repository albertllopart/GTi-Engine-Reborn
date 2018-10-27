#ifndef __MODULE_ENGINE_WINDOWS__
#define __MODULE_ENGINE_WINDOWS__

#include "Application.h"
#include "Globals.h"
#include "Module.h"

class ModuleEngineWindows : public Module
{
public:

	ModuleEngineWindows(Application* app, bool start_enabled = true);
	~ModuleEngineWindows();

	bool Start();
	update_status PreUpdate(float dt);
	update_status Update(float dt);
	update_status PostUpdate(float dt);

	bool CleanUp();

private:
	std::vector<E_Windows> e_windows;

};


#endif // !__MODULE_ENGINE_WINDOWS__
