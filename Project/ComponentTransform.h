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

	ComponentTransform(float3 pos = { 0,0,0 }, float3 scale = { 1,1,1 }, Quat rot = { 0, 0, 0, 1 });
	~ComponentTransform();

	void SetTransform(float3 scale, Quat rotation, float3 position);
	void OnEditor();
	void ShowInspectorWindow();
	void Update();

	void UpdatePosition(float3 pos);
	void UpdateRotation(float3 rot);
	void UpdateScale(float3 scale);
	void UpdateMatrix();
	void TransformCamera();

	float4x4 GetGlobalMatrix()const;
	float4x4 GetTransposedGlobalMatrix()const;

	float3 GetPosition()const;

	bool OnSave(JSON_Value* array, uint go_uid);
	bool OnLoad(JSONConfig data);

	void SetPosition(float3 pos);
	void ShowGizmo(ComponentCamera & camera);

	float3 pos = float3::zero;
	float3 new_pos = float3::zero;
	float3 scale = float3::one;
	float3 rot_euler = float3::zero;
	Quat rot_quat = Quat::identity;

private:

	float4x4 trans_matrix = float4x4::identity;
	float4x4 global_trans_matrix = float4x4::identity;
	float4x4 global_trans_matrix_transposed = float4x4::identity;

	bool needs_update = false;
};

#endif // __COMPONENT_TRANSFORM__
