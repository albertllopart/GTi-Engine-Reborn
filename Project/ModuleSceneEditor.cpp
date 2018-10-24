#include "Application.h"
#include "ModuleSceneEditor.h"
#include "ImGui/imgui.h"
#include "ModuleTextures.h"
#include "GameObject.h"
#include "ComponentTransform.h"

ModuleSceneEditor::ModuleSceneEditor(Application* app, bool startEnabled) : Module(app, startEnabled)
{
	name = "Scene editor";
}
ModuleSceneEditor::~ModuleSceneEditor()
{

}

bool ModuleSceneEditor::Init(JSON_Object* data)
{
	return true;
}

bool ModuleSceneEditor::CleanUp(JSON_Object* data)
{
	delete root;
	root = nullptr;
	return true;
}

bool ModuleSceneEditor::Start()
{
	App->camera->Move(float3(0.0, 10.0, 0.0));
	App->camera->LookAt(float3(0.0, 0.0, 0.0));

	root = new GameObject();
	root->SetName("Scene 1");

	return true;
}

update_status ModuleSceneEditor::PreUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

update_status ModuleSceneEditor::Update(float dt)
{
	root->Update();
	return UPDATE_CONTINUE;
}

update_status ModuleSceneEditor::PostUpdate(float dt)
{
	root->PostUpdate();
	return UPDATE_CONTINUE;
}

void ModuleSceneEditor::Draw()
{

	pPlane p(0, 0, 0, 100);
	p.color = White;
	p.Render();
}

//void ModuleSceneEditor::AddCube(float3 size, float3 pos)
//{
//	pCube* cube = new pCube(pos,size);
//	cube->size.Set(size.x, size.y, size.z);
//	cube->SetPos(pos.x, pos.y, pos.z);
//	cubes_list.push_back(cube);
//}
//void ModuleSceneEditor::AddCube2(float3 size, float3 pos)
//{
//	pCube2* cube = new pCube2(pos,size);
//	cube->size.Set(size.x, size.y, size.z);
//	cube->SetPos(pos.x, pos.y, pos.z);
//	indcubes_list.push_back(cube);
//}
//
//void ModuleSceneEditor::AddMesh(Mesh * model)
//{
//	mesh_list.push_back(model);
//	App->camera->CenterToMesh(GetMeshList().back());
//}

GameObject * ModuleSceneEditor::GetRoot()
{
	return root;
}

GameObject * ModuleSceneEditor::CreateNewGameObject(const char * path)
{
	GameObject* ret = App->import->LoadGameObject(path); //needs rework
	root->AddChild(ret);

	return ret;
}

void ModuleSceneEditor::SetSelected(GameObject * to_select)
{
	selected_go = to_select;
}

GameObject * ModuleSceneEditor::GetSelected() const
{
	return selected_go;
}

ComponentMaterial* ModuleSceneEditor::LoadComponentMaterial(const char * path)
{
	ComponentMaterial* mat = new ComponentMaterial;
	std::string str = path;
	uint position_name = str.find_last_of("\\");
	std::string newPath = str.erase(0, position_name + 1);

	mat->SetID(App->textures->ImportImage(path));
	mat->SetTextureName(newPath.c_str());
	mat->SetTextureSize(App->textures->last_tex.width, App->textures->last_tex.height);

	App->imgui->AddConsoleLog(("%s", newPath));
	App->imgui->AddConsoleLog("Texture loaded to mesh");

	return mat;
}

void ModuleSceneEditor::CreateEmptyGameObject()
{
	GameObject* go = new GameObject();
	ComponentTransform* transform = new ComponentTransform();
	go->AddComponent(transform);
	
}

void ModuleSceneEditor::ShowRoot()
{
	root->OnEditor();
}



