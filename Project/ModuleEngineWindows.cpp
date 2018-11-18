#include "ModuleEngineWindows.h"
#include "E_Windows.h"
#include "E_Hierarchy.h"
#include "E_Inspector.h"
#include "E_Folder.h"
#include "ImGui/imgui.h"

#define UPDATE_FOLDER_TIME 2.0f

ModuleEngineWindows::ModuleEngineWindows(Application * app, bool start_enabled):Module(app, start_enabled)
{
	name = "Engine Windows";
	e_hierarchy = new E_Hierarchy(this);
	e_windows.push_back(e_hierarchy);
	e_inspector = new E_Inspector(this);
	e_windows.push_back(e_inspector);
	e_folder = new E_Folder(this);
	e_windows.push_back(e_folder);
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
	time_update += dt;
	e_folder->DeleteFolders();

	if (want_to_update || time_update > UPDATE_FOLDER_TIME)
	{
		e_folder->UpdateFiles();
		time_update = 0.0f;
		want_to_update = false;
	}
	if (want_to_save)
	{
		App->editor->SaveScene(path_to_save.c_str());
		want_to_update = true;
		want_to_save = false;
	}

	if (want_to_load)
	{
		switch (next_load)
		{
		case LOAD_NONE:
			break;
		case LOAD_MESH:
			App->editor->LoadGO(path_to_load.c_str());
			break;
		case LOAD_TEXTURE:
			break;
		case LOAD_SCENE:
			App->editor->LoadScene(path_to_load.c_str());
			break;
		default:
			break;
		}
		want_to_load = false;
		next_load = LOAD_NONE;
	}

	return UPDATE_CONTINUE;
}

update_status ModuleEngineWindows::Update(float dt)
{
	//if (load_window)
	//{
	//	LoadWindow();
	//}
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


void ModuleEngineWindows::WantToLoad(const char * path)
{
	want_to_load = true;
	next_load = DetermineFileFromPath(path);
	path_to_load = path;

}

//void ModuleEngineWindows::LoadWindow()
//{
//	char* buffer = nullptr;
//
//	if (e_folder->LoadWindow(&buffer))
//	{
//		load_window = false;
//		if (buffer != nullptr)
//		{
//			WantToLoad(buffer);
//			RELEASE_ARRAY(buffer);
//		}
//	}
//
//}

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

LoadFile ModuleEngineWindows::DetermineFileFromPath(const char * path)
{

	std::string extension_check = path;
	std::size_t found = extension_check.find_last_of('.');
	std::string extension = extension_check.substr(found + 1);
	if (_stricmp(extension.c_str(), "png") == 0
		|| _stricmp(extension.c_str(), "jpg") == 0
		|| _stricmp(extension.c_str(), "dds") == 0
		|| _stricmp(extension.c_str(), "tga") == 0)
	{
		return LOAD_TEXTURE;
	}
	else if (_stricmp(extension.c_str(), "obj") == 0
		|| _stricmp(extension.c_str(), "fbx") == 0)
	{
		return LOAD_MESH;

	}
	else if (_stricmp(extension.c_str(), "json") == 0)
	{
		return LOAD_SCENE;
	}
	else
	{
		return LOAD_NONE;
	}
}