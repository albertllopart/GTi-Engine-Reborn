#ifndef __COMPONENT_MESH__
#define __COMPONENT_MESH__

#include "Component.h"
#include "Globals.h"

class ComponentMesh : public Component
{
	ComponentMesh(GameObject* my_go);
	~ComponentMesh();


private:

	uint mesh_id = 0;

};





#endif//__COMPONENT_MESH__