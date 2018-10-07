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
			Mesh* m = new Mesh; //our Mesh struct with all vertices and index
			m->num_vertex = newMesh->mNumVertices; //we store the number of vertex of our mesh in "m"
			m->vertex = new float[(m->num_vertex * 3)];//storing in "m", the vertices of our mesh 
			memcpy(m->vertex, newMesh->mVertices, sizeof(float)* m->num_vertex * 3); //
			LOG("New mesh with %d vertexs", m->num_vertex);
			glGenBuffers(1, (GLuint*)&m->id_vertex);
			glBindBuffer(GL_ARRAY_BUFFER, m->id_vertex);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * m->num_vertex * 3, m->vertex, GL_STATIC_DRAW);

			if (newMesh->HasFaces())
			{
				m->num_index = newMesh->mNumFaces * 3;
				m->index = new uint[m->num_index]; // GLshort
				for (uint i = 0; i < newMesh->mNumFaces; ++i)
				{
					if (newMesh->mFaces[i].mNumIndices != 3)
					{
						LOG("WARNING, geometry face with != 3 indices!");
					}
					else
					{
						memcpy(&m->index[i * 3], newMesh->mFaces[i].mIndices, 3 * sizeof(GLushort));
					}
				}
				glGenBuffers(1, &m->id_vertex);
				glBindBuffer(GL_ARRAY_BUFFER, m->id_vertex);
				glBufferData(GL_ARRAY_BUFFER, sizeof(m->vertex), m->vertex, GL_STATIC_DRAW);

				glGenBuffers(1, &m->id_index);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m->id_index);
				glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(m->index), m->index, GL_STATIC_DRAW);
			}
			else
			{
				glGenBuffers(1, &m->id_vertex);
				glBindBuffer(GL_ARRAY_BUFFER, m->id_vertex);
				glBufferData(GL_ARRAY_BUFFER, sizeof(m->vertex), m->vertex, GL_STATIC_DRAW);
			}
			App->editor->AddMesh(m);
		}
		aiReleaseImport(scene);
		return true;
	}
	else {
		LOG("Error loading scene %s", fullPath);
		return false;
	}
}
