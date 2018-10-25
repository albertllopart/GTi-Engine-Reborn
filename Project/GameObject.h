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
	~GameObject();

	void Update();
	void PostUpdate();
	void OnEditor();
	void ShowInspectorWindow();
	void ShowProperties();

	void AddComponent(Component * to_add);
	GameObject* GetParent() const;
	Component* FindComponent(COMPONENT_TYPE type) const;

	std::vector<GameObject*> GetChilds() const;
	std::vector<Component*> GetComponents() const;
	void SetName(const char* name);
	void AddChild(GameObject * child);

private:
	uint uid = 0;
	GameObject* parent = nullptr;
	
public:

	std::string name;
	std::vector<Component*> components;
	std::vector<GameObject*> childs;

	bool is_static = false; 
	bool active = true;
};

#endif // __GAME_OBJECT_H__