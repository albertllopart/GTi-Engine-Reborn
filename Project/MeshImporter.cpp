#include "Application.h"
#include "Importer.h"
#include "MeshImporter.h"

#include "ModuleFileSystem.h"

#include "Glew\include\glew.h"
#include "Assimp\include\cimport.h" 
#include "Assimp\include\scene.h" 
#include "Assimp\include\postprocess.h" 
#include "Assimp\include\cfileio.h"

#pragma comment (lib, "Assimp/libx86/assimp.lib")

MeshImporter::MeshImporter(importer_type type) : Importer(type)
{

}

MeshImporter::~MeshImporter()
{

}

bool MeshImporter::Import(const aiMesh* aimesh, std::string output_file)
{
	ComponentMesh* mesh = new ComponentMesh();

	mesh->mesh->num_vertex = aimesh->mNumVertices;
	mesh->mesh->vertex = new float[mesh->mesh->num_vertex * 3];
	memcpy(mesh->mesh->vertex, aimesh->mVertices, sizeof(float) * mesh->mesh->num_vertex * 3);

	if (aimesh->HasFaces())
	{
		mesh->mesh->num_index = aimesh->mNumFaces * 3;
		mesh->mesh->index = new uint[mesh->mesh->num_index];

		for (int i = 0; i < aimesh->mNumFaces; i++)
		{
			if (aimesh->mFaces[i].mNumIndices != 3)
			{
				LOG("WARNING, geometry face with != 3 indices!");
			}
			else
			{
				memcpy(&mesh->mesh->index[i * 3], aimesh->mFaces[i].mIndices, 3 * sizeof(uint));
			}
		}
	}

	if (aimesh->HasNormals())
	{
		mesh->mesh->normals = new float[mesh->mesh->num_vertex * 3];
		memcpy(mesh->mesh->normals, aimesh->mNormals, sizeof(float) * mesh->mesh->num_vertex * 3);
	}

	if (aimesh->HasTextureCoords(0))
	{
		mesh->mesh->texCoords = new float[mesh->mesh->num_vertex * 2];
		for (int i = 0; i < aimesh->mNumVertices; i++)
		{
			memcpy(&mesh->mesh->texCoords[i * 2], &aimesh->mTextureCoords[0][i].x, sizeof(float));
			memcpy(&mesh->mesh->texCoords[(i * 2) + 1], &aimesh->mTextureCoords[0][i].y, sizeof(float));
		}
	}

	//calculate memory needed
	
	uint ranges[4] = { mesh->mesh->num_index, mesh->mesh->num_vertex, 0, 0 };
	if (aimesh->HasNormals())
	{
		ranges[2] = mesh->mesh->num_vertex;
	}
	if (aimesh->HasTextureCoords(0))
	{
		ranges[3] = mesh->mesh->num_vertex;
	}

	float size = sizeof(ranges);
	size += sizeof(uint) * mesh->mesh->num_index;
	size += sizeof(float) * mesh->mesh->num_vertex * 3;

	if (mesh->mesh->normals != nullptr)
	{
		size += sizeof(float) * mesh->mesh->num_vertex * 3;
	}
	if (mesh->mesh->texCoords != nullptr)
	{
		size += sizeof(float) * mesh->mesh->num_vertex * 2;
	}

	char* data = new char[size];
	char* pointer = data;

	uint bytes = sizeof(ranges);
	memcpy(pointer, ranges, bytes);
	pointer += bytes;

	//indices
	bytes = sizeof(uint) * mesh->mesh->num_index;
	memcpy(pointer, mesh->mesh->index, sizeof(uint) * mesh->mesh->num_index);
	pointer += bytes;

	//vertices
	bytes = sizeof(float) * mesh->mesh->num_vertex * 3;
	memcpy(pointer, mesh->mesh->vertex, sizeof(float) * 3 * mesh->mesh->num_vertex);
	pointer += bytes;

	//normals
	if (mesh->mesh->normals != nullptr)
	{
		bytes = sizeof(float) * mesh->mesh->num_vertex * 3;
		memcpy(pointer, mesh->mesh->normals, sizeof(float) * 3 * mesh->mesh->num_vertex);
		pointer += bytes;
	}

	//texcoords
	if (mesh->mesh->texCoords != nullptr)
	{
		bytes = sizeof(float) * mesh->mesh->num_vertex * 2;

		for (int i = 0; i < mesh->mesh->num_vertex; i++)
		{
			memcpy(&pointer[i * 2 * sizeof(float)], &aimesh->mTextureCoords[0][i].x, sizeof(float));
			memcpy(&pointer[i * 2 * sizeof(float) + sizeof(float)], &aimesh->mTextureCoords[0][i].y, sizeof(float));
		}

		pointer += bytes;
	}

	App->filesystem->SaveFile(output_file, data, size, FILE_MESH);

	RELEASE(mesh);
	RELEASE_ARRAY(data);

	return true;
}

