#include "ModuleResourceManager.h"
#include "Application.h"
#include "ModuleSceneEditor.h"
#include "ModuleRNG.h"
#include "ModuleTextures.h"
#include "ModuleImporter.h"




ModuleResourceManager::ModuleResourceManager(Application* app, bool start_enabled) : Module (app, start_enabled)
{
	name = "ModuleResourceManager";
}

ModuleResourceManager::~ModuleResourceManager()
{
}

bool ModuleResourceManager::Start()
{
	return true;
}

bool ModuleResourceManager::CleanUp()
{
	return true;
}

update_status ModuleResourceManager::PreUpdate(float dt)
{
	if (want_delete)
	{
		RemoveResources();
	}

	return UPDATE_CONTINUE;
}

update_status ModuleResourceManager::Update(float dt)
{
	time_update += dt;

	if (time_update > UPDATE_RESOURCES)
	{
		SearchResources(); //look every X seconds assets folder to search changes
	}
	return UPDATE_CONTINUE;
}

uint ModuleResourceManager::ResourceWindow(ResourceType type)
{
	return uint();
}

uint ModuleResourceManager::ImportFile(const char * file_path)
{
	bool import_success = false;

	std::string asset = file_path;
	std::size_t found = asset.find_last_of('.');

	ResourceType type = GetResourceFromFile(file_path);

	uint UID = App->rng->Random32();
	std::string name = std::to_string(UID);

	switch (type)
	{
	case RESOURCE_NONE:
		break;
	case RESOURCE_MATERIAL:
		//import_success = App->textures->ImportImage(file_path);
		break;
	case RESOURCE_MESH:
		//import_success = App->import->ImportMesh(file_path, name.c_str());
		break;
	default:
		break;
	}

	if (import_success)
	{
		//CreateResource
	}

	return 0;
}

Resource * ModuleResourceManager::CreateResource(ResourceType type, uint UID)
{
	Resource* ret = nullptr;
	//uint UID = App->rng->Random32();
	switch (type)
	{
	case RESOURCE_NONE:
		break;
	case RESOURCE_MATERIAL:
		//ret = new ResourceTexture(UID);
		break;
	case RESOURCE_MESH:
		//ret = new ResourceMesh(UID);
		break;
	default:
		break;
	}
	if (ret != nullptr)
	{
		resources[UID] = ret;
	}
	return ret;
}

void ModuleResourceManager::SearchResources()
{
//search for resources 

}

void ModuleResourceManager::RemoveResources()
{
}

ResourceType ModuleResourceManager::GetResourceFromFile(const char * file) const
{
	std::string extension_check = file;
	std::size_t found = extension_check.find_last_of('.');
	std::string extension = extension_check.substr(found + 1);

	if (_stricmp(extension.c_str(), "png") == 0
		|| _stricmp(extension.c_str(), "jpg") == 0
		|| _stricmp(extension.c_str(), "dds") == 0
		|| _stricmp(extension.c_str(), "tga") == 0)
	{
		return RESOURCE_MATERIAL;
	}
	else if (_stricmp(extension.c_str(), "obj") == 0
		|| _stricmp(extension.c_str(), "fbx") == 0)
	{
		return RESOURCE_MESH;
	}
	else
	{
		return RESOURCE_NONE;
	}
	//scene?¿
}
