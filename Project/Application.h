#pragma once

#include "Globals.h"
#include "Timer.h"
#include "Module.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleRenderer3D.h"
#include "ModuleCamera3D.h"
#include "ModuleImGui.h"
#include "ModuleImporter.h"
#include "ModuleRNG.h"
#include "ModuleSceneEditor.h"
#include "ModuleTextures.h"
#include <list>
#include <vector>
#include <string>

class Application
{
public:

	ModuleWindow* window;
	ModuleInput* input;
	ModuleRenderer3D* renderer3D;
	ModuleCamera3D* camera;
	ModuleImGui* imgui;
	ModuleRNG* rng;
	ModuleSceneEditor* editor;
	ModuleImporter* import;
	ModuleTextures* textures;

private:

	std::list<Module*> list_modules;

	//timer related

	Timer	frame_time;
	Timer   ms_time;

	uint frames = 0;
	int fps_counter;

	float dt;
	float lastFPS;
	float lastMs;
	
	int capped_ms;

	mutable int max_fps;

	//load save
	mutable bool want_to_save = false;
	mutable bool want_to_load = false;

public:

	std::string appname;
	std::string organization;

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

extern Application* App;