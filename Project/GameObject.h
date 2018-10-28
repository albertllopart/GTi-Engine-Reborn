#ifndef __GAME_OBJECT_H__
#define __GAME_OBJECT_H__
#include "Component.h"
#include "Globals.h"
#include <string>
#include <vector>

class Component;
enum COMPONENT_TYPE;

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
	GameObject* GetParent() const;
	void SetParent(GameObject* new_parent);
	Component* FindComponent(COMPONENT_TYPE type) const;

	std::vector<GameObject*> GetChilds() const;
	std::vector<Component*> GetComponents() const;
	void SetName(const char* name);
	void AddChild(GameObject * child);
	void SetToDelete();

private:
	uint uid = 0;
	GameObject* parent = nullptr;


public:

	std::string name;
	std::vector<Component*> components;
	std::vector<GameObject*> childs;

	bool is_static = false; 
	bool active = true;
	bool want_delete = false;
};

#endif // __GAME_OBJECT_H__