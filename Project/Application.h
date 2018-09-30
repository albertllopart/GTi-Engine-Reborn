#pragma once

#include "Globals.h"
#include "Timer.h"
#include "Module.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleAudio.h"
#include "ModuleRenderer3D.h"
#include "ModuleCamera3D.h"
#include "ModuleImGui.h"
#include "ModuleRNG.h"
#include "ModuleSceneEditor.h"
#include <list>
#include <vector>
#include <string>

class Application
{
public:

	ModuleWindow* window;
	ModuleInput* input;
	ModuleAudio* audio;
	ModuleRenderer3D* renderer3D;
	ModuleCamera3D* camera;
	ModuleImGui* imgui;
	ModuleRNG* rng;
	ModuleSceneEditor* editor;

private:

	Timer	ms_timer;
	float	dt;
	float lastFPS = 0;
	float lastMs = 0;
	std::list<Module*> list_modules;

	int capped_ms;
	mutable int max_fps = 60;

	//load save
	mutable bool want_to_save = false;
	mutable bool want_to_load = false;

public:

	Application();
	~Application();

	bool Init();
	update_status Update();
	bool CleanUp();

	float GetFPS();
	float GetMs();
	int* GetMaxFPS();
	
	void Save();
	void Load();

private:

	void AddModule(Module* mod);
	void PrepareUpdate();
	void FinishUpdate();

	//load save
	void SaveEngine() const;
	void LoadEngine() const;

};