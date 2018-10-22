#include "Component.h"
#include "Globals.h"
#include "Application.h"

Component::Component(GameObject* my_go, COMPONENT_TYPE type): my_go(my_go), type(type)
{

}

Component::~Component()
{
}

void Component::Enable()
{
	active = true;
}

void Component::Update()
{
}

void Component::Disable()
{
	active = false;
}

bool Component::IsActive() const
{
	return active;
}


