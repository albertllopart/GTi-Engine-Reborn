#include "ComponentTransform.h"


ComponentTransform::ComponentTransform(GameObject* my_go, float3 pos, float3 scale, Quat rot): Component(my_go,COMPONENT_TYPE::COMPONENT_TRANSFORM),pos(pos), new_pos(pos), scale(scale),rot_quat(rot)
{
	name = "ComponentTransform";

}

ComponentTransform::~ComponentTransform()
{
}

void ComponentTransform::Update()
{

}

void ComponentTransform::OnEditor()
{
	//TODO
}
