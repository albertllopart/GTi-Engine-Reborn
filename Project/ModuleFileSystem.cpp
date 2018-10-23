#include "Application.h"
#include "ModuleFileSystem.h"

#include <fstream>

#define MESH_DIRECTORY "Library/Meshes"
#define MESH_EXTENSION ".meshgti"
#define MATERIAL_DIRECTORY "Library/Materials"
#define MATERIAL_EXTENSION "dds"

ModuleFileSystem::ModuleFileSystem(Application* app, bool start_enabled) : Module(app, start_enabled)
{

}

ModuleFileSystem::~ModuleFileSystem()
{

}

bool ModuleFileSystem::Init(JSON_Object* node)
{
	CreateNewDirectory("Library");
	CreateNewDirectory(MESH_DIRECTORY);
	CreateNewDirectory(MATERIAL_DIRECTORY);

	return true;
}

bool ModuleFileSystem::CreateNewDirectory(const char* name)
{
	bool result = false;
	
	result = CreateDirectory(name, NULL);

	if (result)
	{
		std::string add = ("Directory %s created", name);
		App->imgui->AddConsoleLog(add);
	}
	else
	{
		std::string add = ("Couldn't create %s directory", name);
		App->imgui->AddConsoleLog(add);
	}

	return result;
}
