#include "Application.h"
#include "ModuleSceneEditor.h"



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
	for (std::list<pCube*>::iterator it = cubes_list.begin(); it != cubes_list.end(); ++it)
	{
		(*it)->Render();
	}
	for (std::list<pCube2*>::iterator it = indcubes_list.begin(); it != indcubes_list.end(); ++it)
	{
		(*it)->Render();
	}
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
	mesh_list.push_back(model);
}

void ModuleSceneEditor::LoadTexture2LastMesh(const char * path)
{
	uint text_id = App->textures->ImportImage(path);
	mesh_list.back()->texture = text_id;
}
