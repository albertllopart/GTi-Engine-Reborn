#ifndef __RESOURCE_MESH__
#define __RESOURCE_MESH__

#include "Resource.h"
#include "MathGeoLib\Geometry\AABB.h"
#include "Glew/include/glew.h"

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

	//shader
	void CreateVBO();
	void CreateIBO();
	void CreateVAO();

	float* vertex_info = nullptr;
	uint vertex_num = 0;

	float* index_info = nullptr;
	uint index_num = 0;

	GLuint VBO = 0;
	GLuint IBO = 0;
	GLuint VAO = 0;
};


class ResourceMesh : public Resource
{
public:
	ResourceMesh(uint uid);
	~ResourceMesh();

	void LoadInMemory();
	void UnLoadFromMemory();
	//save
	//load

public:
	Mesh mesh;

};


#endif // !__RESOURCE_MESH__
