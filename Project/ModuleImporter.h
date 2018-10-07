#ifndef __ModuleImporter_H__
#define __ModuleImporter_H__

#include "Globals.h"
#include "Module.h"

//struct Mesh
//{
//	uint id_index = 0; // index in VRAM
//	uint num_index = 0;
//	uint* index = nullptr;
//
//	uint id_vertex = 0; // unique vertex in VRAM
//	uint num_vertex = 0;
//	float* vertex = nullptr;
//};

class ModuleImporter : public Module
{
public:
	ModuleImporter(Application* app, bool start_enabled = true);
	~ModuleImporter();

	bool Init(JSON_Object* data);
	bool CleanUp(JSON_Object* data = nullptr);
	bool LoadMesh(const char* fullPath);
};

#endif //__ModuleImporter_H__