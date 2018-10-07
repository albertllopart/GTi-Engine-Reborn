#ifndef __ModuleImporter_H__
#define __ModuleImporter_H__

#include "Globals.h"
#include "Module.h"

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