bool MeshImporter::Load(const char* exported_file, ComponentMesh* mesh)
{
	char* buffer;
	uint size;

	//TODO: loadFile needs to receive ONLY the name of the file with no extension nor directories
	bool result = App->filesystem->LoadFile(exported_file, &buffer, size, FILE_MESH);

	if (result)
	{
		char* pointer = buffer;

		uint ranges[4];
		uint bytes = sizeof(ranges);
		memcpy(ranges, pointer, bytes);

		mesh->mesh->num_index = ranges[0];
		mesh->mesh->num_vertex = ranges[1];

		//indices
		pointer += bytes;
		bytes = sizeof(uint) * mesh->mesh->num_index;
		mesh->mesh->index = new uint[mesh->mesh->num_index];
		memcpy(mesh->mesh->index, pointer, bytes);

		glGenBuffers(1, (GLuint*)&mesh->mesh->id_index);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->mesh->id_index);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * mesh->mesh->num_index, mesh->mesh->index, GL_STATIC_DRAW);

		//vertices
		pointer += bytes;
		bytes = sizeof(float) * 3 * mesh->mesh->num_vertex;
		mesh->mesh->vertex = new float[mesh->mesh->num_vertex * 3];
		memcpy(mesh->mesh->vertex, pointer, bytes);

		glGenBuffers(1, (GLuint*)&mesh->mesh->id_vertex);
		glBindBuffer(GL_ARRAY_BUFFER, mesh->mesh->id_vertex);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mesh->mesh->num_vertex * 3, mesh->mesh->vertex, GL_STATIC_DRAW);

		//normals
		if (ranges[2] > 0)
		{
			pointer += bytes;
			bytes = sizeof(float) * 3 * mesh->mesh->num_vertex;
			mesh->mesh->normals = new float[mesh->mesh->num_vertex * 3];
			memcpy(mesh->mesh->normals, pointer, bytes);

			glGenBuffers(1, (GLuint*) &(mesh->mesh->id_normals));
			glBindBuffer(GL_ARRAY_BUFFER, mesh->mesh->id_normals);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mesh->mesh->num_vertex * 3, mesh->mesh->normals, GL_STATIC_DRAW);
		}

		//texcoords
		if (ranges[3] > 0)
		{
			pointer += bytes;
			bytes = sizeof(float) * 2 * mesh->mesh->num_vertex;
			mesh->mesh->texCoords = new float[mesh->mesh->num_vertex * 2];

			for (int i = 0; i < mesh->mesh->num_vertex; i++)
			{
				memcpy(&mesh->mesh->texCoords[i * 2], &pointer[i * 2 * sizeof(float)], sizeof(float));
				memcpy(&mesh->mesh->texCoords[(i * 2) + 1], &pointer[i * 2 * sizeof(float) + sizeof(float)], sizeof(float));
			}

			glGenBuffers(1, (GLuint*) &(mesh->mesh->id_texcoord));
			glBindBuffer(GL_ARRAY_BUFFER, mesh->mesh->id_texcoord);
			glBufferData(GL_ARRAY_BUFFER, sizeof(float) * mesh->mesh->num_vertex * 2, mesh->mesh->texCoords, GL_STATIC_DRAW);
	
		}
	}

	mesh->mesh->bbox.SetNegativeInfinity();
	mesh->mesh->bbox.Enclose((float3*)mesh->mesh->vertex, mesh->mesh->num_vertex);

	RELEASE_ARRAY(buffer);

	return result;
}