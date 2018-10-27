#include "ModuleImporter.h"
#include "Application.h"
#include "ModuleImGui.h"
#include "GameObject.h"
#include "ComponentMesh.h"
#include "Assimp/include/cimport.h" 
#include "Assimp/include/scene.h" 
#include "Assimp/include/postprocess.h" 
#include "Assimp/include/cfileio.h"
#include "Glew/include/glew.h"
#include "ComponentTransform.h"
#include "ComponentMaterial.h"

#pragma comment (lib, "Assimp/libx86/assimp.lib")

void AssimpCallback(const char* msg, char* userData)
{
	//LOG("%s", msg);
	if (App != nullptr)
	{
		std::string log = msg;
		App->imgui->AddConsoleLog(log);
	}
}

ModuleImporter::ModuleImporter(Application * app, bool start_enabled) : Module(app, start_enabled)
{
	name = "importer";
}

ModuleImporter::~ModuleImporter()
{
}

bool ModuleImporter::Init(JSON_Object* data)
{
	importer = new MeshImporter(IMPORTER_MESH);

	struct aiLogStream stream;
	stream.callback = AssimpCallback;
	aiAttachLogStream(&stream);

	return true;
}

bool ModuleImporter::CleanUp(JSON_Object* data)
{
	RELEASE(importer);

	aiDetachAllLogStreams();
	return true;
}

ComponentMesh* ModuleImporter::LoadMesh(const char* fullPath)
{
	ComponentMesh* mesh = new ComponentMesh;
	
	importer->Load(fullPath, mesh);

	return mesh;
}

bool ModuleImporter::ImportMesh(const char* fullPath)
{
	const aiScene* scene = aiImportFile(fullPath, aiProcessPreset_TargetRealtime_MaxQuality);
	if (scene != nullptr && scene->HasMeshes())
	{
		aiNode* node = scene->mRootNode;

		for (int i = 0; i < scene->mNumMeshes; i++)
		{
			importer->Import(scene->mMeshes[i], "testing1234");
		}

		aiReleaseImport(scene);
		return true;
	}
	else
	{
		LOG("Error loading scene %s", fullPath);
		return false;
	}
}

GameObject * ModuleImporter::LoadGameObject(const char * fullPath)
{
	GameObject* newObject = new GameObject();

	uint size = strlen(fullPath);

	std::string namePath = fullPath;
	uint i = namePath.find_last_of("\\");
	char* temp = new char[size - i];
	namePath.copy(temp, size - i, i);
	newObject->name.assign(temp);

	newObject->AddComponent(LoadMesh(fullPath));

	return newObject;
}

ComponentTransform * ModuleImporter::LoadTransform(aiNode * node)
{
	aiVector3D translation;
	aiVector3D scale;
	aiQuaternion rotate;
	node->mTransformation.Decompose(scale, rotate, translation);

	float3 pos(translation.x, translation.y, translation.z);
	float3 sca(scale.x, scale.y, scale.z);
	Quat rot(rotate.x, rotate.y, rotate.z, rotate.w);

	return new ComponentTransform(pos, sca, rot);
}

ComponentMaterial* ModuleImporter::LoadMaterial(aiMaterial* drop)
{
	ComponentMaterial* mat = new ComponentMaterial();

	//MATERIAL
	if (drop != nullptr)
	{
		uint numTextures = drop->GetTextureCount(aiTextureType_DIFFUSE);
		aiString path;
		drop->GetTexture(aiTextureType_DIFFUSE, 0, &path);
		std::string fullPath = "Assets/";
		fullPath.append(path.C_Str());
		mat->SetID(App->textures->ImportImage(fullPath.c_str()));
		mat->SetTextureName(path.C_Str());

		return mat;
	}
	return nullptr;
}





