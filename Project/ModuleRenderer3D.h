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
#include "ComponentMaterial.h"
#include "ComponentMesh.h"
#include "GameObject.h"
#include "ComponentCamera.h"

#define MAX_LIGHTS 8

class ModuleRenderer3D : public Module
{
public:
	ModuleRenderer3D(Application* app, bool start_enabled = true);
	~ModuleRenderer3D();

	
	update_status PreUpdate(float dt);
	update_status PostUpdate(float dt);

	bool Init(JSON_Object* node);
	bool CleanUp();
	void FrustumCulling() const;
	//void Draw(Mesh* toDraw);
	void Draw(ComponentMesh * to_draw);
	void Draw(GameObject* toDraw);
	void OnResize(int width, int height);

	float4x4 perspective(float fovy, float aspect, float n, float f);

	void SetDepthTest();
	void SetCullFace();
	void SetLighting();
	void SetColorMaterial();
	void SetTexture2D();
	void SetWireFrame();

	void SetCamera(ComponentCamera * cam);
	void SetMainCamera(ComponentCamera * cam);

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

	ComponentCamera* camera = nullptr;
	ComponentCamera* active_camera = nullptr;
};

#endif