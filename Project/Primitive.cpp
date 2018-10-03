#include "Globals.h"
#include "Glew/include/glew.h"
#include <gl/GL.h>
#include <gl/GLU.h>
#include "Primitive.h"
#include "MathGeoLib/Math/TransformOps.h"
#include "MathGeoLib/Math/MathConstants.h"


// ------------------------------------------------------------
Primitive::Primitive() : transform(float4x4::identity), color(White), wire(false), axis(false), type(PrimitiveTypes::Primitive_Point)
{}

// ------------------------------------------------------------
PrimitiveTypes Primitive::GetType() const
{
	return type;
}

// ------------------------------------------------------------
void Primitive::Render() const
{
	glPushMatrix();
	glMultMatrixf((GLfloat*)transform.Transposed().ptr());

	// GEOLIB
	if(axis == true)
	{
		// Draw Axis Grid
		glLineWidth(2.0f);

		glBegin(GL_LINES);

		glColor4f(1.0f, 0.0f, 0.0f, 1.0f);

		glVertex3f(0.0f, 0.0f, 0.0f); glVertex3f(1.0f, 0.0f, 0.0f);
		glVertex3f(1.0f, 0.1f, 0.0f); glVertex3f(1.1f, -0.1f, 0.0f);
		glVertex3f(1.1f, 0.1f, 0.0f); glVertex3f(1.0f, -0.1f, 0.0f);

		glColor4f(0.0f, 1.0f, 0.0f, 1.0f);

		glVertex3f(0.0f, 0.0f, 0.0f); glVertex3f(0.0f, 1.0f, 0.0f);
		glVertex3f(-0.05f, 1.25f, 0.0f); glVertex3f(0.0f, 1.15f, 0.0f);
		glVertex3f(0.05f, 1.25f, 0.0f); glVertex3f(0.0f, 1.15f, 0.0f);
		glVertex3f(0.0f, 1.15f, 0.0f); glVertex3f(0.0f, 1.05f, 0.0f);

		glColor4f(0.0f, 0.0f, 1.0f, 1.0f);

		glVertex3f(0.0f, 0.0f, 0.0f); glVertex3f(0.0f, 0.0f, 1.0f);
		glVertex3f(-0.05f, 0.1f, 1.05f); glVertex3f(0.05f, 0.1f, 1.05f);
		glVertex3f(0.05f, 0.1f, 1.05f); glVertex3f(-0.05f, -0.1f, 1.05f);
		glVertex3f(-0.05f, -0.1f, 1.05f); glVertex3f(0.05f, -0.1f, 1.05f);

		glEnd();

		glLineWidth(1.0f);
	}

	glColor3f(color.r, color.g, color.b);

	if(wire)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	InnerRender();

	glPopMatrix();
}

// ------------------------------------------------------------
void Primitive::InnerRender() const
{
	glPointSize(5.0f);

	glBegin(GL_POINTS);

	glVertex3f(0.0f, 0.0f, 0.0f);

	glEnd();

	glPointSize(1.0f);
}

// ------------------------------------------------------------
void Primitive::SetPos(float x, float y, float z)
{
	// GEOLIB
	//transform.translate(x, y, z);
	transform = float4x4::Translate(x, y, z).ToFloat4x4() * transform;
}

// ------------------------------------------------------------
void Primitive::SetRotation(float angle, const float3 &u)
{
	// GEOLIB
	//transform.rotate(angle, u);
	transform = float4x4::RotateAxisAngle(u, angle) * transform;
}

// ------------------------------------------------------------
void Primitive::Scale(float x, float y, float z)
{
	// GEOLIB
	//transform.scale(x, y, z);
	transform = float4x4::Scale(x, y, z).ToFloat4x4() * transform;
}

// array CUBE ============================================
pCube::pCube(float3 position,float3 size) : Primitive(), size(size.x, size.y, size.z)
{
	type = PrimitiveTypes::Primitive_Cube;


	float cube[108] =
	{
		-0.5f,  0.5f,  0.5f,	//A	0
		 0.5f, -0.5f,  0.5f,	//B	1
		 0.5f,  0.5f,  0.5f,	//C	2

		-0.5f,  0.5f,  0.5f,	//A	0
		-0.5f, -0.5f,  0.5f,	//D	3
		 0.5f, -0.5f,  0.5f,	//B	1

		 0.5f,  0.5f,  0.5f,	//C	2
		 0.5f, -0.5f,  0.5f,	//B	1
		 0.5f,  0.5f, -0.5f,	//E	4

		 0.5f,  0.5f, -0.5f,	//E	4
		 0.5f, -0.5f,  0.5f,	//B	1
		 0.5f, -0.5f, -0.5f,	//F	7

		-0.5f,  0.5f, -0.5f,	//G	8
		-0.5f, -0.5f,  0.5f,	//D	3
		-0.5f,  0.5f,  0.5f,	//A	0

		-0.5f, -0.5f, -0.5f,	//H
		-0.5f, -0.5f,  0.5f,	//D
		-0.5f,  0.5f, -0.5f,	//G

		 0.5f,  0.5f, -0.5f,	//E
		 0.5f, -0.5f, -0.5f,	//F
		-0.5f,  0.5f, -0.5f,	//G

		 0.5f, -0.5f, -0.5f,	//F
		-0.5f, -0.5f, -0.5f,	//H
		-0.5f,  0.5f, -0.5f,	//G

		 0.5f,  0.5f, -0.5f,	//E
		-0.5f,  0.5f,  0.5f,	//A
		 0.5f,  0.5f,  0.5f,	//C

		-0.5f,  0.5f,  0.5f,	//A
		 0.5f,  0.5f, -0.5f,	//E
		-0.5f,  0.5f, -0.5f,	//G

		 0.5f, -0.5f,  0.5f,	//B
		-0.5f, -0.5f,  0.5f,	//D
		 0.5f, -0.5f, -0.5f,	//F

		-0.5f, -0.5f, -0.5f,	//H
		 0.5f, -0.5f, -0.5f,	//F
		-0.5f, -0.5f,  0.5f,	//D
	};


	glGenBuffers(1, (GLuint*)&my_id);
	glBindBuffer(GL_ARRAY_BUFFER, my_id);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 108, cube, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	SetPos(position.x, position.y, position.z);
}

