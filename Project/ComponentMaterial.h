#ifndef __COMPONENT_MATERIAL_
#define __COMPONENT_MATERIAL_

#include "Component.h"
#include "Globals.h"

class ResourceMaterial;
class ShaderProgram;
class ComponentMaterial : public Component
{
public:
	ComponentMaterial();
	~ComponentMaterial();

	void OnEditor();
	void LoadTexture(const char* path);

	uint GetID()const;
	void SetID(uint id);
	void SetTextureName(const char* new_name);
	void SetTextureSize(uint width, uint height);
	void ShowInspectorWindow();

	bool OnSave(JSON_Value* array, uint go_uid);
	bool OnLoad(JSONConfig data);
	bool IsAlphaTest()const;
	float GetAlphaValue()const;
	ShaderProgram* GetShader() const;

	//shaders
	bool sample_shader = false;
	bool own_shader = false;

private:

	bool alpha_test = false;
	float alpha_value = 0.0f;

	uint tex_id = 0;

	std::string tex_name = "none";
	uint tex_width = 0;
	uint tex_height = 0;

	ResourceMaterial* material = nullptr;

	//shaders
	ShaderProgram* shader = nullptr;
};

#endif //__COMPONENT_MATERIAL_