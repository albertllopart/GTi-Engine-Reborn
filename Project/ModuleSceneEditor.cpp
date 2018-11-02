#include "Application.h"
#include "ModuleSceneEditor.h"
#include "ImGui/imgui.h"
#include "ModuleTextures.h"
#include "GameObject.h"
#include "ComponentTransform.h"

#include "parson/parson.h"
#include "JSONConfig.h"

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
	quadtree.Boundaries(AABB(float3(-quad_size, -quad_size, -quad_size), float3(quad_size, quad_size, quad_size)));//boundariese of 100 for testing


	return true;
}

update_status ModuleSceneEditor::PreUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

update_status ModuleSceneEditor::Update(float dt)
{
	if (quadtree_draw)
	{
		quadtree.root->DrawQuadtree();
	}
	
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


GameObject * ModuleSceneEditor::GetRoot()
{
	return root;
}

GameObject * ModuleSceneEditor::CreateNewGameObject(const char * path)
{
	GameObject* ret = App->import->LoadGameObject(path);
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

void ModuleSceneEditor::AddToScene(GameObject * to_add)
{
	to_add->SetParent(root);	//CHECK IF THIS IS POSSIBLE (ROOT == SCENE)??
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
	App->imgui->AddConsoleLog("Texture loaded to geometry");

	return mat;
}

GameObject* ModuleSceneEditor::CreateEmptyGameObject(GameObject* parent)
{
	GameObject* go = new GameObject(parent);
	ComponentTransform* transform = new ComponentTransform();
	go->AddComponent(transform);
	return go;
}

void ModuleSceneEditor::ShowRoot()
{
	root->OnEditor();
}

GameObject* ModuleSceneEditor::FindGObyUID(uint uid, GameObject* to_find)
{
	GameObject* ret = nullptr;

	if (to_find->uid == uid)
	{
		return to_find;
	}

	for (int i = 0; i < to_find->childs.size() && ret == nullptr; i++)
	{
		ret = FindGObyUID(uid, to_find->childs[i]);
	}

	return ret;
}

std::vector<GameObject*>* ModuleSceneEditor::GetAllGO()
{
	scene_go.clear();
	root->GetSceneGameObjects(scene_go);
	return &scene_go;
}

bool ModuleSceneEditor::SaveScene(const char* name) const
{
	std::string log = "Saving Scene: ";
	log += name;
	App->imgui->AddConsoleLog(log);

	JSON_Value *root_value = json_value_init_object();
	JSON_Object *root_object = json_value_get_object(root_value);

	json_object_set_value(root_object, "Scene", json_value_init_object());
	root_object = json_object_get_object(root_object, "Scene");
	
	if (root != NULL)
	{
		json_object_set_string(root_object, "Name", root->name.c_str());
		json_object_set_number(root_object, "UID", root->uid);
		
		JSON_Value* array = json_value_init_array();
		json_object_set_value(root_object, "Game Objects", array);

		root->OnSave(array);
	}
	
	std::string to_save = "Library/Scenes/";
	to_save += name;
	to_save += ".GTIscene";

	json_serialize_to_file(root_value, to_save.c_str());

	json_value_free(root_value);

	return true;
}

bool ModuleSceneEditor::LoadScene(const char* name)
{
	JSONConfig config;

	if (!config.ParseFile("scene.testing"))
		return false;

	config = config.SetFocus("Scene");
	root->uid = config.GetInt("UID");

	uint size = config.GetArraySize("Game Objects");

	for (uint i = 0; i < size; i++)
	{
		JSONConfig item_config = config.SetFocusArray("Game Objects", i);
		GameObject* item = new GameObject();

		item->OnLoad(item_config);
		item->SetParent(FindGObyUID(item_config.GetInt("Parent"), root));
	}
	
	return true;
}



