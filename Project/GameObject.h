#ifndef __GAME_OBJECT_H__
#define __GAME_OBJECT_H__
#include "Component.h"

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
	Component* CreateComponent(COMPONENT_TYPE type);
	GameObject* GetParent() const;

	std::vector<GameObject*> GetChilds() const;
	std::vector<Component*> GetComponents() const;
	void SetName(const char* name);

private:

	GameObject* parent = nullptr;

	std::string name;
	std::vector<Component*> components;
	std::vector<GameObject*> childs;

	bool is_static = false; 
};

#endif // __GAME_OBJECT_H__