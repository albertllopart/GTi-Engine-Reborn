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

	mesh->num_vertex = aimesh->mNumVertices;
	mesh->vertex = new float[mesh->num_vertex * 3];
	memcpy(mesh->vertex, aimesh->mVertices, sizeof(float) * mesh->num_vertex * 3);

	if (aimesh->HasFaces())
	{
		mesh->num_index = aimesh->mNumFaces * 3;
		mesh->index = new uint[mesh->num_index];

		for (int i = 0; i < aimesh->mNumFaces; i++)
		{
			if (aimesh->mFaces[i].mNumIndices != 3)
			{
				LOG("WARNING, geometry face with != 3 indices!");
			}
			else
			{
				memcpy(&mesh->index[i * 3], aimesh->mFaces[i].mIndices, 3 * sizeof(uint));
			}
		}
	}

	if (aimesh->HasNormals())
	{
		mesh->normals = new float[mesh->num_vertex * 3];
		memcpy(mesh->normals, aimesh->mNormals, sizeof(float) * mesh->num_vertex * 3);
	}

	if (aimesh->HasTextureCoords(0))
	{
		mesh->texCoords = new float[mesh->num_vertex * 3];
		memcpy(mesh->texCoords, aimesh->mTextureCoords[0], sizeof(float) * mesh->num_vertex * 3);
	}

	//calculate memory needed
	uint ranges[4] = { mesh->num_index, mesh->num_vertex, mesh->num_vertex, mesh->num_vertex };
	float size = sizeof(ranges);
	size += sizeof(uint) * mesh->num_index;
	size += sizeof(float) * mesh->num_vertex * 3;

	if (mesh->normals != nullptr)
	{
		size += sizeof(float) * mesh->num_vertex * 3;
	}
	if (mesh->texCoords != nullptr)
	{
		size += sizeof(float) * mesh->num_vertex * 3;
	}

	char* data = new char[size];
	char* pointer = data;

	uint bytes = sizeof(ranges);
	memcpy(pointer, ranges, bytes);
	pointer += bytes;

	//indices
	bytes = sizeof(uint) * mesh->num_index;
	memcpy(pointer, mesh->index, sizeof(uint) * mesh->num_index);
	pointer += bytes;

	//vertices
	bytes = sizeof(float) * mesh->num_vertex * 3;
	memcpy(pointer, mesh->vertex, sizeof(float) * 3 * mesh->num_vertex);
	pointer += bytes;

	//normals
	if (mesh->normals != nullptr)
	{
		bytes = sizeof(float) * mesh->num_vertex * 3;
		memcpy(pointer, mesh->normals, sizeof(float) * 3 * mesh->num_vertex);
		pointer += bytes;
	}

	//texcoords
	if (mesh->texCoords != nullptr)
	{
		bytes = sizeof(float) * mesh->num_vertex * 3;
		memcpy(pointer, mesh->texCoords, sizeof(float) * 3 * mesh->num_vertex);
		pointer += bytes;
	}

	App->filesystem->SaveFile(output_file, data, size, FILE_MESH);
	delete mesh;
	delete[] data;
	data = nullptr;

	return true;
}

bool MeshImporter::Load(const char* exported_file, ComponentMesh* mesh)
{
	return true;
}