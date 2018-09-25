#include "Globals.h"
#include "Application.h"
#include "ModuleImGui.h"
#include "ImGui\imgui.h"
#include "ImGui\imgui_impl_sdl_gl3.h"
#include "Glew\include\glew.h"
#include "ModuleWindow.h"

#pragma comment( lib, "Glew/libx86/glew32.lib" )

#define MAX_FPS_CAP 60



ModuleImGui::ModuleImGui(Application* app, bool start_enabled) : Module(app, start_enabled)
{
}

ModuleImGui::~ModuleImGui()
{}

bool ModuleImGui::Start()
{
	LOG("Starting glew & ImGui");
	glewInit();
	ImGui_ImplSdlGL3_Init(App->window->GetWindowPtr());

	return true;
}

update_status ModuleImGui::PreUpdate(float dt)
{
	ImGui_ImplSdlGL3_NewFrame(App->window->GetWindowPtr());
	return(UPDATE_CONTINUE);
}

update_status ModuleImGui::Update(float dt)
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Save"))
			{
				App->SaveConfig();
			}
			if (ImGui::MenuItem("Quit"))
			{
				return UPDATE_STOP;
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("View"))
		{
			if (ImGui::MenuItem("Console"))
			{
				showconsole = !showconsole;
			}
			if (ImGui::MenuItem("Configuration"))
			{
				config_menu = !config_menu;
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("Help"))
		{
			if (ImGui::MenuItem("ImGui Demo"))
			{
				demo = !demo;
			}
			if (ImGui::MenuItem("Documentation"))
			{
				ShellExecuteA(NULL, "open", "https://github.com/albertllopart/GTi-Engine-Reborn/blob/master/README.md", NULL, NULL, SW_SHOWNORMAL);
			}
			if (ImGui::MenuItem("Latest Release"))
			{
				ShellExecuteA(NULL, "open", "https://github.com/albertllopart/GTi-Engine-Reborn/releases", NULL, NULL, SW_SHOWNORMAL);
			}
			if (ImGui::MenuItem("Report a bug"))
			{
				ShellExecuteA(NULL, "open", "https://github.com/albertllopart/GTi-Engine-Reborn/issues", NULL, NULL, SW_SHOWNORMAL);
			}
			if (ImGui::MenuItem("About"))
			{
				about = !about;
			}
			ImGui::EndMenu();
		}
		if (about)
		{
			AboutWindow();
		}
		ImGui::EndMainMenuBar();
	}

	if (config_menu)
	{
		ShowConfigurationMenu();
	}
	if (showconsole)
	{

	}
	if (demo)
	{
		ImGui::ShowTestWindow();
	}
	ImGui::Render();
	return UPDATE_CONTINUE;
}

bool ModuleImGui::CleanUp()
{
	LOG("Unloading Intro scene");
	ImGui_ImplSdlGL3_Shutdown();
	return true;
}

