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
	struct aiLogStream stream;
	stream.callback = AssimpCallback;
	aiAttachLogStream(&stream);

	return true;
}

bool ModuleImporter::CleanUp(JSON_Object* data)
{
	aiDetachAllLogStreams();
	return true;
}

ComponentMesh* ModuleImporter::LoadMesh(aiMesh* drop)
{
	//const aiScene* scene = aiImportFile(fullPath, aiProcessPreset_TargetRealtime_MaxQuality);
	ComponentMesh* mesh = new ComponentMesh;
	mesh->mesh->num_vertex = drop->mNumVertices;
	mesh->mesh->vertex = new float[mesh->mesh->num_vertex * 3];
	memcpy(mesh->mesh->vertex, drop->mVertices, sizeof(float)* mesh->mesh->num_vertex * 3);
	LOG("New mesh with %d vertices", mesh->mesh->num_vertex);

	glGenBuffers(1, (GLuint*)&mesh->mesh->id_vertex);
	glBindBuffer(GL_ARRAY_BUFFER, mesh->mesh->id_vertex);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mesh->mesh->num_vertex * 3, mesh->mesh->vertex, GL_STATIC_DRAW);

	if (drop->HasFaces())
	{
		mesh->mesh->num_index = drop->mNumFaces * 3;
		mesh->mesh->index = new uint[mesh->mesh->num_index];
		for (uint i = 0; i < drop->mNumFaces; ++i)
		{
			if (drop->mFaces[i].mNumIndices != 3)
			{
				LOG("WARNING, geometry face with != 3 indices!");
			}
			else
			{
				memcpy(&mesh->mesh->index[i * 3], drop->mFaces[i].mIndices, 3 * sizeof(uint));
			}
		}

		glGenBuffers(1, (GLuint*)&mesh->mesh->id_index);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->mesh->id_index);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * mesh->mesh->num_index, mesh->mesh->index, GL_STATIC_DRAW);
	}

	if (drop->HasNormals())
	{
		mesh->mesh->normals = new float[mesh->mesh->num_vertex * 3];
		memcpy(mesh->mesh->normals, drop->mNormals, sizeof(float) * mesh->mesh->num_vertex * 3);

		glGenBuffers(1, (GLuint*) &(mesh->mesh->id_normals));
		glBindBuffer(GL_ARRAY_BUFFER, mesh->mesh->id_normals);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mesh->mesh->num_vertex * 3, mesh->mesh->normals, GL_STATIC_DRAW);
	}

	if (drop->HasTextureCoords(0))

	{
		mesh->mesh->texCoords = new float[drop->mNumVertices * 2];

		for (int i = 0; i < drop->mNumVertices; i++)
		{
			memcpy(&mesh->mesh->texCoords[i * 2], &drop->mTextureCoords[0][i].x, sizeof(float));
			memcpy(&mesh->mesh->texCoords[(i * 2) + 1], &drop->mTextureCoords[0][i].y, sizeof(float));
		}
		glGenBuffers(1, (GLuint*) &(mesh->mesh->id_texcoord));
		glBindBuffer(GL_ARRAY_BUFFER, mesh->mesh->id_texcoord);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat*) * 2 * mesh->mesh->num_vertex, mesh->mesh->texCoords, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	mesh->mesh->bbox.SetNegativeInfinity();
	mesh->mesh->bbox.Enclose((float3*)mesh->mesh->vertex, mesh->mesh->num_vertex);

	return mesh;
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

	const aiScene* scene = aiImportFile(fullPath, aiProcessPreset_TargetRealtime_MaxQuality);
	if (scene != nullptr && scene->HasMeshes())
	{
		LOG("Scene %s loaded succesfully", fullPath);

		aiNode* node = scene->mRootNode;
		newObject->AddComponent(LoadTransform(node));

		for (int i = 0; i < scene->mNumMeshes; i++)
		{
			newObject->AddComponent(LoadMesh(scene->mMeshes[i]));
			aiMaterial* material = nullptr;
			material = scene->mMaterials[scene->mMeshes[i]->mMaterialIndex];

			//newObject->AddComponent(LoadMaterial(material));
		}

		aiReleaseImport(scene);
		return newObject;
	}
	else
	{
		LOG("Error loading scene %s", fullPath);
		return nullptr;
	}
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





