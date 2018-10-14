#ifndef __MODULE_RENDERER_3D_H__
#define __MODULE_RENDERER_3D_H__

#include "Module.h"
#include "Globals.h"
#include "Light.h"
#include "Primitive.h"
#include "MathGeoLib/Math/float3x3.h"
#include "MathGeoLib/Math/float4x4.h"
#include "MathGeoLib/Math/Quat.h"
#include "MathGeoLib\Geometry\AABB.h"
#define MAX_LIGHTS 8


struct Mesh
{
	std::string name;
	float3 pos;
	float3 scale;
	float3 euler_angles;
	Quat quat;
	AABB bbox;
	pCube* bbox_debug = nullptr;

	uint id_index = 0; // index in VRAM
	uint num_index = 0;
	uint* index = nullptr;

	uint id_vertex = 0; // unique vertex in VRAM
	uint num_vertex = 0;
	float* vertex = nullptr;

	//normals
	uint id_normals = 0;
	float* normals = nullptr;

	//text coords
	uint id_texcoord = 0; // id textcoords
	uint texture = 0; // text id
	float* texCoords = nullptr;

	//extra info
	uint faces = 0;
	std::string tex_name = "none";
	uint tex_width = 0;
	uint tex_height = 0;
};


class ModuleRenderer3D : public Module
{
public:
	ModuleRenderer3D(Application* app, bool start_enabled = true);
	~ModuleRenderer3D();

	
	update_status PreUpdate(float dt);
	update_status PostUpdate(float dt);

	bool Init(JSON_Object* node);
	bool CleanUp();
	void Draw(Mesh* toDraw);
	void OnResize(int width, int height);

	float4x4 perspective(float fovy, float aspect, float n, float f);

	void SetDepthTest();
	void SetCullFace();
	void SetLighting();
	void SetColorMaterial();
	void SetTexture2D();
	void SetWireFrame();

public:

	Light lights[MAX_LIGHTS];
	SDL_GLContext context;
	math::float3x3 NormalMatrix;
	math::float4x4 ModelMatrix, ViewMatrix, ProjectionMatrix;

	bool wireframe = false;
	bool depthTest = true;
	bool cullFace = true;
	bool lighting = false;
	bool colorMaterial = true;
	bool texture2D = true;
	bool show_normals = false;
};

#endif