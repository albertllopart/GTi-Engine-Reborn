#pragma once
#include "Module.h"
#include "Globals.h"
#include "Light.h"
#include "MathGeoLib/Math/float3x3.h"
#include "MathGeoLib/Math/float4x4.h"

#define MAX_LIGHTS 8

class ModuleRenderer3D : public Module
{
public:
	ModuleRenderer3D(Application* app, bool start_enabled = true);
	~ModuleRenderer3D();

	bool Init(JSON_Object* node);
	update_status PreUpdate(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();

	void OnResize(int width, int height);

	float4x4 perspective(float fovy, float aspect, float n, float f);

	void Save(JSON_Object* node);
	void Load(JSON_Object* node);

	void SetDepthTest();

	void SetCullFace();

	void SetLighting();

	void SetColorMaterial();

	void SetTexture2D();

public:

	Light lights[MAX_LIGHTS];
	SDL_GLContext context;
	float3x3 NormalMatrix;
	float4x4 ModelMatrix, ViewMatrix, ProjectionMatrix;

	bool wframe;
	bool depthTest;
	bool cullFace;
	bool lighting;
	bool colorMaterial;
	bool texture2D;
};