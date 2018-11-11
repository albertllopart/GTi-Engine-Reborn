#include "ModuleTimeManager.h"
#include "Globals.h"
#include "Application.h"
#include "ModuleSceneEditor.h"
#include "ImGui/imgui.h"
#include <stdio.h>

ModuleTimeManager::ModuleTimeManager(Application * app, bool start_enabled) : Module(app, start_enabled)
{
	name = "ModuleTimeManager";

	frame_count = 0;

	time_scale = 0.0f;
	delta_time = 0.0f;
	real_dtime_since_start = 0.0f;

	play_frame = false;
	game_paused = false;
	in_game = false;

	time.Stop();
}

ModuleTimeManager::~ModuleTimeManager()
{
}

bool ModuleTimeManager::Init(JSON_Object * nod)
{
	return true;
}

bool ModuleTimeManager::CleanUp()
{
	return true;
}

update_status ModuleTimeManager::PostUpdate(float dt)
{
	//if (!ImGui::Begin("Time Manager"))
	//{
	//	ImGui::End();
	//}
	if (ImGui::Begin("Time Status"))
	{
		if (ImGui::Button("Play"))
		{
			PlayGame(!in_game);
			//Change Main camera??
		}
		ImGui::SameLine();
		if (ImGui::Button("Pause"))
		{
			PauseGame(!game_paused);
		}
		ImGui::SameLine();
		if (play_frame == true)
		{
			play_frame = false;
		}
		if (ImGui::Button("Play One Frame") && game_paused == true)
		{
			PlayOneFrame();
		}

		ImGui::Text("Total Frames: %i", GetFrameCount());
		ImGui::Text("Game Time Clock: %.2f", GetTime());
		ImGui::Text("Real Time Clock: %.2f", GetRealTime());

		if (in_game == true)
		{
			ImGui::Text("Runing!");
		}
		if (in_game == false)
		{
			ImGui::Text("Paused!");
		}
	}
	ImGui::End();

	return UPDATE_CONTINUE;
}

void ModuleTimeManager::PlayGame(bool play)
{
	in_game = play;

	if (play == true)
	{
		time.Start();
		App->editor->SaveScene("MainScene");
	}
	if (play == false)
	{
		time.Start();
		time.Stop();
		App->editor->SetSelected(nullptr);
		//uncomment once we have Assets folder
		//App->editor->LoadScene("Assets/Scenes/MainScene.GTIscene");
	}
}

void ModuleTimeManager::PauseGame(bool pause)
{
	game_paused = pause;

	if (pause == true)
	{
		time.Stop();
	}
	if (pause == false)
	{
		time.Continue();
	}
}

void ModuleTimeManager::PlayOneFrame()
{
	play_frame = true;
}

bool ModuleTimeManager::IsGamePaused() const
{
	return game_paused;
}

void ModuleTimeManager::StartTime()
{
	time.Start();
}

void ModuleTimeManager::StartRealTime()
{
	real_time_since_start.Start();
}

void ModuleTimeManager::StopTime()
{
	time.Stop();
}

void ModuleTimeManager::StopRealTime()
{
	real_time_since_start.Stop();
}

void ModuleTimeManager::SetFrameCount(uint amount)
{
	frame_count += amount;
}

uint ModuleTimeManager::GetFrameCount() const
{
	return frame_count;
}

float ModuleTimeManager::GetTime()
{
	return (float)time.Read() / 1000.0f;
}

float ModuleTimeManager::GetTimeScale() const
{
	return time_scale;
}

float ModuleTimeManager::GetDeltaTime() const
{
	return delta_time;
}

float ModuleTimeManager::GetRealTime()
{
	return (float)real_time_since_start.Read() / 1000.0f;
}

float ModuleTimeManager::GetRealDeltaTime() const
{
	return real_dtime_since_start;
}

bool ModuleTimeManager::GetPlayOneFrame() const
{
	return play_frame;
}
