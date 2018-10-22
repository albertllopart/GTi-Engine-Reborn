#ifndef __COMPONENT__
#define __COMPONENT__
#include "GameObject.h"

#include <string>

class GameObject;

enum COMPONENT_TYPE
{
	COMPONENT_NONE,
	COMPONENT_MESH,
	COMPONENT_MATERIAL,
	COMPONENT_TRANSFORM,
	COMPONENT_CAMERA
};

class Component
{
public:
	Component(GameObject* my_go, COMPONENT_TYPE type);
	virtual ~Component();

	virtual void Enable();
	virtual void Update();
	virtual void Disable();

	bool IsActive() const;

protected:

	std::string name;
	COMPONENT_TYPE type = COMPONENT_NONE;
	bool active = true;
	GameObject* my_go = nullptr;
};

#endif // __COMPONENT__