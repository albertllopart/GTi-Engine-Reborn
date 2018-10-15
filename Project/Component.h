#ifndef __COMPONENT__
#define __COMPONENT__

#include "Module.h"
#include "Globals.h"

enum COMPONENT_TYPE
{
	COMPONENT_NONE,
	COMPONENT_MESH,
	COMPONENT_MATERIAL,
	COMPONENT_TRANSFORM
};

class Component
{
public:
	Component();
	~Component();

	void Update();

	COMPONENT_TYPE type = COMPONENT_NONE;
	bool active = true;
	GameObject* my_go = nullptr;
};

#endif // __COMPONENT__