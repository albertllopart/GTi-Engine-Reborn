#ifndef __COMPONENT_MATERIAL_
#define __COMPONENT_MATERIAL_

#include "Component.h"
#include "Globals.h"

class ComponentMaterial : public Component
{
	ComponentMaterial(GameObject* my_go);
	~ComponentMaterial();

	void OnEditor();
	void LoadTexture(const char* path);

private:

	uint tex_id = 0;

};

#endif //__COMPONENT_MATERIAL_