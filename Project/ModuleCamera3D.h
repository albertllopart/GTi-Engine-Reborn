#ifndef __MODULE_CAMERA_3D_H__
#define __MODULE_CAMERA_3D_H__

#include "Module.h"
#include "Globals.h"
#include "MathGeoLib/Math/float3.h"
#include "MathGeoLib/Math/float4x4.h"
#include "MathGeoLib/Geometry/LineSegment.h"

class MousePicking;

class ModuleCamera3D : public Module
{
public:
	ModuleCamera3D(Application* app, bool start_enabled = true);
	~ModuleCamera3D();

	bool Init(JSON_Object* node);
	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	void Look(const float3 &Position);
	void LookAt(const float3 & position);
	void Move(float dt);
	void Orbit(float dx, float dy);
	void Rotate(float dx, float dy);
	
	void Zoom(float zoom);
	void CameraRotation()const;
	
	//void CenterToMesh(Mesh * mesh);
	ComponentCamera* GetCamera() const;
	ComponentCamera* SetCamera(ComponentCamera* cam);

	float3 GetPosition()const;
	void SetPosition(float3 pos);
	void CenterToMesh(ComponentMesh * mesh);

public:

	ComponentCamera* camera = nullptr;
	bool show_ray = false;
private:

	float3 reference = float3::zero;
	float3 new_pos = float3::zero;

	LineSegment picking = LineSegment(float3::zero, float3::zero);

	MousePicking* mouse_picker = nullptr;
};

#endif