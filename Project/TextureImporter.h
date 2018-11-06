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

	bool Import(const char* source_path, std::string output_file) const;
	uint Load(const char* file_name);

	uint last_tex_width = 0;
	uint last_tex_height = 0;
};

#endif