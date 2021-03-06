
#pragma once
#include "Color.h"
#include "MathGeoLib/Math/float4x4.h"
#include "MathGeoLib/Math/float3.h"

enum PrimitiveTypes
{
	Primitive_Point,
	Primitive_Line,
	Primitive_Plane,
	Primitive_Cube,
	Primitive_Sphere,
	Primitive_Cylinder
};

class Primitive
{
public:

	Primitive();

	virtual void	Render() const;
	virtual void	InnerRender() const;
	void			SetPos(float x, float y, float z);
	void			SetRotation(float angle, const math::float3 &u);
	void			Scale(float x, float y, float z);
	PrimitiveTypes	GetType() const;

public:
	
	Color color;
	math::float4x4 transform;
	bool axis,wire;

protected:
	PrimitiveTypes type;
};

// ==============VERTEX CUBE===========
class pCube : public Primitive
{
public :
	pCube(math::float3 position, math::float3 size = math::float3(1.f, 1.f, 1.f));
	void InnerRender() const;
public:
	math::float3 size;
	uint my_id;
};
// ==============INDEX CUBE===========
class pCube2 : public Primitive
{
public:
	pCube2(math::float3 position, math::float3 size = math::float3(1.f,1.f,1.f));
	void InnerRender() const;
public:
	math::float3 size;
	uint my_id;
	//uint my_vertex;
	uint my_indices;
};
// ============================================
class pSphere : public Primitive
{
public:
	pSphere();
	pSphere(float radius);
	void InnerRender() const;
public:
	float radius;
};

// ============================================
class pCylinder : public Primitive
{
public:
	pCylinder();
	pCylinder(float radius, float height);
	void InnerRender() const;
public:
	float radius;
	float height;
};

// ============================================
class pLine : public Primitive
{
public:
	pLine();
	pLine(float x, float y, float z);
	pLine(float oX, float oY, float oZ, float dX, float dY, float dZ);
	void InnerRender() const;
public:
	math::float3 origin;
	math::float3 destination;
};

// ============================================
class pPlane : public Primitive
{
public:
	pPlane();
	pPlane(float x, float y, float z, float d);
	void InnerRender() const;
public:
	math::float3 normal;
	float constant;
};