#ifndef __ModuleSceneEditor_H__
#define __ModuleSceneEditor_H__
#include "Module.h"
#include "Globals.h"
#include "Primitive.h"
#include "Quadtree.h"

#include "ImGuizmo/ImGuizmo.h"

#include <list>

class GameObject;

struct json_object_t;
typedef struct json_object_t JSON_Object;

struct json_value_t;
typedef struct json_value_t  JSON_Value;

struct json_array_t;
typedef struct json_array_t JSON_Array;

class ModuleSceneEditor : public Module
{
public:

	ModuleSceneEditor(Application* app, bool startEnabled = true);
	~ModuleSceneEditor();

	bool Init(JSON_Object* data = nullptr);
	bool CleanUp(JSON_Object* data = nullptr);
	bool Start();
	update_status PreUpdate(float dt);
	update_status Update(float dt);
	update_status PostUpdate(float dt);

	void Draw();
	ComponentMaterial* LoadComponentMaterial(const char* path);
	GameObject* CreateEmptyGameObject(GameObject* parent);
	void ShowRoot();
	GameObject* GetRoot();
	GameObject* CreateNewGameObject(const char* path);
	void SetSelected(GameObject* to_select);
	GameObject* GetSelected()const;
	void AddToScene(GameObject* to_add);
	GameObject* FindGObyUID(uint uid, GameObject* to_find);
	std::vector<GameObject*> GetAllGO();
	std::vector<GameObject*> GetStaticGO();
	void RemoveGameObjectFromScene();
	void DeleteScene();
	//Quadtree
	void GenQuadtree();
	void FillQuadtree();
	void LoadGO(const char * path);
	void LoadMat(const char * path);
	//save and load
	bool SaveScene(const char* name) const;
	bool LoadScene(const char* name);
	void WantToLoadScene(const char* name);
	void WantToLoadSceneNoCamera();
	void DeleteMainCamera();

public:

	Quadtree quadtree;
	bool quadtree_draw = false;

	ImGuizmo::OPERATION currentOperation = ImGuizmo::OPERATION::TRANSLATE;

	bool want_to_load_scene = false;
	bool want_to_load_scene_no_camera = false;
	std::string next_scene;
	ComponentCamera* main_camera = nullptr;
private:

	GameObject* selected_go = nullptr;
	GameObject* root = nullptr;

	std::vector<GameObject*>scene_go;
	std::vector<GameObject*>static_scene_go;
	
};

#endif 