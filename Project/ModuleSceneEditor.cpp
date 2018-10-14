#include "Application.h"
#include "ModuleSceneEditor.h"
#include "ImGui/imgui.h"
#include "ModuleTextures.h"

ModuleSceneEditor::ModuleSceneEditor(Application* app, bool startEnabled) : Module(app, startEnabled)
{
	name = "Scene editor";
}
ModuleSceneEditor::~ModuleSceneEditor()
{
	while (!mesh_list.empty())
	{
		delete mesh_list.front();
		mesh_list.pop_front();
	}
}

bool ModuleSceneEditor::Init(JSON_Object* data)
{
	return true;
}

bool ModuleSceneEditor::CleanUp(JSON_Object* data)
{
	return true;
}

bool ModuleSceneEditor::Start()
{
	App->camera->Move(float3(0.0, 10.0, 0.0));
	App->camera->LookAt(float3(0.0, 0.0, 0.0));

	return true;
}

update_status ModuleSceneEditor::PreUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

update_status ModuleSceneEditor::Update(float dt)
{
	return UPDATE_CONTINUE;
}

update_status ModuleSceneEditor::PostUpdate(float dt)
{
	
	return UPDATE_CONTINUE;
}

void ModuleSceneEditor::Draw()
{

	for (std::list<Mesh*>::iterator it = mesh_list.begin(); it != mesh_list.end(); ++it)
	{
		App->renderer3D->Draw((*it));
	}

	pPlane p(0, 0, 0, 100);
	p.color = White;
	p.Render();
}

void ModuleSceneEditor::SetToWireframe(bool wframe)
{
	if (wframe == true)
	{
		for (std::list<pCube*>::iterator it = cubes_list.begin(); it != cubes_list.end(); ++it)
		{
			(*it)->wire = true;
		}
		for (std::list<pCube2*>::iterator it = indcubes_list.begin(); it != indcubes_list.end(); ++it)
		{
			(*it)->wire = true;
		}
	}
	else
	{
		for (std::list<pCube*>::iterator it = cubes_list.begin(); it != cubes_list.end(); ++it)
		{
			(*it)->wire = false;
		}
		for (std::list<pCube2*>::iterator it = indcubes_list.begin(); it != indcubes_list.end(); ++it)
		{
			(*it)->wire = false;
		}
	}
}
void ModuleSceneEditor::AddCube(float3 size, float3 pos)
{
	pCube* cube = new pCube(pos,size);
	cube->size.Set(size.x, size.y, size.z);
	cube->SetPos(pos.x, pos.y, pos.z);
	cubes_list.push_back(cube);
}
void ModuleSceneEditor::AddCube2(float3 size, float3 pos)
{
	pCube2* cube = new pCube2(pos,size);
	cube->size.Set(size.x, size.y, size.z);
	cube->SetPos(pos.x, pos.y, pos.z);
	indcubes_list.push_back(cube);
}

void ModuleSceneEditor::AddMesh(Mesh * model)
{
	if (mesh_list.size() > 0)
	{
		mesh_list.clear();
	}
	mesh_list.push_back(model);
	App->camera->CenterToMesh(GetMeshList().back());
}

void ModuleSceneEditor::LoadTexture2AllMesh(const char * path)
{
	std::string str = path;
	uint position_name = str.find_last_of("\\");
	std::string newPath = str.erase(0, position_name + 1);

	uint text_id = App->textures->ImportImage(path);
	for (std::list<Mesh*>::const_iterator iterator = mesh_list.begin(); iterator != mesh_list.end(); ++iterator)
	{
		iterator._Ptr->_Myval->texture = text_id;
		iterator._Ptr->_Myval->tex_name = newPath;
		iterator._Ptr->_Myval->tex_width = App->textures->last_tex.width;
		iterator._Ptr->_Myval->tex_height = App->textures->last_tex.height;
	}

	App->textures->last_tex.name = newPath;
	App->imgui->AddConsoleLog(("%s", newPath));
	App->imgui->AddConsoleLog("Texture loaded to all meshes");
}

std::list<Mesh*> ModuleSceneEditor::GetMeshList() const
{
	return  mesh_list;
}


