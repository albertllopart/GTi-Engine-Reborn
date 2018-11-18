#ifndef __MeshImporter__
#define __MeshImporter__

#include "Importer.h"
#include "ComponentMesh.h"
#include "Globals.h"

struct aiMesh;
struct aiScene;
struct aiNode;

class MeshImporter : Importer
{
public:
	MeshImporter(importer_type type);
	~MeshImporter();

	bool Import(const aiMesh* aimesh, std::string output_file) const;
	bool ImportNodes(const aiScene* scene, const aiNode* node, const GameObject* parent, const GameObject* transform)const;
	bool Load(const char* exported_file, ComponentMesh* mesh)const;

	bool CheckIfTransform(std::string name) const;

};

#endif