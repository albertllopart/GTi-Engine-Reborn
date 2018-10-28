#ifndef __COMPONENT_TRANSFORM__
#define __COMPONENT_TRANSFORM__

#include "Component.h"
#include "MathGeoLib/Math/float3.h"
#include "MathGeoLib/Math/Quat.h"
#include "MathGeoLib/Math/float4x4.h"

class ComponentTransform : public Component
{
public:

	ComponentTransform(float3 pos = { 0,0,0 }, float3 scale = { 1,1,1 }, Quat rot = { 0, 0, 0, 1 });
	~ComponentTransform();

	void Update();
	void OnEditor();
	void ShowInspectorWindow();

	void UpdatePosition(float3 pos);
	void UpdateRotation(float3 rot);
	void UpdateScale(float3 scale);
	void UpdateMatrix();

	float4x4 GetGlobalMatrix()const;

private:
	float4x4 trans_matrix;
	float4x4 global_trans_matrix;


	float3 pos;
	float3 new_pos;
	float3 scale;
	float3 rot_euler;
	Quat rot_quat;
};

#endif // __COMPONENT_TRANSFORM__
