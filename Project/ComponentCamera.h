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
	Frustum GetFrustum() const;
	float* GetOpenGLViewMatrix();
	float* GetOpenGLProjectionMatrix();
	float GetAspectRatio() const;

	//setters
	void SetPos(float3 pos);
	void SetAspectRatio(float x, float y);
	void SetFov();
	void SetVerticalFOV(float value);
	
	void OnEditor();
	void ShowInspectorWindow();
	void OnUpdateMatrix(const float4x4 & matrix);
	void UpdateMatrix();
	void DrawDebug() const;
	void Culling()const;

	void Look(const float3& position);

public:

	bool update_mat = false;
	Frustum frustum;

private:

	float fov;
	float aspect_ratio;
	bool culling = false;
	bool main_camera = false;

	bool projection_changed = false;

	GameObject* scene_go = nullptr;

	float4x4 view_matrix = float4x4::zero;
	float4x4 projection_matrix = float4x4::zero;

};


#endif // !__COMPONENT_CAMERA__
