#include "Globals.h"
#include "Application.h"
#include "ModuleImGui.h"
#include "ImGui\imgui.h"
#include "ImGui\imgui_impl_sdl_gl3.h"
#include "Glew\include\glew.h"
#include "ModuleWindow.h"
#include "SDL/include/SDL_opengl.h"
#include <gl/GL.h>
#include <gl/GLU.h>
#include <stdio.h>
#include <string.h>


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
	AddConsoleLog("Starting glew & ImGui");

	ImGui_ImplSdlGL3_Init(App->window->GetWindowPtr());
	
	ImGui::GetStyle().WindowRounding = 0.0f;

	return true;
}

update_status ModuleImGui::PreUpdate(float dt)
{
	ImGui_ImplSdlGL3_NewFrame(App->window->GetWindowPtr());
	return(UPDATE_CONTINUE);
}

update_status ModuleImGui::Update(float dt)
{
	if (App->input->GetKey(SDL_SCANCODE_0) == KEY_DOWN)
	{
		AddConsoleLog("Shinny Mewtoo appeared!");
	}
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Load (F6)"))
			{
				App->Load();
			}
			if (ImGui::MenuItem("Save (F5)"))
			{
				App->Save();
			}
			if (ImGui::MenuItem("Quit (Alt + F4)"))
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
			if (ImGui::MenuItem("Meshes Parameters"))
			{
				show_mesh_info = !show_mesh_info;
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
		ShowConsole();
	}
	if (demo)
	{
		ImGui::ShowTestWindow();
	}
	if (show_mesh_info)
	{
		ShowMeshesInfo();
	}
	return UPDATE_CONTINUE;
}

bool ModuleImGui::CleanUp()
{
	LOG("Unloading ImGui Module");
	AddConsoleLog("Unloading ImGui Module");
	ImGui_ImplSdlGL3_Shutdown();
	return true;
}

void ModuleImGui::ShowConfigurationMenu(bool* opened)
{
	if (config_menu)
	{
		ImGui::Begin("Configuration", &config_menu);
		{

			ImGui::PushItemWidth(-140);
			ImGui::Text("Options");
			if (ImGui::CollapsingHeader("Application"))
			{
				static char appname[100];
				strcpy_s(appname, 100, App->appname.c_str());

				if (ImGui::InputText("App Name", appname, 100))
				{
					App->window->SetTitle(appname);
					App->appname = appname;
				}

				static char organization[100];
				strcpy_s(organization, 100, App->organization.c_str());

				if (ImGui::InputText("Organization", organization, 20))
				{
					App->organization = organization;
				}

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
				SDL_version version;
				SDL_GetVersion(&version);
				ImGui::Text("SDL Version:"); ImGui::SameLine(); ImGui::TextColored(ImVec4(0.f, 1.f, 1.f, 1.f), "%i.%i.%i", version.major, version.minor, version.patch);
				ImGui::Text("OpenGL Version:"); ImGui::SameLine(); ImGui::TextColored(ImVec4(0.f, 1.f, 1.f, 1.f), "%s", glGetString(GL_VERSION));
				ImGui::Text("GLSL:"); ImGui::SameLine(); ImGui::TextColored(ImVec4(0.f, 1.f, 1.f, 1.f), "%s", glGetString(GL_SHADING_LANGUAGE_VERSION));
				ImGui::Separator();
				ImGui::Text("CPUs:"); ImGui::SameLine(); ImGui::TextColored(ImVec4(0.f, 1.f, 1.f, 1.f), "%i", SDL_GetCPUCount()); ImGui::SameLine(); ImGui::TextColored(ImVec4(0.f, 1.f, 1.f, 1.f), "(Cache: %ikb)", SDL_GetCPUCacheLineSize());
				ImGui::Text("System RAM:"); ImGui::SameLine(); ImGui::TextColored(ImVec4(0.f, 1.f, 1.f, 1.f), "%.2fGb", SDL_GetSystemRAM() / 1024.0f);
				//ImGui::Text("Caps:"); ImGui::SameLine(); ImGui::TextColored(ImVec4(0.f, 1.f, 1.f, 1.f), "%s%s%s%s%s%s%s%s%s%s%s", (SDL_Has3DNow()) ? "3DNow, " : "", (SDL_HasAVX()) ? "AVX, " : "", (SDL_HasAVX2()) ? "AVX2, " : "", (SDL_HasAltiVec()) ? "AltiVec, " : "", (SDL_HasMMX()) ? "MMX, " : "", (SDL_HasRDTSC()) ? "RDTSC, " : "", (SDL_HasSSE()) ? "SSE, " : "", (SDL_HasSSE2()) ? "SSE2, " : "", (SDL_HasSSE3()) ? "SSE3, " : "", (SDL_HasSSE41()) ? "SSE41, " : "", (SDL_HasSSE42()) ? "SSE42 " : "");
				ImGui::Separator();
				ImGui::Text("GPU:"); ImGui::SameLine(); ImGui::TextColored(ImVec4(0.f, 1.f, 1.f, 1.f), "%s", glGetString(GL_RENDERER));
				ImGui::Text("Brand:"); ImGui::SameLine(); ImGui::TextColored(ImVec4(0.f, 1.f, 1.f, 1.f), "%s", glGetString(GL_VENDOR));

				GLint budget;
				GLint available;
				GLint reserved;
				glGetIntegerv(0x9048, &budget);
				glGetIntegerv(0x9049, &available);
				glGetIntegerv(0x9047, &reserved);
				ImGui::Text("VRAM Budget:"); ImGui::SameLine(); ImGui::TextColored(ImVec4(0.f, 1.f, 1.f, 1.f), "%.2fMb", budget / 1024.0f);
				ImGui::Text("VRAM Usage:"); ImGui::SameLine(); ImGui::TextColored(ImVec4(0.f, 1.f, 1.f, 1.f), "%.2fMb", (budget / 1024.0f) - (available / 1024.0f));
				ImGui::Text("VRAM Available:"); ImGui::SameLine(); ImGui::TextColored(ImVec4(0.f, 1.f, 1.f, 1.f), "%.2fMb", available / 1024.0f);
				ImGui::Text("VRAM Reserved:"); ImGui::SameLine(); ImGui::TextColored(ImVec4(0.f, 1.f, 1.f, 1.f), "%.2fMb", reserved / 1024.0f);
			}
			if (ImGui::CollapsingHeader("Render"))
			{
				if (ImGui::Checkbox("Wireframe Mode", &App->renderer3D->wireframe))
				{
					//App->renderer3D->SetWireFrame();
				}
				if (ImGui::Checkbox("Depth Test", &App->renderer3D->depthTest))
				{
					App->renderer3D->SetDepthTest();
				}
				if (ImGui::Checkbox("Cull Face", &App->renderer3D->cullFace))
				{
					App->renderer3D->SetCullFace();
				}
				if (ImGui::Checkbox("Lighting", &App->renderer3D->lighting))
				{
					App->renderer3D->SetLighting();
				}
				if (ImGui::Checkbox("Color Material", &App->renderer3D->colorMaterial))
				{
					App->renderer3D->SetColorMaterial();
				}
				if (ImGui::Checkbox("2D Textures", &App->renderer3D->texture2D))
				{
					App->renderer3D->SetTexture2D();
				}
				ImGui::Checkbox("Show Normals", &App->renderer3D->show_normals);
			}
		}
		ImGui::End();
	}
	
}

