#include "GameObject.h"
#include "Application.h"
#include "ImGui/imgui.h"
#include "ComponentTransform.h"
#include "ComponentCamera.h"
#include "MathGeoLib/Math/float4x4.h"

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
	//CHECK how to delete go!!
	//if (want_delete)
	//{
	//	for (int i = 0; i < components.size(); i++)
	//	{
	//		delete components[i];
	//		components[i] = nullptr;
	//	}
	//	for (int i = 0; i < childs.size(); i++)
	//	{
	//		delete childs[i];
	//		childs[i] = nullptr;
	//	}
	//	childs.clear();
	//	delete this;
	//}
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
			App->editor->GetSelected()->AddChild(new GameObject());
			ImGui::CloseCurrentPopup();
		}
		ImGui::EndPopup();
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

void GameObject::ShowInspectorWindow()
{
	if (this != App->editor->GetRoot())
	{

		if (ImGui::BeginChild(ImGui::GetID("Inspector"), ImVec2(ImGui::GetWindowWidth(), 500)))
		{
			ImGui::Checkbox("Active", &active);
			ImGui::SameLine();
			ImGui::Text(name.c_str());	//TODO EDIT G.O. NAME
			ImGui::Checkbox("", &is_static);
			ImGui::SameLine();
			ImGui::TextColored(ImVec4(0.25f, 1.00f, 1.00f, 1.00f), "Static");
		}
		ImGui::Checkbox("BoundingBox", &show_bbox);
		for (uint i = 0; i < components.size(); i++)
		{
			ImGui::Separator();
			Component* item = components[i];
			item->ShowInspectorWindow();

		}
		ImGui::EndChild();
	}
}

void GameObject::AddComponent(Component* to_add)
{
	components.push_back(to_add);
	to_add->SetMyGo(this);
}

void GameObject::AddComponent(COMPONENT_TYPE component)
{
	if (component != COMPONENT_NONE)
	{
		Component* to_add;
		switch (component)
		{
		case COMPONENT_MESH:
			to_add = new ComponentMesh();
			components.push_back(to_add);
			to_add->SetMyGo(this);
			break;

		case COMPONENT_TRANSFORM:
			to_add = new ComponentTransform();
			components.push_back(to_add);
			to_add->SetMyGo(this);
			break;
		case COMPONENT_MATERIAL:
			to_add = new ComponentMaterial();
			components.push_back(to_add);
			to_add->SetMyGo(this);
			break;
		case COMPONENT_CAMERA:
			to_add = new ComponentCamera();
			components.push_back(to_add);
			to_add->SetMyGo(this);
			break;
		default:
			break;

		}
	}
}

void GameObject::UpdateBBox()
{
 //TODO
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

float4x4 GameObject::GetTransMatrix()const
{
	if (my_transform != nullptr)
	{
		return my_transform->GetGlobalMatrix();
	}
	return float4x4::zero;
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

void GameObject::RefreshBBox() const
{
	//TODO
	//WHEN WE TRSNSFROM THE G.O. WE HAVE TO UPDATE THE BBOX
}

void GameObject::UpdateMatrix() const
{
	my_transform->UpdateMatrix();
}
