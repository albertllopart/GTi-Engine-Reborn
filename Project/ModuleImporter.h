#ifndef __ModuleImporter_H__
#define __ModuleImporter_H__

#include "Globals.h"
#include "Module.h"
#include "MeshImporter.h"

struct aiScene;
struct aiMesh;
struct aiMaterial;
struct aiNode;

class GameObject;

class ComponentMesh;
class ComponentMaterial;
class ComponentTransform;

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
	bool ImportMesh(const char* fullPath);


private:

public:

	MeshImporter* importer;
};

#endif //__ModuleImporter_H__