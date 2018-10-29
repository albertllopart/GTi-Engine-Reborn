#include "Component.h"
#include "Globals.h"
#include "Application.h"

Component::Component(COMPONENT_TYPE type): type(type)
{
	uid = App->rng->RandomInt(0, MAX_INT32_VALUE);
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

bool Component::OnSave(JSON_Value* value, JSON_Object* node, uint go_uid) const
{
	return true;
}


