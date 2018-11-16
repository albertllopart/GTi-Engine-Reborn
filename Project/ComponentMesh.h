#ifndef __COMPONENT_MESH__
#define __COMPONENT_MESH__

#include "Component.h"
#include "Globals.h"
#include "MathGeoLib/Math/float3.h"
#include "MathGeoLib/Math/Quat.h"
#include "Primitive.h"
#include "ResourceMesh.h"

class JSONConfig;

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
	ResourceMesh* mesh;
};

#endif//__COMPONENT_MESH__