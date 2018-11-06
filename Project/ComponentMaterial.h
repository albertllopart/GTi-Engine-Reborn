#ifndef __COMPONENT_MATERIAL_
#define __COMPONENT_MATERIAL_

#include "Component.h"
#include "Globals.h"

class ComponentMaterial : public Component
{
public:
	ComponentMaterial();
	~ComponentMaterial();

	void OnEditor();
	void LoadTexture(const char* path);

	//void OnSave(); TODO WORK ON SAVE LOAD (G.O. & Components)
	//void OnLoad();
	
	uint GetID()const;
	void SetID(uint id);
	void SetTextureName(const char* new_name);
	void SetTextureSize(uint width, uint height);
	void ShowInspectorWindow();

private:

	uint tex_id = 0;

	std::string tex_name = "none";
	uint tex_width = 0;
	uint tex_height = 0;
};

#endif //__COMPONENT_MATERIAL_