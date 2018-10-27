#include "GameObject.h"
#include "Application.h"
#include "ImGui/imgui.h"

GameObject::GameObject()
{
	name = "GameObject";
}

GameObject::GameObject(GameObject * parent)
{
	name = "GameObject";
	this->parent = parent;
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
	bool node_open = false;

	ImGuiWindowFlags tree_flags = 0;
	if (childs.empty())
		tree_flags |= ImGuiTreeNodeFlags_Leaf;

	if (App->editor->GetSelected() == this)
	{
		tree_flags |= ImGuiTreeNodeFlags_Selected;
	}

	if (!active)
	{
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 1.0f, 1.0f, 0.5f));
	} //if not active, set  color to grey
	
	char item_id[80];
	sprintf_s(item_id, 80, "%s##%i", name.c_str(), uid);
	node_open = ImGui::TreeNodeEx(item_id, tree_flags);

	if (!active)
	{
		ImGui::PopStyleColor();
	}//reset color if was set to grey

	//Set item selected->InspectorUpdate
	if (ImGui::IsItemClicked())
	{
		App->editor->SetSelected(this); //setting item clicked to selected
	}
		
	ImGui::PushID(item_id); //ImGui unique identifier 
	if (ImGui::BeginPopupContextItem("GameObject_options"))
	{
		if (parent != nullptr)
		{
			if (ImGui::Button("Delete Game Object"))
			{
				SetToDelete();
				ImGui::CloseCurrentPopup();
			}
		}
		if (ImGui::Button("Create Game Object"))
		{
			App->editor->CreateEmptyGameObject(this);
			ImGui::CloseCurrentPopup();
		}
	}

	ImGui::PopID();
	if (node_open)
	{
		for (uint i = 0; i < childs.size(); i++)
		{
			GameObject* item = childs[i];
			item->OnEditor();
		}
		ImGui::TreePop();
	}
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

void GameObject::SetParent(GameObject* new_parent)
{
	parent = new_parent;
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

void GameObject::SetToDelete()
{
	want_delete = true;
}
