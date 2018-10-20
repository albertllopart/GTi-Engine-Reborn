#include "Component.h"
#include "Globals.h"
#include "Application.h"

Component::Component(GameObject* my_go, COMPONENT_TYPE type)
{
}

Component::~Component()
{
}

void Component::SetActive(bool activate)
{
	active = activate;
}

bool Component::IsActive() const
{
	return active;
}

const GameObject * Component::GetParent() const
{
	if (my_go != nullptr)
	{
		return my_go;
	}
	else
	{
		App->imgui->AddConsoleLog("Couldn't return parent!!");
	}
}

