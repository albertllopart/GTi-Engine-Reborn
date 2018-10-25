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
	Component(COMPONENT_TYPE type);
	virtual ~Component();

	virtual void Update();
	virtual void PostUpdate();

	virtual void Enable();
	virtual void Disable();
	virtual void OnEditor();

	GameObject* GetMyGo()const;
	bool IsActive() const;
	void SetMyGo(GameObject * my_go);
	void ShowInspectorWindow();
	COMPONENT_TYPE GetType() const;

protected:

	std::string name;
	COMPONENT_TYPE type = COMPONENT_NONE;
	bool active = true;
	GameObject* my_go = nullptr;
};

#endif // __COMPONENT__