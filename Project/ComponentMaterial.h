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

public:

	uint tex_id = 0;

	std::string tex_name = "none";
	uint tex_width = 0;
	uint tex_height = 0;
};

#endif //__COMPONENT_MATERIAL_