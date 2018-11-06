#ifndef __COMPONENT_CAMERA__
#define __COMPONENT_CAMERA__

#include "Component.h"
//#include "Globals.h"
#include "MathGeoLib/Geometry/Frustum.h"
#include "MathGeoLib/Math/float3.h"

class ComponentCamera : public Component
{
public: 
	ComponentCamera(float3 pos = { 0,1,-2 }, float3 front = { 0,0,1 }, float3 up = { 0,0.5f,0 }, float near_plane_dist = 1.0f, float far_plane_dist = 100.0f, float vertica_fov = 10, float aspect_ratio = 16.f / 9.f, FrustumType type = FrustumType::PerspectiveFrustum);
	~ComponentCamera();

	void Update();

	void SetPos(float3 pos);
	void OnEditor();
	void SetAspectRatio(float x, float y);
	void SetFov();
	void SetVerticalFOV(float value);
	void ShowInspectorWindow();
	void OnUpdateMatrix(const float4x4 & matrix);
	void UpdateMatrix();
	void DrawDebug() const;
	void Culling()const;


	float* GetProjectionMatrix()const;
	Frustum GetFrustum() const;

public:
	bool update_mat = false;

public:

	float fov;
	float aspect_ratio;
	bool culling = false;
	bool main_camera = false;
	GameObject* scene_go;
	Frustum frustum;
	float4x4 view_matrix = float4x4::zero;
	float4x4 projection_matrix = float4x4::zero;
	//TODO ADD CULLING TO THE FRUSTUM
};


#endif // !__COMPONENT_CAMERA__
