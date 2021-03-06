#ifndef __COMPONENT_CAMERA__
#define __COMPONENT_CAMERA__

#include "Component.h"
#include "Globals.h"
#include "MathGeoLib/Geometry/Frustum.h"
#include "MathGeoLib/Math/float3.h"
#include <vector>

class ComponentCamera : public Component
{
public: 
	ComponentCamera();
	~ComponentCamera();

	void Update();

	//getters
	float* GetViewMatrix()const;
	float* GetProjectionMatrix()const;
	math::Frustum GetFrustum() const;
	math::float4x4 GetOpenGLViewMatrix();
	math::float4x4 GetOpenGLProjectionMatrix();
	float GetAspectRatio() const;

	//setters
	void SetPos(math::float3 pos);
	void SetAspectRatio(float ratio);
	void SetFov();
	void SetVerticalFOV(float value);
	
	void OnEditor();
	void ShowInspectorWindow();
	void OnUpdateMatrix(const math::float4x4 & matrix);
	void UpdateMatrix();
	void DrawDebug() const;
	void Culling()const;

	void Look(const math::float3& position);

	bool OnSave(JSON_Value* array, uint go_uid);
	bool OnLoad(JSONConfig data);

	bool update_mat = false;
	math::Frustum frustum;
	bool projection_changed = false;

public:

	float fov = 60.0f;
	float aspect_ratio = 0.0f;
	bool culling = false;
	bool main_camera = false;

	GameObject* scene_go = nullptr;

	math::float4x4 view_matrix = math::float4x4::zero;
	math::float4x4 projection_matrix = math::float4x4::zero;

};


#endif // !__COMPONENT_CAMERA__
