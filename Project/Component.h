#ifndef __COMPONENT__
#define __COMPONENT__

#include "Module.h"
#include "Globals.h"
#include "GameObject.h"

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
	Component(GameObject* my_go, COMPONENT_TYPE type);
	~Component();

	void SetActive(bool activate);
	bool IsActive() const;
	const GameObject* GetParent()const;
private:

	COMPONENT_TYPE type = COMPONENT_NONE;
	bool active = true;
	GameObject* my_go = nullptr;
};

#endif // __COMPONENT__