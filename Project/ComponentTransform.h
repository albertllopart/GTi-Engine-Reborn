#ifndef __COMPONENT_TRANSFORM__
#define __COMPONENT_TRANSFORM__

#include "Component.h"
#include "MathGeoLib/Math/float3.h"
#include "MathGeoLib/Math/Quat.h"
#include "MathGeoLib/Math/float4x4.h"

class ComponentCamera;
class ComponentTransform : public Component
{
public:

	ComponentTransform(math::float3 pos = { 0,0,0 }, math::float3 scale = { 1,1,1 }, math::Quat rot = { 0, 0, 0, 1 });
	~ComponentTransform();

	void SetTransform(math::float3 scale, math::Quat rotation, math::float3 position);
	void OnEditor();
	void ShowInspectorWindow();
	void Update();

	void UpdatePosition(math::float3 pos);
	void UpdateRotation(math::float3 rot);
	void UpdateScale(math::float3 scale);
	void UpdateMatrix();
	void TransformCamera();

	math::float4x4 GetGlobalMatrix()const;
	math::float4x4 GetTransposedGlobalMatrix()const;

	math::float3 GetPosition()const;

	bool OnSave(JSON_Value* array, uint go_uid);
	bool OnLoad(JSONConfig data);

	void SetPosition(math::float3 pos);
	void ShowGizmo(ComponentCamera & camera);

	math::float3 pos = math::float3::zero;
	math::float3 new_pos = math::float3::zero;
	math::float3 scale = math::float3::one;
	math::float3 rot_euler = math::float3::zero;
	math::Quat rot_quat = math::Quat::identity;

private:

	math::float4x4 trans_matrix = math::float4x4::identity;
	math::float4x4 global_trans_matrix = math::float4x4::identity;
	math::float4x4 global_trans_matrix_transposed = math::float4x4::identity;

	bool needs_update = false;
};

#endif // __COMPONENT_TRANSFORM__
