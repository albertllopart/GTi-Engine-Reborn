#include "ResourceMesh.h"
#include "Application.h"
#include "ModuleImporter.h"

ResourceMesh::ResourceMesh(uint uid) : Resource (uid, RESOURCE_MESH)
{
}

ResourceMesh::~ResourceMesh()
{
}

void ResourceMesh::LoadInMemory()
{
	if (IsLoaded())
	{
		//App->import->LoadMesh(this);	TODO
	}
	loaded++;
}

void ResourceMesh::UnLoadFromMemory()
{

}


