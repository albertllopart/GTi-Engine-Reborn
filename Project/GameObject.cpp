#include "GameObject.h"
#include "Globals.h"
#include "Application.h"


GameObject::GameObject()
{
}

GameObject::~GameObject()
{
}

void GameObject::Update()
{
}

Component * GameObject::CreateComponent(COMPONENT_TYPE type)
{
	return nullptr;
}
