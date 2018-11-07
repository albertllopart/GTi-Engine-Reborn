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
	void CenterOn(const float3& position, float distance);
	void LookAt(const float3 &Spot);
	void Move(float dt);
	void Orbit(float dx, float dy);
	void LookAt(float dx, float dy);
	void CameraRotation()const;
	float* GetViewMatrix();
	//void CenterToMesh(Mesh * mesh);
	ComponentCamera* GetCamera() const;
	ComponentCamera* SetCamera(ComponentCamera* cam);

	float3 GetPosition()const;

private:

	void CalculateViewMatrix();

public:
	
	math::float3 X, Y, Z, Position, Reference;
	math::float3 focus;
private:

	math::float4x4 ViewMatrix, ViewMatrixInverse;
	bool free_move = false;

	ComponentCamera* camera = nullptr;
	float3 looking_at = float3::zero;
	bool looking = false;

	math::LineSegment picking;

	MousePicking* mouse_picker = nullptr;
};

#endif