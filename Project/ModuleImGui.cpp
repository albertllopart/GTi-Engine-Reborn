#include "Globals.h"
#include "Application.h"
#include "ModuleImGui.h"
#include "ImGui\imgui.h"
#include "ImGui\imgui_impl_sdl_gl3.h"
#include "Glew\include\glew.h"
#include "ModuleWindow.h"
#include "GameObject.h"
#include "ModuleSceneEditor.h"
#include "SDL/include/SDL_opengl.h"
#include <gl/GL.h>
#include <gl/GLU.h>
#include <stdio.h>
#include <string.h>

#include "DevIL/include/il.h"
#include "DevIL/include/ilu.h"
#include "DevIL/include/ilut.h"

#include "Assimp/include/version.h"

#pragma comment( lib, "Glew/libx86/glew32.lib" )

#define MAX_FPS_CAP 100



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
	update_status ret = UPDATE_CONTINUE;

	if (App->input->GetKey(SDL_SCANCODE_0) == KEY_DOWN)
	{
		AddConsoleLog("Shinny Mew2 appeared!");
	}
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Load (F6)"))
			{
				App->Load();
				App->editor->LoadScene();
			}
			if (ImGui::MenuItem("Save (F5)"))
			{
				App->Save();
				App->editor->SaveScene();
			}
			if (ImGui::MenuItem("Quit (Alt + F4)"))
			{
				return UPDATE_STOP;
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("View"))
		{
			if (ImGui::BeginMenu("Hierarchy", false)) // Disabled
			{
				IM_ASSERT(0);
			}
			if (ImGui::MenuItem("Console"))
			{
				showconsole = !showconsole;
			}
			if (ImGui::MenuItem("Configuration"))
			{
				config_menu = !config_menu;
			}
			if (ImGui::BeginMenu("Mesh Parameters", false)) // Disabled
			{
				IM_ASSERT(0);
				//ImGui::EndMenu();
			}
			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("GameObject"))
		{
			if (ImGui::MenuItem("Create Empty"))
			{
				App->editor->GetSelected()->AddChild(new GameObject());
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
	//if (show_mesh_info)
	//{
	//	ShowMeshesInfo();
	//}
	if (about)
	{
		AboutWindow();
	}
	if (hierarchy)
	{
		ShowHierarchy();
	}
	return ret;
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

				ImGui::SliderInt("Max FPS", App->GetMaxFPS(), 0, 100);
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

			if (ImGui::CollapsingHeader("Renderer"))
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

			if (ImGui::CollapsingHeader("Textures"))
			{
				if (App->textures->last_tex.width != 0)
				{
					ImGui::Text("Name:"); ImGui::SameLine(120); ImGui::TextColored(ImVec4(1.f, 0.5f, 0.f, 1.f), "%s", App->textures->last_tex.name.c_str());
					ImGui::Text("Size:"); ImGui::SameLine(120); ImGui::Text("%i x %i", App->textures->last_tex.width, App->textures->last_tex.height);
					ImGui::Separator();
					ImGui::Image((ImTextureID)App->textures->last_tex.texture_id, ImVec2(128, 128));
				}
				else
				{
					ImGui::Text("No textures loaded");
				}
			}

			if (ImGui::CollapsingHeader("Hardware"))
			{
				SDL_version version;
				SDL_GetVersion(&version);
				ImGui::Text("SDL Version:"); ImGui::SameLine(); ImGui::TextColored(ImVec4(0.f, 1.f, 1.f, 1.f), "%i.%i.%i", version.major, version.minor, version.patch);
				ImGui::Text("OpenGL Version:"); ImGui::SameLine(); ImGui::TextColored(ImVec4(0.f, 1.f, 1.f, 1.f), "%s", glGetString(GL_VERSION));
				ImGui::Text("GLSL:"); ImGui::SameLine(); ImGui::TextColored(ImVec4(0.f, 1.f, 1.f, 1.f), "%s", glGetString(GL_SHADING_LANGUAGE_VERSION));
				ImGui::Text("DevIL Version:"); ImGui::SameLine(); ImGui::TextColored(ImVec4(0.f, 1.f, 1.f, 1.f), "%i", IL_VERSION);
				ImGui::Separator();
				ImGui::Text("CPUs:"); ImGui::SameLine(); ImGui::TextColored(ImVec4(0.f, 1.f, 1.f, 1.f), "%i", SDL_GetCPUCount()); ImGui::SameLine(); ImGui::TextColored(ImVec4(0.f, 1.f, 1.f, 1.f), "(Cache: %ikb)", SDL_GetCPUCacheLineSize());
				ImGui::Text("System RAM:"); ImGui::SameLine(); ImGui::TextColored(ImVec4(0.f, 1.f, 1.f, 1.f), "%.2fGb", SDL_GetSystemRAM() / 1024.0f);
				ImGui::Text("Caps:"); ImGui::SameLine(); ImGui::TextColored(ImVec4(0.f, 1.f, 1.f, 1.f), "%s%s%s%s%s%s%s%s%s%s%s", (SDL_Has3DNow()) ? "3DNow, " : "", (SDL_HasAVX()) ? "AVX, " : "", (SDL_HasAVX2()) ? "AVX2, " : "", (SDL_HasAltiVec()) ? "AltiVec, " : "", (SDL_HasMMX()) ? "MMX, " : "", (SDL_HasRDTSC()) ? "RDTSC, " : "", (SDL_HasSSE()) ? "SSE, " : "", (SDL_HasSSE2()) ? "SSE2, " : "", (SDL_HasSSE3()) ? "SSE3, " : "", (SDL_HasSSE41()) ? "SSE41, " : "", (SDL_HasSSE42()) ? "SSE42 " : "");
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
		for (int i = 0; i < consoleLogs.size(); i++)
		{
			std::string s = consoleLogs[i];
			ImGui::Text("%s", consoleLogs[i].c_str());
		}
		ImGui::End();
	}

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
	if (FPSvec.size() < MAX_FPS_CAP)
	{
		FPSvec.push_back(fps);
		MSvec.push_back(ms);
	}
	else
	{
		FPSvec.erase(FPSvec.begin());
		MSvec.erase(MSvec.begin());
		FPSvec.push_back(fps);
		MSvec.push_back(ms);
	}
}

void ModuleImGui::AboutWindow()
{
	if (about)
	{
		ImGui::Begin(("About"), &about);
		
		ImGui::Text("GTi Engine");
		ImGui::Text("A game engine developed by Albert Llopart & Marc Fabian");

		if (ImGui::CollapsingHeader("Libraries used"))
		{
			SDL_version sdl;
			SDL_GetVersion(&sdl);

			if (ImGui::Button("SDL"))
			{
				ShellExecuteA(NULL, "open", "https://www.libsdl.org/index.php", NULL, NULL, SW_SHOWNORMAL);
			}
			ImGui::SameLine(160); ImGui::Text("%i.%i.%i", sdl.major, sdl.minor, sdl.patch);

			if (ImGui::Button("MathGeoLib"))
			{
				ShellExecuteA(NULL, "open", "http://clb.demon.fi/MathGeoLib/nightly/", NULL, NULL, SW_SHOWNORMAL);
			}
			ImGui::SameLine(160); ImGui::Text("1.3");

			if (ImGui::Button("ImGui"))
			{
				ShellExecuteA(NULL, "open", "https://github.com/ocornut/imgui", NULL, NULL, SW_SHOWNORMAL);
			}
			ImGui::SameLine(160); ImGui::Text("%s", ImGui::GetVersion());

			if (ImGui::Button("OpenGL"))
			{
				ShellExecuteA(NULL, "open", "https://www.opengl.org/", NULL, NULL, SW_SHOWNORMAL);
			}
			ImGui::SameLine(160); ImGui::Text("%s", glGetString(GL_VERSION));

			if (ImGui::Button("Glew"))
			{
				ShellExecuteA(NULL, "open", "http://glew.sourceforge.net/", NULL, NULL, SW_SHOWNORMAL);
			}
			ImGui::SameLine(160); ImGui::Text("%s", glewGetString(GLEW_VERSION));

			if (ImGui::Button("DevIL"))
			{
				ShellExecuteA(NULL, "open", "http://openil.sourceforge.net/", NULL, NULL, SW_SHOWNORMAL);
			}
			ImGui::SameLine(160); ImGui::Text("%i", IL_VERSION);

			if (ImGui::Button("Assimp"))
			{
				ShellExecuteA(NULL, "open", "http://www.assimp.org/", NULL, NULL, SW_SHOWNORMAL);
			}
			ImGui::SameLine(160); ImGui::Text("%i.%i.%i", aiGetVersionMajor(), aiGetVersionMinor(), aiGetVersionRevision());

			if (ImGui::Button("Parson"))
			{
				ShellExecuteA(NULL, "open", "https://github.com/kgabis/parson", NULL, NULL, SW_SHOWNORMAL);
			}
			ImGui::SameLine(160); ImGui::Text("No version control");

			if (ImGui::Button("PCG"))
			{
				ShellExecuteA(NULL, "open", "http://www.pcg-random.org/", NULL, NULL, SW_SHOWNORMAL);
			}
			ImGui::SameLine(160); ImGui::Text("0.94");
	
		}
		if (ImGui::CollapsingHeader("License"))
		{
			if (ImGui::MenuItem("License MIT"))
			{
				ShellExecuteA(NULL, "open", "https://github.com/albertllopart/GTi-Engine-Reborn/blob/master/LICENSE", NULL, NULL, SW_SHOWNORMAL);
			}
		}
		ImGui::End();
	}
}

//void ModuleImGui::ShowMeshesInfo()
//{
//	int alignment = 120;
//
//	if (show_mesh_info)
//	{
//		std::list<Mesh*> scene_meshes = App->editor->GetMeshList();
//		ImGui::Begin("Meshes", &show_mesh_info);
//
//		if (App->editor->GetMeshList().size() > 0)
//		{
//			for (std::list<Mesh*>::const_iterator iterator = scene_meshes.begin(), end = scene_meshes.end(); iterator != scene_meshes.end(); ++iterator)
//			{
//				if (ImGui::CollapsingHeader(iterator._Ptr->_Myval->name.c_str()))
//				{
//					ImGui::TextColored(ImVec4(1.f, 1.f, 0.f, 1.f), "Transform"); ImGui::Separator();
//					ImGui::Text("Position:"); ImGui::SameLine(alignment);//120 means horizontal alignment
//					ImGui::TextColored(ImVec4(1.f, 0.f, 0.f, 1.f), "X"); ImGui::SameLine(); ImGui::Text("%.3f", iterator._Ptr->_Myval->pos.x); ImGui::SameLine(alignment + 100);
//					ImGui::TextColored(ImVec4(0.f, 1.f, 0.f, 1.f), "Y"); ImGui::SameLine(); ImGui::Text("%.3f", iterator._Ptr->_Myval->pos.y); ImGui::SameLine(alignment + 200);
//					ImGui::TextColored(ImVec4(0.f, 0.f, 1.f, 1.f), "Z"); ImGui::SameLine(); ImGui::Text("%.3f", iterator._Ptr->_Myval->pos.z);
//																										   
//					ImGui::Text("Scale:"); ImGui::SameLine(alignment);									   
//					ImGui::TextColored(ImVec4(1.f, 0.f, 0.f, 1.f), "X"); ImGui::SameLine(); ImGui::Text("%.3f", iterator._Ptr->_Myval->scale.x); ImGui::SameLine(alignment + 100);
//					ImGui::TextColored(ImVec4(0.f, 1.f, 0.f, 1.f), "Y"); ImGui::SameLine(); ImGui::Text("%.3f", iterator._Ptr->_Myval->scale.y); ImGui::SameLine(alignment + 200);
//					ImGui::TextColored(ImVec4(0.f, 0.f, 1.f, 1.f), "Z"); ImGui::SameLine(); ImGui::Text("%.3f", iterator._Ptr->_Myval->scale.z);
//																										   
//					ImGui::Text("Rotation:"); ImGui::SameLine(alignment);								   
//					ImGui::TextColored(ImVec4(1.f, 0.f, 0.f, 1.f), "X"); ImGui::SameLine(); ImGui::Text("%.3f", iterator._Ptr->_Myval->euler_angles.x); ImGui::SameLine(alignment + 100);
//					ImGui::TextColored(ImVec4(0.f, 1.f, 0.f, 1.f), "Y"); ImGui::SameLine(); ImGui::Text("%.3f", iterator._Ptr->_Myval->euler_angles.y); ImGui::SameLine(alignment + 200);
//					ImGui::TextColored(ImVec4(0.f, 0.f, 1.f, 1.f), "Z"); ImGui::SameLine(); ImGui::Text("%.3f", iterator._Ptr->_Myval->euler_angles.z); ImGui::Separator();
//
//					ImGui::TextColored(ImVec4(1.f, 1.f, 0.f, 1.f), "Geometry"); ImGui::Separator();
//					ImGui::Text("Vertices:"); ImGui::SameLine(alignment); ImGui::Text("%i", iterator._Ptr->_Myval->num_vertex);
//					ImGui::Text("Faces:"); ImGui::SameLine(alignment); ImGui::Text("%i", iterator._Ptr->_Myval->faces); ImGui::Separator();
//
//					ImGui::TextColored(ImVec4(1.f, 1.f, 0.f, 1.f), "Texture"); ImGui::Separator();
//					if (iterator._Ptr->_Myval->tex_name != "none")
//					{
//						ImGui::Text("Name:"); ImGui::SameLine(alignment); ImGui::TextColored(ImVec4(1.f, 0.5f, 0.f, 1.f), "%s", iterator._Ptr->_Myval->tex_name.c_str());
//						ImGui::Text("Size:"); ImGui::SameLine(alignment); ImGui::Text("%i x %i", iterator._Ptr->_Myval->tex_width, iterator._Ptr->_Myval->tex_height);
//					}
//					else
//					{
//						ImGui::Text("No texture loaded");
//					}
//				}
//
//			}
//		}
//		else
//		{
//			ImGui::Text("There are no meshes loaded");
//		}
//	}
//	ImGui::End();
//}

void ModuleImGui::ShowHierarchy()
{
	//if (hierarchy)
	//{
	//	ImGui::Begin(("Hierarchy"), &hierarchy);
	//	if (ImGui::CollapsingHeader("Scenes"))
	//	{
	//		App->editor->ShowRoot();
	//	}
	//	ImGui::End();
	//}
}

void ModuleImGui::Draw()const
{
	ImGui::Render();
}

std::vector<float> ModuleImGui::GetFramerateVec() const
{
	return FPSvec;
}
