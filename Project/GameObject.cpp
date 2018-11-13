#include "GameObject.h"
#include "Application.h"
#include "ImGui/imgui.h"
#include "ComponentTransform.h"
#include "ComponentCamera.h"
#include "MathGeoLib/Math/float4x4.h"
#include "ComponentMesh.h"
#include "Glew/include/glew.h"
#include "JSONConfig.h"
#include "MathGeoLib/Geometry/OBB.h"
#include "ComponentMesh.h"

GameObject::GameObject()
{
	uid = App->rng->Random32();
	name = "GameObject";
}

GameObject::GameObject(GameObject * parent)
{
	uid = App->rng->Random32();
	name = "GameObject";
	this->parent = parent;
}

GameObject::~GameObject()
{

}

void GameObject::PreUpdate()
{
	for (int i = 0; i < childs.size(); i++)
	{
		childs[i]->PreUpdate();
	}

	for (uint i = 0; i < components.size(); i++)
	{
		Component* item = components[i];
		if (item->GetDelete())
		{
			if (RemoveComponent(item->GetType(), i))
			{
				LOG("Component Removed");
				break;
			}
			else
			{
				LOG("Feiled to remove component!");
			}
		}
	}

	for (uint i = 0; i < childs.size(); i++)
	{
		GameObject* item = childs[i];
		if (item->want_delete)
		{
			if (RemoveGameObject(item))
			{
				LOG("Remove GameObject Complete");
			}
			else
			{
				item->want_delete = false;
				LOG("Remove GameObject Fail");
			}
		}
	}
}

