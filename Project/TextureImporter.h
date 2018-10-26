#ifndef __TextureImporter__
#define __TextureImporter__

#include "Importer.h"
#include "ComponentMaterial.h"
#include "Globals.h"

class TextureImporter : Importer
{
public:
	TextureImporter(importer_type type);
	~TextureImporter();

	bool Import(const char* source_path, std::string output_file);
	bool Load(){}

};

#endif