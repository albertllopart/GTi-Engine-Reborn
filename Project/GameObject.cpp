#include "GameObject.h"
#include "Application.h"
#include "ImGui/imgui.h"

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

void GameObject::PostUpdate()
{
	for (int i = 0; i < components.size(); i++)
	{
		components[i]->PostUpdate();
	}
	for (int i = 0; i < childs.size(); i++)
	{
		childs[i]->PostUpdate();
	}
}

void GameObject::OnEditor()
{
	if (ImGui::TreeNodeEx(name.c_str()))
	{
		for (int i = 0; i < components.size(); i++)
		{
			components[i]->OnEditor();
		}
		for (int i = 0; i < childs.size(); i++)
		{
			childs[i]->OnEditor();
		}
		ImGui::TreePop();
	}
}

void GameObject::ShowProperties()
{
	ImGui::SetNextWindowSize(ImVec2(500, 300));
	ImGui::SetNextWindowPos(ImVec2(App->window->width - 10, 25));

	std::string go = name;
	go += " properties";

}

void  GameObject::AddComponent(Component* to_add)
{
	components.push_back(to_add);
	to_add->SetMyGo(this);
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

void GameObject::SetName(const char * name)
{
	this->name = name;
}

void GameObject::AddChild(GameObject* child)
{
	childs.push_back(child);
	child->parent = this;
}
