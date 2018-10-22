#ifndef __GAME_OBJECT__
#define __GAME_OBJECT__

#include "Module.h"
#include "Globals.h"
#include "Component.h"


class GameObject
{

public:
	GameObject();
	~GameObject();

	void Update();
	Component* CreateComponent(COMPONENT_TYPE type);

	std::string name;
	std::vector<Component*> components;
	GameObject* parent = nullptr;
	std::vector<Component*> childs;

	
};

#endif // __GAME_OBJECT__