void ModuleImGui::ShowConsole()
{

	if (showconsole)
	{
		ImGui::Begin("Console", &showconsole);

		if (ImGui::Button("Clear"))
		{
			consoleLogs.clear();
		}
		for (int i = consoleLogs.size() - 1; i >= 0; i--)
		{
			std::string s = consoleLogs[i];
			ImGui::Text("%s", consoleLogs[i].c_str());
		}
	}
	ImGui::End();
}

void ModuleImGui::AddConsoleLog(const char * add)
{
	std::string log = add;
	consoleLogs.push_back(log);
}

void ModuleImGui::AddConsoleLog(std::string add)
{
	consoleLogs.push_back(add);
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
	//FALTA POSAR DEVIL I ASSIMP 
	if (ImGui::CollapsingHeader("License"))
	{
		if (ImGui::MenuItem("License MIT"))
		{
			ShellExecuteA(NULL, "open", "https://github.com/xDragan/GTi-Engine/blob/master/LICENSE", NULL, NULL, SW_SHOWNORMAL);
		}
	}
	ImGui::End();
	
}

void ModuleImGui::ShowMeshesInfo()
{
	if (show_mesh_info)
	{
		ImGui_ImplSdlGL3_NewFrame(App->window->GetWindowPtr());
		std::list<Mesh*> scene_meshes = App->editor->GetMeshList();
		ImGui::Begin("Meshes properties", &show_mesh_info);

		if (App->editor->GetMeshList().size() > 0)
		{
			for (std::list<Mesh*>::const_iterator iterator = scene_meshes.begin(), end = scene_meshes.end(); iterator != scene_meshes.end(); ++iterator)
			{
				const char *test = iterator._Ptr->_Myval->name.c_str();
				if (ImGui::CollapsingHeader(test))
				{
					ImGui::Text("Position:");
					ImGui::Text("X: %f Y:%f Z:%f", iterator._Ptr->_Myval->pos.x, iterator._Ptr->_Myval->pos.y, iterator._Ptr->_Myval->pos.z);
					ImGui::Text("Scale:");
					ImGui::Text("X: %f Y:%f Z:%f", iterator._Ptr->_Myval->scale.x, iterator._Ptr->_Myval->scale.y, iterator._Ptr->_Myval->scale.z);
					//FALTA MOSTRAR ROTACIONS (HEM DE PASSAR EL QUATERNIÓ A ANGLES D'EULER)
				}

			}
		}
		else
		{
			App->imgui->AddConsoleLog("Could not load mesh info");
		}

		ImGui::End();
	}
}

void ModuleImGui::Draw()const
{
	ImGui::Render();
}

std::vector<float> ModuleImGui::GetFramerateVec() const
{
	return FPSvec;
}
