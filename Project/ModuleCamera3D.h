#ifndef __MODULE_CAMERA_3D_H__
#define __MODULE_CAMERA_3D_H__

#include "Module.h"
#include "Globals.h"
#include "MathGeoLib/Math/float3.h"
#include "MathGeoLib/Math/float4x4.h"


class ModuleCamera3D : public Module
{
public:
	ModuleCamera3D(Application* app, bool start_enabled = true);
	~ModuleCamera3D();

	bool Start();
	update_status Update(float dt);
	bool CleanUp();

	void Look(const float3 &Position, const float3 &Reference, bool RotateAroundReference = false);
	void LookAt(const float3 &Spot);
	void Move(const float3 &Movement);
	void CameraRotation()const;
	float* GetViewMatrix();
	//void CenterToMesh(Mesh * mesh);
	ComponentCamera* GetCamera() const;

private:

	void CalculateViewMatrix();

public:
	
	math::float3 X, Y, Z, Position, Reference;
	math::float3 focus;
private:

	math::float4x4 ViewMatrix, ViewMatrixInverse;
	bool free_move = false;
	ComponentCamera* camera = nullptr;
};

#endif