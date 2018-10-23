#ifndef __ModuleFileSystem_H__
#define __ModuleFileSystem_H__

#include "Globals.h"
#include "Module.h"

enum file_type
{
	FILE_NONE,
	FILE_MESH,
	FILE_MATERIAL,
};

class ModuleFileSystem : public Module
{
public:
	ModuleFileSystem(Application* app, bool startEnabled = true);
	~ModuleFileSystem();

	bool Init(JSON_Object* node);

	bool CreateNewDirectory(const char* name) const;
	bool SaveFile(const char* name, char* buffer, int buffer_size, file_type type);
	bool LoadFile(const char* name, char** buffer, uint& size, file_type type);
};

#endif