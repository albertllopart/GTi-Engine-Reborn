#ifndef __MeshImporter__
#define __MeshImporter__

#include "Importer.h"
#include "ComponentMesh.h"
#include "Globals.h"

struct aiMesh;

class MeshImporter : Importer
{
public:
	MeshImporter(importer_type type);
	~MeshImporter();

	bool Import(const aiMesh* aimesh, std::string output_file);
	bool Load(const char* exported_file, ComponentMesh* mesh);

};

#endif