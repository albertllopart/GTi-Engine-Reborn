#ifndef __GAME_OBJECT_H__
#define __GAME_OBJECT_H__
#include "Component.h"
#include "MathGeoLib/Math/float4x4.h"
#include "Globals.h"
#include <string>
#include <vector>

class Component;
class ComponentTransform;
class ComponentMesh;
class JSONConfig;
enum COMPONENT_TYPE;

struct json_object_t;
typedef struct json_object_t JSON_Object;

struct json_value_t;
typedef struct json_value_t  JSON_Value;

struct json_array_t;
typedef struct json_array_t JSON_Array;

class GameObject
{

public:
	GameObject();
	GameObject(GameObject* parent);
	~GameObject();

	void PreUpdate();
	void Update();
	void PostUpdate();
	void OnEditor();
	void ShowInspectorWindow();

	void AddComponent(Component * to_add);
	Component* AddComponent(COMPONENT_TYPE component);
	void UpdateBBox();
	bool RemoveComponent(COMPONENT_TYPE type, int position);
	void DrawBBox(ComponentMesh* c_mesh);
	void DrawBBox(AABB * bboc);

	GameObject* GetParent() const;
	void SetParent(GameObject* new_parent);
	Component* FindComponent(COMPONENT_TYPE type) const;
	float4x4 GetTransMatrix()const;

	std::vector<GameObject*> GetChilds() const;
	std::vector<Component*> GetComponents() const;
	void SetName(const char* name);
	void AddChild(GameObject * child);
	void SetToDelete();
	//void RefreshBBox()const;
	void UpdateMatrix()const;
	void GetSceneGameObjects(std::vector<GameObject*>& SceneGameObjects) const; //method for root, to get all gameobjects in scene
	bool RemoveGameObject(GameObject * to_remove);
	void CleanRemove();
	AABB* GetBBox();
	//save
	bool OnSave(JSON_Value* array) const;
	bool OnLoad(const JSONConfig data);
	

private:
	
	GameObject* parent = nullptr;
	ComponentTransform* my_transform = nullptr;

public:

	uint uid = 0;
	std::string name;
	std::vector<Component*> components;
	std::vector<GameObject*> childs;
	AABB* bbox = nullptr; //only if we have a mesh

	bool is_static = false; 
	bool active = true;
	bool want_delete = false;
	bool show_bbox = false;
	bool visible = true;
};

#endif // __GAME_OBJECT_H__