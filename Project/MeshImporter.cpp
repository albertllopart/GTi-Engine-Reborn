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
		mesh->mesh->texCoords = new float[mesh->mesh->num_vertex * 3];
		memcpy(mesh->mesh->texCoords, aimesh->mTextureCoords[0], sizeof(float) * mesh->mesh->num_vertex * 3);
	}

	//calculate memory needed
	uint ranges[4] = { mesh->mesh->num_index, mesh->mesh->num_vertex, mesh->mesh->num_vertex, mesh->mesh->num_vertex };
	float size = sizeof(ranges);
	size += sizeof(uint) * mesh->mesh->num_index;
	size += sizeof(float) * mesh->mesh->num_vertex * 3;

	if (mesh->mesh->normals != nullptr)
	{
		size += sizeof(float) * mesh->mesh->num_vertex * 3;
	}
	if (mesh->mesh->texCoords != nullptr)
	{
		size += sizeof(float) * mesh->mesh->num_vertex * 3;
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
		bytes = sizeof(float) * mesh->mesh->num_vertex * 3;
		memcpy(pointer, mesh->mesh->texCoords, sizeof(float) * 3 * mesh->mesh->num_vertex);
		pointer += bytes;
	}

	App->filesystem->SaveFile(output_file, data, size, FILE_MESH);

	RELEASE(mesh);
	RELEASE_ARRAY(data);

	return true;
}

bool MeshImporter::Load(const char* exported_file, ComponentMesh* mesh)
{
	return true;
}