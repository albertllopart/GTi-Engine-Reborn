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
	ImGui::SetNextWindowPos(ImVec2(SDL_GetWindowSurface(App->window->window)->w/2.5f, 20), ImGuiCond_Always);

	if(show_info)
		ImGui::SetNextWindowSize(ImVec2(250, 120), ImGuiCond_Always);
	else
		ImGui::SetNextWindowSize(ImVec2(250, 70), ImGuiCond_Always);

	ImGuiWindowFlags flags = 0;
	flags |= ImGuiWindowFlags_NoResize;
	flags |= ImGuiWindowFlags_NoScrollbar;
	

	if (ImGui::Begin("Time Status",NULL,flags))
	{
		if (ImGui::Button("Play"))
		{
			PlayGame(!in_game);
		}
		ImGui::SameLine();
		if (ImGui::Button("Pause"))
		{
			if (in_game)
			{
				PauseGame(!game_paused);
			}
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
		ImGui::SameLine();
		if (show_info)
		{
			if (ImGui::Button("-"))
			{
				show_info = !show_info;
				//ImGui::SetNextWindowSize(ImVec2(200, 300), ImGuiCond_Always);
			}
		}
		else
		{
			if (ImGui::Button("+"))
			{
				show_info = !show_info;
				//ImGui::SetNextWindowSize(ImVec2(200, 600), ImGuiCond_Always);
			}
		}


		if (show_info)
		{
			ShowInfo();
		}
		if (in_game)
		{
			ImGui::TextColored(ImVec4(0.2f, 0.9f, 0.2f, .8f), "Playing");
		}
		if (!in_game)
		{
			ImGui::TextColored(ImVec4(0.9f, 0.2f, 0.2f, 0.8f), "Not Playing");
		}
	}
	ImGui::End();

	return UPDATE_CONTINUE;
}

void ModuleTimeManager::ShowInfo()
{
	ImGui::Text("Total Frames: %i", GetFrameCount());
	ImGui::Text("Game Time Clock: %.2f", GetTime());
	ImGui::Text("Real Time Clock: %.2f", GetRealTime());
}

void ModuleTimeManager::PlayGame(bool play)
{
	in_game = play;

	if (play == true)
	{
		time.Start();
		App->editor->SaveScene("MainScene");
		App->renderer3D->SetCamera(App->editor->main_camera);
	}
	if (play == false)
	{
		time.Start();
		time.Stop();
		App->editor->SetSelected(nullptr);
		App->renderer3D->SetCamera(App->camera->camera);
		App->editor->WantToLoadScene("MainScene");
	}
}

void ModuleTimeManager::PauseGame(bool pause)
{
	if (pause == true)
	{
		time.Stop();
	}
	if (pause == false)
	{
		time.Continue();
	}
	game_paused = pause;
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