void ModuleImGui::ShowConfigurationMenu(bool* opened)
{
	if (ImGui::Begin("Configuration"))
	{

		ImGui::PushItemWidth(-140);                                 
		ImGui::Text("Options");
		if (ImGui::CollapsingHeader("Application"))
		{
		
			ImGui::InputText("App Name", "GTi-Engine", 30);
			ImGui::InputText("Organization", "UPC CITM", 30);
			ImGui::SliderInt("Max FPS", App->GetMaxFPS(), 0, 300);

			PerformanceGraphCalc(App->GetFPS(), App->GetMs());
			char title[25];
			sprintf_s(title, 25, "Framerate %.1f", FPSvec[FPSvec.size() - 1]);
			ImGui::PlotHistogram("##framerate", &FPSvec[0], FPSvec.size(), 0, title, 0.0f, 100.0f, ImVec2(310, 100));
			sprintf_s(title, 25, "Milliseconds %0.1f", MSvec[MSvec.size() - 1]);
			ImGui::PlotHistogram("##milliseconds", &MSvec[0], MSvec.size(), 0, title, 0.0f, 40.0f, ImVec2(310, 100));
		}
		if (ImGui::CollapsingHeader("Window"))
		{
			if (ImGui::SliderFloat("Brightness", &App->window->brightness, 0.0f, 1.0f, "%.2f"))
			{
				App->window->SetBrightness(App->window->brightness);
			}
			if (ImGui::SliderInt("Width", &App->window->width, 1, 1920))
			{
				if (App->window->resizable)
					App->window->ResizeWindow(App->window->width, App->window->height);
			}
			if (ImGui::SliderInt("Height", &App->window->height, 1, 1080))
			{
				if (App->window->resizable)
					App->window->ResizeWindow(App->window->width, App->window->height);
			}

			if (ImGui::Checkbox("Fullscreen", &App->window->fullscreen))
				App->window->SetFullscreen(App->window->fullscreen);

			ImGui::SameLine();
			if (ImGui::Checkbox("Resizable", &App->window->resizable))
				App->window->SetResizable(App->window->resizable);

			if (ImGui::Checkbox("Borderless", &App->window->borderless))
				App->window->SetBorderless(App->window->borderless);

			ImGui::SameLine();
			if (ImGui::Checkbox("Full Desktop", &App->window->fulldesktop))
				App->window->SetFullDesktop(App->window->fulldesktop);

		}
		
		if (ImGui::CollapsingHeader("Hardware"))
		{
			ImGui::Text("SDL Version:"); ImGui::SameLine(); ImGui::TextColored(ImVec4(0.f, 1.f, 1.f, 1.f), "2.0.4");
		}
	}
	ImGui::End();
}

void ModuleImGui::PerformanceGraphCalc(float fps, float ms)
{
	if (FPSvec.size() >= MAX_FPS_CAP)
	{
		for (int i = 0; i < MAX_FPS_CAP - 2; i++)
		{
			FPSvec[i] = FPSvec[i + 1];
		}
		FPSvec[MAX_FPS_CAP - 1] = fps;
	}
	else
	{
		FPSvec.push_back(fps);
	}

	////////////////////////////////

	if (MSvec.size() >= MAX_FPS_CAP)
	{
		for (int i = 0; i < MAX_FPS_CAP - 2; i++)
		{
			MSvec[i] = MSvec[i + 1];
		}
		MSvec[MAX_FPS_CAP - 1] = ms;
	}
	else
	{
		MSvec.push_back(ms);
	}
}

void ModuleImGui::AboutWindow()
{
	ImGui::Begin("About");
	ImGui::Text("GTi Engine");
	ImGui::Text("A game engine developed by Albert Llopart & Marc Fabian");

	if (ImGui::CollapsingHeader("Libraries used"))
	{
		if (ImGui::MenuItem("SDL 2.0.4"))
		{
			ShellExecuteA(NULL, "open", "https://www.libsdl.org/index.php", NULL, NULL, SW_SHOWNORMAL);
		}
		if (ImGui::MenuItem("MathGeoLib 1.3"))
		{
			ShellExecuteA(NULL, "open", "http://clb.demon.fi/MathGeoLib/nightly/", NULL, NULL, SW_SHOWNORMAL);
		}
		if (ImGui::MenuItem("ImGui 1.51"))
		{
			ShellExecuteA(NULL, "open", "https://github.com/ocornut/imgui", NULL, NULL, SW_SHOWNORMAL);
		}
		if (ImGui::MenuItem("OpenGl 3.1"))
		{
			ShellExecuteA(NULL, "open", "https://www.opengl.org/", NULL, NULL, SW_SHOWNORMAL);
		}
	}
	if (ImGui::CollapsingHeader("License"))
	{
		if (ImGui::MenuItem("License MIT"))
		{
			ShellExecuteA(NULL, "open", "https://github.com/xDragan/GTi-Engine/blob/master/LICENSE", NULL, NULL, SW_SHOWNORMAL);
		}
	}
	ImGui::End();
	
}