void pCube::InnerRender() const
{	
	glEnableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, my_id);
	glVertexPointer(3, GL_FLOAT, 0, NULL);

	glDrawArrays(GL_TRIANGLES, 0, 108);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glDisableClientState(GL_VERTEX_ARRAY);
}

//INDICES CUBE=============================
pCube2::pCube2(float3 position, float3 size )
{
	GLfloat vertices[24] = {
						-0.5f,  0.5f,  0.5f,	//A	0
						0.5f, -0.5f,  0.5f,		//B	1	
						0.5f,  0.5f,  0.5f,		//C	2	
						-0.5f, -0.5f,  0.5f,	//D	3
						0.5f,  0.5f, -0.5f,		//E	4
						0.5f, -0.5f, -0.5f,		//F	5	
						-0.5f,  0.5f, -0.5f,	//G	6
						-0.5f, -0.5f, -0.5f,	//H	7
	};          
	GLubyte indices[] = { 0,1,2, 2,3,0,  
						 0,3,4, 4,5,0,
						 0,5,6, 6,1,0,
						 1,6,7, 7,2,1,
						 7,4,3, 3,2,7,
						 4,7,6, 6,5,4 };




	glGenBuffers(1, &my_id);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, my_id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * 36, indices, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

//http://www.songho.ca/opengl/gl_vertexarray.html

}

void pCube2::InnerRender() const
{
	glEnableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, my_id);
	glVertexPointer(3, GL_FLOAT, 0, NULL);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, my_indices);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, NULL);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glDisableClientState(GL_VERTEX_ARRAY);

}

// SPHERE ============================================
pSphere::pSphere() : Primitive(), radius(1.0f)
{
	type = PrimitiveTypes::Primitive_Sphere;
}

pSphere::pSphere(float radius) : Primitive(), radius(radius)
{
	type = PrimitiveTypes::Primitive_Sphere;
}

void pSphere::InnerRender() const
{
	
}


// CYLINDER ============================================
pCylinder::pCylinder() : Primitive(), radius(1.0f), height(1.0f)
{
	type = PrimitiveTypes::Primitive_Cylinder;
}

pCylinder::pCylinder(float radius, float height) : Primitive(), radius(radius), height(height)
{
	type = PrimitiveTypes::Primitive_Cylinder;
}

void pCylinder::InnerRender() const
{
	int n = 30;

	// Cylinder Bottom
	glBegin(GL_POLYGON);
	
	for(int i = 360; i >= 0; i -= (360 / n))
	{
		float a = i * math::pi / 180; // degrees to radians
		glVertex3f(-height*0.5f, radius * cos(a), radius * sin(a));
	}
	glEnd();

	// Cylinder Top
	glBegin(GL_POLYGON);
	glNormal3f(0.0f, 0.0f, 1.0f);
	for(int i = 0; i <= 360; i += (360 / n))
	{
		float a = i * math::pi / 180; // degrees to radians
		glVertex3f(height * 0.5f, radius * cos(a), radius * sin(a));
	}
	glEnd();

	// Cylinder "Cover"
	glBegin(GL_QUAD_STRIP);
	for(int i = 0; i < 480; i += (360 / n))
	{
		float a = i * math::pi / 180; // degrees to radians

		glVertex3f(height*0.5f,  radius * cos(a), radius * sin(a) );
		glVertex3f(-height*0.5f, radius * cos(a), radius * sin(a) );
	}
	glEnd();
}

// LINE ==================================================
pLine::pLine() : Primitive(), origin(0, 0, 0), destination(1, 1, 1)
{
	type = PrimitiveTypes::Primitive_Line;
}

pLine::pLine(float x, float y, float z) : Primitive(), origin(0, 0, 0), destination(x, y, z)
{
	type = PrimitiveTypes::Primitive_Line;
}

void pLine::InnerRender() const
{
	glLineWidth(2.0f);

	glBegin(GL_LINES);

	glVertex3f(origin.x, origin.y, origin.z);
	glVertex3f(destination.x, destination.y, destination.z);

	glEnd();

	glLineWidth(1.0f);
}

// PLANE ==================================================
pPlane::pPlane() : Primitive(), normal(0, 1, 0), constant(1)
{
	type = PrimitiveTypes::Primitive_Plane;
}

pPlane::pPlane(float x, float y, float z, float d) : Primitive(), normal(x, y, z), constant(d)
{
	type = PrimitiveTypes::Primitive_Plane;
}

void pPlane::InnerRender() const
{
	glLineWidth(1.0f);

	glBegin(GL_LINES);

	float d = 200.0f;

	for(float i = -d; i <= d; i += 1.0f)
	{
		glVertex3f(i, 0.0f, -d);
		glVertex3f(i, 0.0f, d);
		glVertex3f(-d, 0.0f, i);
		glVertex3f(d, 0.0f, i);
	}

	glEnd();
}

