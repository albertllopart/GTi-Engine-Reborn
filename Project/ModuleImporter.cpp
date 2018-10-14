#include "ModuleImporter.h"
#include "Application.h"
#include "ModuleImGui.h"

#include "Assimp/include/cimport.h" 
#include "Assimp/include/scene.h" 
#include "Assimp/include/postprocess.h" 
#include "Assimp/include/cfileio.h"
#include "Glew/include/glew.h"

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

bool ModuleImporter::LoadMesh(const char * fullPath)
{
	const aiScene* scene = aiImportFile(fullPath, aiProcessPreset_TargetRealtime_MaxQuality);

	if (scene != nullptr && scene->HasMeshes())
	{
		LOG("Scene %s loaded correctly", fullPath);
		// Use scene->mNumMeshes to iterate on scene->mMeshes array
		for (int i = 0; i < scene->mNumMeshes; i++)
		{
			aiMesh* newMesh = scene->mMeshes[i]; //aiMesh struct from assimp
			Mesh* mesh = new Mesh; //our Mesh struct with all vertices and index
			mesh->num_vertex = newMesh->mNumVertices; //we store the number of vertex of our mesh in "mesh"
			mesh->vertex = new float[(mesh->num_vertex * 3)]; //storing in "mesh", the vertices of our mesh 
			memcpy(mesh->vertex, newMesh->mVertices, sizeof(float)* mesh->num_vertex * 3); //
			LOG("New mesh with %d vertexs", mesh->num_vertex);
			glGenBuffers(1, (GLuint*)&mesh->id_vertex);
			glBindBuffer(GL_ARRAY_BUFFER, mesh->id_vertex);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mesh->num_vertex * 3, mesh->vertex, GL_STATIC_DRAW);
			glBindBuffer(GL_ARRAY_BUFFER, 0);

			if (newMesh->HasFaces())
			{
				mesh->num_index = newMesh->mNumFaces * 3;
				mesh->faces = newMesh->mNumFaces;
				mesh->index = new uint[mesh->num_index];
				for (uint i = 0; i < newMesh->mNumFaces; ++i)
				{
					if (newMesh->mFaces[i].mNumIndices != 3)
					{
						LOG("WARNING, geometry face with != 3 indices!");
					}
					else
					{
						memcpy(&mesh->index[i * 3], newMesh->mFaces[i].mIndices, 3 * sizeof(uint));
					}
				}

				glGenBuffers(1, (GLuint*)&mesh->id_index); // 
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->id_index);
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * mesh->num_index, mesh->index, GL_STATIC_DRAW);
				glBindBuffer(GL_ARRAY_BUFFER, 0);
			}
			if (newMesh->HasNormals())
			{
				mesh->normals = new float[mesh->num_vertex * 3];
				memcpy(mesh->normals, newMesh->mNormals, sizeof(float) * mesh->num_vertex * 3);

				glGenBuffers(1, (GLuint*)&mesh->id_normals);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->id_normals);
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(float) * mesh->num_vertex * 3, mesh->normals,  GL_STATIC_DRAW);
				glBindBuffer(GL_ARRAY_BUFFER, 0);
			}
			if (newMesh->HasTextureCoords(0))
			{
				mesh->texCoords = new float[newMesh->mNumVertices * 2];

				for (int i = 0; i < newMesh->mNumVertices; i++)
				{
					memcpy(&mesh->texCoords[i * 2], &newMesh->mTextureCoords[0][i].x, sizeof(float));
					memcpy(&mesh->texCoords[(i * 2) + 1], &newMesh->mTextureCoords[0][i].y, sizeof(float));
				}
				glGenBuffers(1, (GLuint*) &(mesh->id_texcoord));
				glBindBuffer(GL_ARRAY_BUFFER, mesh->id_texcoord);
				glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat*) * 2 * mesh->num_vertex, mesh->texCoords, GL_STATIC_DRAW);
				glBindBuffer(GL_ARRAY_BUFFER, 0);
			}

			///////////////////LOADING TRANSFORMATIONS\\\\\\\\\\\\\\\\\\\\\\\\\\|
			aiVector3D translation;
			aiVector3D scaling;
			aiQuaternion rotation;
			scene->mRootNode->mChildren[i]->mTransformation.Decompose(scaling, rotation, translation);
			mesh->pos = { translation.x, translation.y, translation.z };
			mesh->scale = { scaling.x, scaling.y, scaling.z };
			mesh->quat = { rotation.x, rotation.y, rotation.z, rotation.w };

			mesh->euler_angles = mesh->quat.ToEulerXYX();
			mesh->euler_angles.x = mesh->euler_angles.x * RADTODEG;
			mesh->euler_angles.y = mesh->euler_angles.y * RADTODEG;
			mesh->euler_angles.z = mesh->euler_angles.z * RADTODEG;

			std::string path = fullPath;
			uint position_name = path.find_last_of("\\");
			std::string newPath = path.erase(0, position_name + 1);
			mesh->name = newPath;
			App->editor->AddMesh(mesh);
		}
		aiReleaseImport(scene);
		return true;
	}
	else {
		LOG("Error loading scene %s", fullPath);
		return false;
	}
}





