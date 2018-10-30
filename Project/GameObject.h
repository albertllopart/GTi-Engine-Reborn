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
enum COMPONENT_TYPE;

struct json_object_t;
typedef struct json_object_t JSON_Object;

struct json_value_t;
typedef struct json_value_t  JSON_Value;

class GameObject
{

public:
	GameObject();
	GameObject(GameObject* parent);
	~GameObject();

	void Update();
	void PostUpdate();
	void OnEditor();
	void ShowInspectorWindow();

	void AddComponent(Component * to_add);
	void AddComponent(COMPONENT_TYPE component);
	void UpdateBBox();
	void DrawBBox(ComponentMesh* c_mesh);

	GameObject* GetParent() const;
	void SetParent(GameObject* new_parent);
	Component* FindComponent(COMPONENT_TYPE type) const;
	float4x4 GetTransMatrix()const;

	std::vector<GameObject*> GetChilds() const;
	std::vector<Component*> GetComponents() const;
	void SetName(const char* name);
	void AddChild(GameObject * child);
	void SetToDelete();
	void RefreshBBox()const;
	void UpdateMatrix()const;

	//save
	bool OnSave(JSON_Value* value, JSON_Object* node);
	

private:
	uint uid = 0;
	GameObject* parent = nullptr;
	ComponentTransform* my_transform = nullptr;

public:

	std::string name;
	std::vector<Component*> components;
	std::vector<GameObject*> childs;


	bool is_static = false; 
	bool active = true;
	bool want_delete = false;
	bool show_bbox = false;
};

#endif // __GAME_OBJECT_H__