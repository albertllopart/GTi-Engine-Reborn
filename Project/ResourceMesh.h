#ifndef __RESOURCE_MESH__
#define __RESOURCE_MESH__

#include "Resource.h"
#include "MathGeoLib\Geometry\AABB.h"

struct Mesh
{
	Mesh();

	uint mesh_id = 0;

	//std::string name;
	AABB bbox;

	uint id_index = 0; // index in VRAM
	uint num_index = 0;
	uint* index = nullptr;

	uint id_vertex = 0; // unique vertex in VRAM
	uint num_vertex = 0;
	float* vertex = nullptr;

	//normals
	uint id_normals = 0;
	float* normals = nullptr;
	uint faces = 0;

	uint id_texcoord = 0; 
	float* texCoords = nullptr;
};


class ResourceMesh : public Resource
{
public:
	ResourceMesh(uint uid);
	~ResourceMesh();

	void LoadInMemory();
	//save
	//load

public:
	Mesh mesh;

};


#endif // !__RESOURCE_MESH__
