#include "Component.h"
#include "Globals.h"
#include "Application.h"

Component::Component(COMPONENT_TYPE type): type(type)
{

}

Component::~Component()
{
}

void Component::Update()
{

}

void Component::PostUpdate()
{

}

void Component::Enable()
{
	active = true;
}

void Component::Disable()
{
	active = false;
}

bool Component::IsActive() const
{
	return active;
}

void Component::OnEditor()
{

}

void Component::SetMyGo(GameObject* my_go)
{
	this->my_go = my_go;
}

COMPONENT_TYPE Component::GetType() const
{
	return type;
}


