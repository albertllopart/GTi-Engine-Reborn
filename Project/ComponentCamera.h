#ifndef __COMPONENT_CAMERA__
#define __COMPONENT_CAMERA__

#include "Component.h"
//#include "Globals.h"
#include "MathGeoLib/Geometry/Frustum.h"
#include "MathGeoLib/Math/float3.h"

class ComponentCamera : public Component
{
	ComponentCamera(GameObject* my_go, float3 pos = { 0,0,0 }, float3 front = { 0,0,1 }, float3 up = { 0,0.5f,0 }, float near_plane_dist = 1.0f, float far_plane_dist = 1000.0f, float vertica_fov = 10, float aspect_ratio = 16.f / 9.f, FrustumType type = FrustumType::PerspectiveFrustum);
	~ComponentCamera();

	void Update();

	void SetPos(float3 pos);
	void OnEditor();
	void SetAspectRatio(float x, float y);
	void SetFov();
	void SetVerticalFOV(float value);

	//void DrawDebug() const; TODO draw camera limits
	



private:

	float fov;
	float aspect_ratio;
	Frustum frustum;
	//TODO ADD CULLING TO THE FRUSTUM
};


#endif // !__COMPONENT_CAMERA__
