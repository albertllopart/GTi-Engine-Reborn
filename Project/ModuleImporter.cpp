#include "ModuleImporter.h"
#include "Application.h"

#include "Assimp/include/cimport.h" 
#include "Assimp/include/scene.h" 
#include "Assimp/include/postprocess.h" 
#include "Assimp/include/cfileio.h"
#include "Glew/include/glew.h"

#pragma comment (lib, "Assimp/libx86/assimp.lib")



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
	stream = aiGetPredefinedLogStream(aiDefaultLogStream_DEBUGGER, nullptr);
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

			if (newMesh->HasFaces())
			{
				mesh->num_index = newMesh->mNumFaces * 3;
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
			}
			if (newMesh->HasNormals())
			{
				mesh->normals = new float[mesh->num_vertex * 3];
				memcpy(mesh->normals, newMesh->mNormals, sizeof(float) * mesh->num_vertex * 3);

				glGenBuffers(1, (GLuint*)&mesh->id_normals);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->id_normals);
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(float) * mesh->num_vertex * 3, mesh->normals,  GL_STATIC_DRAW);
			}


			//else
			//{
			//	glGenBuffers(1, &m->id_vertex);
			//	glBindBuffer(GL_ARRAY_BUFFER, m->id_vertex);
			//	glBufferData(GL_ARRAY_BUFFER, sizeof(m->vertex), m->vertex, GL_STATIC_DRAW);
			//}
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
