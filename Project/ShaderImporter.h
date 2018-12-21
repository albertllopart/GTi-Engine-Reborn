#ifndef __ShaderImporter__
#define __ShaderImporter__

#include "Importer.h"
#include "ShaderProgramManager.h"
#include "Globals.h"

class ShaderImporter : Importer
{
public:


	ShaderImporter(importer_type type);
	~ShaderImporter();

	bool Import(const char* source, std::string output_file) const;
	bool Load(const char* exported_file, shader_type type) const;
	bool NewShaderFile(const char* name, shader_type type) const;

};

#endif