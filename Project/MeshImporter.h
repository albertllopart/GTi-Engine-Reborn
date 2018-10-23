#ifndef __MeshImporter__
#define __MeshImporter__

#include "Importer.h"
#include "Globals.h"

class MeshImporter : Importer
{
public:
	MeshImporter(importer_type type);
	~MeshImporter();

	bool Import(const char* file, const char* path, std::string& output_file);
	bool Import(const void* buffer, uint size, std::string& output_file);
	bool Load(const char* exported_file);
	bool LoadCheckers();
};

#endif