void GameObject::Update()
{
	if (visible)
	{
		for (int i = 0; i < components.size(); i++)
		{
			components[i]->Update();
		}
		for (int i = 0; i < childs.size(); i++)
		{
			childs[i]->Update();
		}
		if (show_bbox)
		{
			DrawBBox(bbox);
		}
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

	
	if (ImGui::IsItemClicked())
	{
		App->editor->SetSelected(this); 
	}
		
	ImGui::PushID(item_id); //ImGui unique identifier 
	if (ImGui::BeginPopupContextItem("GameObject_options"))
	{
		if (parent != nullptr && this!= App->editor->GetRoot())
		{
			if (ImGui::Button("Delete Game Object"))
			{
				SetToDelete();
				
				ImGui::CloseCurrentPopup();
			}
		}
		if (ImGui::Button("Create Game Object"))
		{
			GameObject* bug = new GameObject();
			if (App->editor->GetSelected() == nullptr)
			{
				App->editor->GetRoot()->AddChild(bug);
			}
			else 
			{
				App->editor->GetSelected()->AddChild(bug);
			}
			
			bug->AddComponent(COMPONENT_TRANSFORM);
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
			if (this != nullptr)
			{
				char namedit[50];
				strcpy_s(namedit, 50, name.c_str());
				if (ImGui::InputText("##GOname", namedit, 50, ImGuiInputTextFlags_AutoSelectAll | ImGuiInputTextFlags_EnterReturnsTrue))
				{
					name = std::string(namedit).c_str();
				}
			}
			//ImGui::Text(name.c_str());	
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
	if (to_add != nullptr)
	{
		components.push_back(to_add);
		to_add->SetMyGo(this);
	}
	else
	{
		LOG("Failed to add Component!");
	}

}

Component* GameObject::AddComponent(COMPONENT_TYPE component)
{
	Component* to_add;

	if (component != COMPONENT_NONE)
	{
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
			my_transform = (ComponentTransform*)to_add;
			my_transform->UpdateMatrix();
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

	return to_add;
}

void GameObject::UpdateBBox()
{
	for (uint i = 0; i < components.size(); i++)
	{
	
		if (components[i]->GetType() == COMPONENT_MESH)
		{
			ComponentMesh* c_mesh = dynamic_cast<ComponentMesh*>(components[i]);

			if (c_mesh->mesh != nullptr)
			{
				c_mesh->mesh->bbox.SetNegativeInfinity();
				c_mesh->mesh->bbox.Enclose((float3*)c_mesh->mesh->vertex, c_mesh->mesh->num_vertex); //crash #2
				//we create the obb if we transform the gmaeobject
				OBB obb;
				obb.SetFrom(c_mesh->mesh->bbox);
				if (my_transform != nullptr)
				{
					obb.Transform(my_transform->GetGlobalMatrix());
				}
				c_mesh->mesh->bbox = obb.MinimalEnclosingAABB();
				this->bbox = &c_mesh->mesh->bbox;
				//DrawBBox(bbox);
			}
		}
		for (int i = 0; i < childs.size(); ++i)
		{
			childs[i]->UpdateBBox();
		}
	}
}

bool GameObject::RemoveComponent(COMPONENT_TYPE type, int position)
{
	components.erase(components.begin() + position);
	return true;
}

void GameObject::DrawBBox(ComponentMesh* c_mesh)
{
	if (show_bbox && c_mesh != nullptr)
	{
		float3 bbox_vertex[8];
		c_mesh->mesh->bbox.GetCornerPoints(bbox_vertex);
		this->bbox = &c_mesh->mesh->bbox;

		DrawBBox(bbox);
	}
	else if (c_mesh == nullptr)
	{
		UpdateBBox();
	}
}

void GameObject::DrawBBox(AABB* bbox) //PREGUNTAR RICARD
{
	if (bbox != nullptr)
	{
		glBegin(GL_LINES);
		glLineWidth(1.0f);
		glColor4f(1.0f, 1.0f, 0.0f, 1.0f);

		for (uint i = 0; i < 12; i++)
		{
			glVertex3f(bbox->Edge(i).a.x, bbox->Edge(i).a.y, bbox->Edge(i).a.z);
			glVertex3f(bbox->Edge(i).b.x, bbox->Edge(i).b.y, bbox->Edge(i).b.z);
		}

		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

		glEnd();
	}
	else
		LOG("Couldn't draw BoundingBox!!");
}

AABB * GameObject::GetBBox()
{
	for (uint i = 0; i < components.size(); i++)
	{
		if (components[i]->GetType() == COMPONENT_MESH)
		{
			ComponentMesh* c_mesh = dynamic_cast<ComponentMesh*>(components[i]);

			if (c_mesh->mesh != nullptr)
			{
				c_mesh->mesh->bbox.SetNegativeInfinity();
				c_mesh->mesh->bbox.Enclose((float3*)c_mesh->mesh->vertex, c_mesh->mesh->num_vertex); //crash #2
				return &c_mesh->mesh->bbox;
			}
		}
	}
	return nullptr;
}

ComponentTransform * GameObject::GetTransform()
{
	for (int i = 0; i < components.size(); ++i)
	{
		if (components[i]->GetType() == COMPONENT_TRANSFORM)
			return (ComponentTransform*) components[i];
	}
}

GameObject * GameObject::GetParent() const
{
	return parent;
}

void GameObject::SetParent(GameObject* new_parent)
{
	if (new_parent != nullptr)
	{
		parent = new_parent;
		new_parent->AddChild(this);
	}
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
		return my_transform->GetTransposedGlobalMatrix();
	}
	return float4x4::zero;
}

float4x4 GameObject::GetGlobalMatrix() const
{
	float4x4 ret = float4x4::identity;

	ComponentTransform* my_trans = (ComponentTransform*)FindComponent(COMPONENT_TRANSFORM);

	if (my_trans != nullptr)
	{
		ret = my_trans->GetGlobalMatrix();
	}

	return ret;
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
	if (name != nullptr)
	{
		this->name = name;
	}
	else
		LOG("Couldn't set GameObject name!");
}

void GameObject::AddChild(GameObject* child)
{
	if (child != nullptr)
	{
		childs.push_back(child);
		child->parent = this;
	}
	else
		LOG("Couldn't add %s as a child!", child->name);
}

void GameObject::SetToDelete()
{
	want_delete = true;
}

void GameObject::UpdateMatrix() const
{
	my_transform->UpdateMatrix();
}

void GameObject::GetSceneGameObjects(std::vector<GameObject*>& scene_go) const
{
	scene_go.push_back((GameObject*)this);

	for (std::vector<GameObject*>::const_iterator item = childs.cbegin(); item != childs.cend(); ++item)
		(*item)->GetSceneGameObjects(scene_go);
}

bool GameObject::RemoveGameObject(GameObject * to_remove)
{
	if (to_remove != nullptr)
	{
		for (uint i = 0; i < childs.size(); i++)
		{
			GameObject* item = childs[i];

			if (item == to_remove)
			{

				childs.erase(childs.begin() + i);
				item->CleanRemove();
				RELEASE(item);
				item = nullptr;
				return true;
			}
		}
	}
	return false;
}

void GameObject::CleanRemove()
{
	if (App->editor->GetSelected() == this)
	{
		App->editor->SetSelected(nullptr);
	}

	for (uint i = 0; i < components.size(); i++)
	{
		Component* item = components[i];
		item->CleanUp();
		RELEASE(item);
		item = nullptr;
	}

	for (uint i = 0; i < childs.size(); i++)
	{
		GameObject* item = childs[i];

		item->want_delete = true;
		RELEASE(item);
		item = nullptr;
	}
	parent = nullptr;
}



bool GameObject::OnSave(JSON_Value* array) const
{
	if (parent != NULL)
	{
		//create new child
		JSON_Value* go_value = json_value_init_object();

		//target the new child
		JSON_Object *go_object = json_value_get_object(go_value);

		//copy values
		json_object_set_string(go_object, "Name", name.c_str());
		json_object_set_number(go_object, "UID", uid);

		if (parent != nullptr)
			json_object_set_number(go_object, "Parent", parent->uid);

		//create array for components
		JSON_Value* comp_array = json_value_init_array();
		json_object_set_value(go_object, "Components", comp_array);

		for (int i = 0; i < components.size(); i++)
		{
			components[i]->OnSave(comp_array, uid);
		}

		//add value into array
		if (array != nullptr)
		{
			JSON_Array* my_array = json_value_get_array(array);
			json_array_append_value(my_array, json_value_deep_copy(go_value));
		}

	}

	//call childs OnSave()
	for (int i = 0; i < childs.size(); i++)
	{
		childs[i]->OnSave(array);
	}

	return true;
}

bool GameObject::OnLoad(const JSONConfig data)
{
	uid = data.GetInt("UID");
	name = data.GetString("Name");

	uint size = data.GetArraySize("Components");
	for (uint i = 0; i < size; i++)
	{
		JSONConfig item_config = data.SetFocusArray("Components", i);
		
		Component* item = AddComponent((COMPONENT_TYPE)item_config.GetInt("Type"));

		switch (item->GetType())
		{
			case COMPONENT_MESH:
			{
				ComponentMesh* item_mesh = (ComponentMesh*)item;
				item_mesh->OnLoad(item_config);
			}

			case COMPONENT_MATERIAL:
			{
				ComponentMaterial* item_material = (ComponentMaterial*)item;
				item_material->OnLoad(item_config);
			}

			case COMPONENT_TRANSFORM:
			{
				ComponentTransform* item_transform = (ComponentTransform*)item;
				item_transform->OnLoad(item_config);
			}
		}
	}

	return true;
}

bool GameObject::IsCamera()
{
	for (int i = 0; i < components.size(); ++i)
	{
		if (components[i]->GetType() == COMPONENT_CAMERA)  //taking in account that a gameobject with a componentcamera are cameras, (just for now!) will probably need to be rewritten later
			return true;
	}
	return false;
}
