#include "PhysBody3D.h"
#include "glmath.h"
#include "Bullet/include/btBulletDynamicsCommon.h"

// =================================================
PhysBody3D::PhysBody3D(btRigidBody* body) : body(body)
{
	body->setUserPointer(this);
}

// ---------------------------------------------------------
PhysBody3D::~PhysBody3D()
{
	delete body;
}

// ---------------------------------------------------------
void PhysBody3D::Push(float x, float y, float z)
{
	body->applyCentralImpulse(btVector3(x, y, z));
}

// ---------------------------------------------------------
void PhysBody3D::GetTransform(float* matrix) const
{
	if(body != NULL && matrix != NULL)
	{
		body->getWorldTransform().getOpenGLMatrix(matrix);
	}
}

// ---------------------------------------------------------
void PhysBody3D::SetTransform(const float* matrix) const
{
	if(body != NULL && matrix != NULL)
	{
		btTransform t;
		t.setFromOpenGLMatrix(matrix);
		body->setWorldTransform(t);
	}
}

// ---------------------------------------------------------
void PhysBody3D::SetPos(float x, float y, float z)
{
	btTransform t = body->getWorldTransform();
	t.setOrigin(btVector3(x, y, z));
	body->setWorldTransform(t);
}

// ---------------------------------------------------------
void PhysBody3D::SetAsSensor(bool is_sensor)
{
	if(this->is_sensor != is_sensor)
	{
		this->is_sensor = is_sensor;
		if(is_sensor == true)
			body->setCollisionFlags(body->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE);
		else
			body->setCollisionFlags(body->getCollisionFlags() &~ btCollisionObject::CF_NO_CONTACT_RESPONSE);
	}
}

// ---------------------------------------------------------
bool PhysBody3D::IsSensor() const
{
	return is_sensor;
}

// ---------------------------------------------------------
bool PhysBody3D::IsCheckpoint() const
{
	return is_checkpoint;
}
void PhysBody3D::SetAsCheckpoint(bool is_checkpoint,int id, vec3 pos)
{
	if (this->is_checkpoint != is_checkpoint)
	{
		this->is_checkpoint = is_checkpoint;

		if (is_checkpoint == true) {
			checkpoint_pos = pos;
			this->id = id;
			body->setCollisionFlags(body->getCollisionFlags() | btCollisionObject::CF_NO_CONTACT_RESPONSE);
			this->is_sensor = true;
		}
		else {
			body->setCollisionFlags(body->getCollisionFlags() &~btCollisionObject::CF_NO_CONTACT_RESPONSE);
			this->is_sensor = false;
		}
	}
}

// ---------------------------------------------------------
vec3 PhysBody3D::CheckPointPos()const {
	if (IsCheckpoint()) {
		return checkpoint_pos;
	}
}

// ---------------------------------------------------------
int PhysBody3D::CheckPointId()const {
	if (is_checkpoint == true) {
		return id;
	}
	else return 0;
}

btRigidBody* PhysBody3D::GetRigidBody() {
	return body;
}

void PhysBody3D::SetFriction(int friction) {
	body->setFriction(friction);
}