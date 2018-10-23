#ifndef __MaterialImporter__
#define __MaterialImporter__

#include "Importer.h"
#include "Globals.h"

class MaterialImporter : Importer
{
public:
	MaterialImporter(importer_type type);
	~MaterialImporter();

	bool Import(const char* file, const char* path, std::string& output_file);
	bool Import(const void* buffer, uint size, std::string& output_file);
	bool Load(const char* exported_file);
	bool LoadCheckers();
};

#endif