#ifndef __ModuleImporter_H__
#define __ModuleImporter_H__

#include "Globals.h"
#include "Module.h"
#include "MeshImporter.h"
#include "ShaderImporter.h"

struct aiScene;
struct aiMesh;
struct aiMaterial;
struct aiNode;

class GameObject;

class ComponentMesh;
class ComponentMaterial;
class ComponentTransform;

enum aiPostProcess
{
	convert_to_left_handed,
	target_realtime_fast,
	target_realtime_quality,
	target_realtime_max_quality
};

class ModuleImporter : public Module
{
public:
	ModuleImporter(Application* app, bool start_enabled = true);
	~ModuleImporter();

	bool Init(JSON_Object* data);
	bool CleanUp(JSON_Object* data = nullptr);
 

	GameObject* LoadGameObject(const char* fullPath);

	ComponentTransform* LoadTransform(aiNode* node);
	ComponentMaterial * LoadMaterial(aiMaterial * drop);

	ComponentMesh* LoadMesh(const char* fullPath);
	void ShowImportMeshParameters();
	bool ImportMesh(const char* fullPath);

	std::string CleanFileName(const char* fullPath) const;


private:

public:
	std::string import_path;

	MeshImporter* importer;
	ShaderImporter* shader_importer;
	
	int aiPresset = 0;

	bool mesh_import = false;
};

#endif //__ModuleImporter_H__