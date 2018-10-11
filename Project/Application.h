#pragma once

#include "Globals.h"
#include "Timer.h"
#include "PerfTimer.h"
#include "Module.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleAudio.h"
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
	ModuleAudio* audio;
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
	Timer   last_sec_frame_time;
	PerfTimer perftimer;

	double frame_count = 0;
	int last_sec_frame_count = 0;
	int prev_last_sec_frame_count = 0;

	float	dt;
	float lastFPS = 0;
	float lastMs = 0;
	
	int capped_ms;
	mutable int max_fps = 60;

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