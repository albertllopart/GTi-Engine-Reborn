#ifndef __ModuleFileSystem_H__
#define __ModuleFileSystem_H__

#include "Globals.h"
#include "Module.h"
#include <string>

struct Path;
struct PathList;

enum file_type
{
	FILE_NONE,
	FILE_MESH,
	FILE_MATERIAL,
	FILE_SCENE,
	//FILE_TEXTURE,
	FILE_JSON,
	FILE_META,
	FILE_DIRECTORY
};

class ModuleFileSystem : public Module
{
public:
	ModuleFileSystem(Application* app, bool startEnabled = true);
	~ModuleFileSystem();

	bool Init(JSON_Object* node);

	bool CreateNewDirectory(const char* name) const;
	bool SaveFile(std::string name, char* buffer, int buffer_size, file_type type);
	bool SaveJSONintoOWN(JSON_Value* value, const char* name);
	bool LoadFile(const char* name, char** buffer, uint& size, file_type type);
	bool ListFiles(const std::string & parent_path, PathList & path_fill);
	bool RemoveFile(const char * file, bool directory);

	const char* GetAssetsFolder() const;
	

private:
	//std::string assets;
	//std::string a_textures;
	//std::string a_meshes;




};

#endif