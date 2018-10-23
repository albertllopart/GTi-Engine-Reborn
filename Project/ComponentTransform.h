#ifndef __COMPONENT_TRANSFORM__
#define __COMPONENT_TRANSFORM__

#include "Component.h"
#include "MathGeoLib/Math/float3.h"
#include "MathGeoLib/Math/Quat.h"

class ComponentTransform : public Component
{
public:

	ComponentTransform(float3 pos = { 0,0,0 }, float3 scale = { 1,1,1 }, Quat rot = { 0, 0, 0, 1 });
	~ComponentTransform();

	void Update();
	void OnEditor();


private:

	float3 pos;
	float3 new_pos;
	float3 scale;
	float3 rot_euler;
	Quat rot_quat;
};

#endif // __COMPONENT_TRANSFORM__
