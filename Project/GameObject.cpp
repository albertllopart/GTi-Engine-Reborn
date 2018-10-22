#include "GameObject.h"
#include "Application.h"


GameObject::GameObject()
{
	name = "GameObject";
}

GameObject::~GameObject()
{
	while (!childs.empty())
	{
		delete childs.back();
		childs.pop_back();
	}
	childs.clear();
	while (!components.empty())
	{
		delete components.back();
		components.pop_back();
	}
	components.clear();
}

void GameObject::Update()
{
	for (int i = 0; i < components.size(); i++)
	{
		components[i]->Update();
	}
	for (int i = 0; i < childs.size(); i++)
	{
		childs[i]->Update();
	}
}

Component * GameObject::CreateComponent(COMPONENT_TYPE type)
{
	return nullptr;
}

GameObject * GameObject::GetParent() const
{
	return parent;
}

std::vector<GameObject*> GameObject::GetChilds() const
{
	return childs;
}

std::vector<Component*> GameObject::GetComponents() const
{
	return components;
}
