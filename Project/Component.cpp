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

GameObject * Component::GetMyGo() const
{
	return my_go;
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

void Component::OnUpdateMatrix(const float4x4 mat)
{

}

void Component::DestroyComponent()
{
	want_delete = true;
}

void Component::ShowInspectorWindow()
{
	
}

COMPONENT_TYPE Component::GetType() const
{
	return type;
}

void Component::GenerateUID()
{
	uid = App->rng->Random32();
}

bool Component::OnSave(JSON_Value* array, uint go_uid)
{
	return true;
}


