#ifndef __TextureImporter__
#define __TextureImporter__

#include "Importer.h"
#include "Globals.h"

class TextureImporter : Importer
{
public:
	TextureImporter(Application* app, bool start_enabled = true);
	~TextureImporter();

	bool Import(const char* file, const char* path, std::string& output_file);
	bool Import(const void* buffer, uint size, std::string& output_file);
	bool Load(const char* exported_file);
	bool LoadCheckers();
};

#endif