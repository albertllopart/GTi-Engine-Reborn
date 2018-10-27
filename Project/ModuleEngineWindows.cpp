#include "ModuleEngineWindows.h"

ModuleEngineWindows::ModuleEngineWindows(Application * app, bool start_enabled):Module(app, start_enabled)
{
	name = "Engine Windows";
}

ModuleEngineWindows::~ModuleEngineWindows()
{
}

bool ModuleEngineWindows::Start()
{
	return false;
}

update_status ModuleEngineWindows::PreUpdate(float dt)
{
	return update_status();
}

update_status ModuleEngineWindows::Update(float dt)
{
	return update_status();
}

update_status ModuleEngineWindows::PostUpdate(float dt)
{
	return update_status();
}

bool ModuleEngineWindows::CleanUp()
{
	return false;
}
