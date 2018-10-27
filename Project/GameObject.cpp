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
	App->editor->SetSelected(this);

	ImGuiWindowFlags window_flags = 0;
	window_flags |= ImGuiWindowFlags_ShowBorders;
	window_flags |= ImGuiWindowFlags_NoResize;
	window_flags |= ImGuiWindowFlags_NoCollapse;
	window_flags |= ImGuiWindowFlags_HorizontalScrollbar;
	ImGui::SetNextWindowPos(ImVec2(0, 20), ImGuiCond_Always);
	ImGui::SetNextWindowSize(ImVec2(350, SDL_GetWindowSurface(App->window->window)->h - 250), ImGuiCond_Always);

	ImGui::Begin("Hierarchy", &App->imgui->hierarchy, window_flags);

	if (ImGui::TreeNode(name.c_str()))
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

	ImGui::End();
}

void GameObject::ShowInspectorWindow() //NOT SHOWING NOW
{
	ImGui::PushStyleColor(ImGuiCol_ChildWindowBg, ImVec4(0.f, 0.f, 0.f, 1.00f));

	if (ImGui::BeginChild(ImGui::GetID("Inspector"), ImVec2(ImGui::GetWindowWidth(), 150)))
	{
		/* ENABLE-DISABLE CHECKBOX*/
		if (ImGui::Checkbox("test",&active))
		{
			active = !active;
		}
		/* NAME OF THE GAMEOBJECT */
		ImGui::SameLine();
		ImGui::Text(name.c_str());//TODO EDIT G.O. NAME
		ImGui::Checkbox("#2",&is_static);
		ImGui::SameLine();
		ImGui::PopStyleVar();
		ImGui::TextColored(ImVec4(0.25f, 1.00f, 0.00f, 1.00f), "Static");
		ImGui::PopStyleVar();
	}
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

Component * GameObject::FindComponent(COMPONENT_TYPE type) const
{
	for (int i = 0; i < components.size(); i++)
	{
		if (components[i]->GetType() == type)
		{
			return components[i];
		}
	}

	return nullptr;;
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
