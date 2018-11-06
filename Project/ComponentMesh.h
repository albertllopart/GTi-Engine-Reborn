#ifndef __COMPONENT_MESH__
#define __COMPONENT_MESH__

#include "Component.h"
#include "Globals.h"
#include "MathGeoLib/Math/float3.h"
#include "MathGeoLib/Math/Quat.h"
#include "MathGeoLib\Geometry\AABB.h"
#include "Primitive.h"

class JSONConfig;

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
	uint faces = 0;//WARNING TODO THIS

				   //text coords
	uint id_texcoord = 0; // id textcoords
	float* texCoords = nullptr;
};

class ComponentMesh : public Component
{
public:
	ComponentMesh();
	~ComponentMesh();

	void Update();
	void PostUpdate();

	void OnEditor();
	void ShowInspectorWindow();
	float3 GetCenter() const;

	bool OnSave(JSON_Value* array, uint go_uid);
	bool OnLoad(JSONConfig data);

public:
	std::string source;
	Mesh * mesh = nullptr;
};

#endif//__COMPONENT_MESH__