#ifndef __PhysBody3D_H__
#define __PhysBody3D_H__

#include "p2List.h"
#include "glmath.h"

class btRigidBody;
class Module;

// =================================================
struct PhysBody3D
{
	friend class ModulePhysics3D;
public:
	PhysBody3D(btRigidBody* body);
	~PhysBody3D();

	void Push(float x, float y, float z);
	void GetTransform(float* matrix) const;
	void SetTransform(const float* matrix) const;
	void SetPos(float x, float y, float z);
	void SetAsSensor(bool is_sensor);
	void SetAsCheckpoint(bool is_checkpoint, int id, vec3 pos = { 0,0,0 });
	void SetFriction(int friction);
	bool IsSensor() const;
	bool IsCheckpoint() const;
	vec3 CheckPointPos()const;
	int CheckPointId() const;
	btRigidBody* GetRigidBody();

private:
	btRigidBody* body = nullptr;
	bool is_sensor = false;
	bool is_checkpoint = false;
	vec3 checkpoint_pos = { 0,0,0 };
	int id = 0;

public:
	p2List<Module*> collision_listeners;
};

#endif // __PhysBody3D_H__