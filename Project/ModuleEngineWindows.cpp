#include "ModuleEngineWindows.h"
#include "E_Windows.h"
#include "E_Hierarchy.h"
#include "E_Inspector.h"

ModuleEngineWindows::ModuleEngineWindows(Application * app, bool start_enabled):Module(app, start_enabled)
{
	name = "Engine Windows";
	e_hierarchy = new E_Hierarchy(this);
	e_windows.push_back(e_hierarchy);
	e_inspector = new E_Inspector(this);
	e_windows.push_back(e_inspector);
}

ModuleEngineWindows::~ModuleEngineWindows()
{
}

bool ModuleEngineWindows::Start()
{
	return true;
}

update_status ModuleEngineWindows::PreUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

update_status ModuleEngineWindows::Update(float dt)
{
	return UPDATE_CONTINUE;
}

update_status ModuleEngineWindows::PostUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

update_status ModuleEngineWindows::OnEditor()
{
	//manages draw of engine windows
	bool ret = false;
	for (uint i = 0; i < e_windows.size(); i++)
	{
		if (e_windows[i]->IsActive())
		{
			ret = e_windows[i]->Draw();
		}
	}
	if (!ret)
	{
		return UPDATE_STOP;
	}
	return UPDATE_CONTINUE;
}

bool ModuleEngineWindows::CleanUp()
{
	bool ret = true;
	for (uint i = 0; i < e_windows.size() && ret; i++)
	{
		e_windows[i]->CleanUp();
	}
	RELEASE(e_hierarchy);
	e_windows.clear();
	return ret;
}
