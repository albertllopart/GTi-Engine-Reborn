#include "Application.h"
#include "ModuleSceneEditor.h"



ModuleSceneEditor::ModuleSceneEditor(Application* app, bool startEnabled) : Module(app, startEnabled)
{
	name = "Scene editor";
}
ModuleSceneEditor::~ModuleSceneEditor()
{
	while (!cubes_list.empty())
	{
		delete cubes_list.front();
		cubes_list.pop_front();
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
	App->camera->Move(float3(0, 1, 0));
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
	}
	else
	{
		for (std::list<pCube*>::iterator it = cubes_list.begin(); it != cubes_list.end(); ++it)
		{
			(*it)->wire = false;
		}
	}
}
void ModuleSceneEditor::AddCube(float3 size, float3 pos)
{
	pCube* cube = new pCube;
	cube->size.Set(size.x, size.y, size.z);
	cube->SetPos(pos.x, pos.y, pos.z);
	cubes_list.push_back(cube);
}