#include "Application.h"
#include "ModuleFileSystem.h"
#include "E_Folder.h"
#include <fstream>
#include <queue>
#include <experimental/filesystem>

#define MESH_DIRECTORY "Library/Meshes"
#define MESH_EXTENSION ".GTImesh"
#define MATERIAL_DIRECTORY "Library/Materials"
#define MATERIAL_EXTENSION ".dds"
#define SCENE_DIRECTORY "Library/Scenes"
#define SCENE_EXTENSION ".GTIscene"
#define ASSETS_FOLDER "Assets"
#define ASSETS_MESHES_FOLDER "Assets/Meshes"
#define ASSETS_TEXTURE_FOLDER "Assets/Textures"
#define SHADER_DIRECTORY "Library/Shaders"
#define SHADER_EXTENSION ".GTIshader"

ModuleFileSystem::ModuleFileSystem(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	//assets = ASSETS_FOLDER;
	//a_textures = ASSETS_TEXTURE_FOLDER;
	//a_meshes = ASSETS_MESHES_FOLDER;
}

ModuleFileSystem::~ModuleFileSystem()
{

}

bool ModuleFileSystem::Init(JSON_Object* node)
{
	CreateNewDirectory("Library");
	CreateNewDirectory(MESH_DIRECTORY);
	CreateNewDirectory(MATERIAL_DIRECTORY);
	CreateNewDirectory(SCENE_DIRECTORY);
	CreateNewDirectory(ASSETS_FOLDER);
	CreateNewDirectory(ASSETS_MESHES_FOLDER);
	CreateNewDirectory(ASSETS_TEXTURE_FOLDER);
	CreateNewDirectory(SHADER_DIRECTORY);
	
	return true;
}

bool ModuleFileSystem::CreateNewDirectory(const char* name) const
{
	bool result = false;
	if (name != nullptr)
	{
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
	}
	return result;
}

bool ModuleFileSystem::SaveFile(std::string name, char* buffer, int buffer_size, file_type type)
{
	std::string path;

	switch (type)
	{
		case FILE_MESH:
		{
			path = MESH_DIRECTORY;
			path += "/";
			path += name;
			path += MESH_EXTENSION;

			break;
		}

		case FILE_MATERIAL:
		{
			path = MATERIAL_DIRECTORY;
			path += "/";
			path += name;
			path += MATERIAL_EXTENSION;

			break;
		}

		case FILE_SHADER:
		{
			path = SHADER_DIRECTORY;
			path += "/";
			path += name;
			path += SHADER_EXTENSION;

			break;
		}
	}

	std::ofstream file(path.c_str(), std::ofstream::out | std::ofstream::binary);
	//http://www.cplusplus.com/reference/fstream/ofstream/ofstream/
	file.write(buffer, buffer_size);
	file.close();

	return true;
}

bool ModuleFileSystem::SaveJSONintoOWN(JSON_Value * value, const char * name)
{
	std::string path;
	
	if (name != nullptr)
	{
		path = SCENE_DIRECTORY;
		path += "/";
		path += name;
		path += SCENE_EXTENSION;
	}

	return json_serialize_to_file(value, path.c_str());
}



bool ModuleFileSystem::LoadFile(const char* name, char** buffer, uint& size, file_type type)
{
	bool result = false;

	std::string path;

	switch (type)
	{
		case FILE_MESH:
		{
			path = MESH_DIRECTORY;
			path += "/";
			path += name;
			path += MESH_EXTENSION;

			break;
		}

		case FILE_MATERIAL:
		{
			path = MATERIAL_DIRECTORY;
			path += "/";
			path += name;
			path += MATERIAL_EXTENSION;

			break;
		}

		case FILE_SHADER:
		{
			path = SHADER_DIRECTORY;
			path += "/";
			path += name;
			path += SHADER_EXTENSION;

			break;
		}
	}
	
	std::ifstream file(path, std::ifstream::binary);
	//http://www.cplusplus.com/reference/istream/istream/read/ 

	if (file.good())
	{
		if (file.is_open())
		{
			file.seekg(0, file.end);
			std::streamsize length = file.tellg();
			file.seekg(0, file.beg);

			*buffer = new char[length];

			file.read(*buffer, length);

			if (file)
			{
				std::string add = ("File %s succesfully loaded", name);
				App->imgui->AddConsoleLog(add);
				result = true;
			}
			else
			{
				std::string add = ("Couldn't load %s", name);
				App->imgui->AddConsoleLog(add);
				result = false;
			}
			file.close();
		}
	}

	return result;
}

bool ModuleFileSystem::ListFiles(const std::string& parent_path, PathList& path_fill)
{

	std::queue<std::string> directory_queue;
	directory_queue.push(parent_path);
	while (!directory_queue.empty())
	{
		for (std::experimental::filesystem::directory_iterator::value_type item : std::experimental::filesystem::directory_iterator(directory_queue.front()))
		{
			bool directory = false;
			if (item.status().type() == std::experimental::filesystem::file_type::directory)
			{
				directory_queue.push(item.path().string().c_str());
				directory = true;
			}
			Path* new_directory = new Path(
				item.path().string().c_str(),
				item.path().filename().generic_string(),
				directory_queue.front(),
				directory);
			path_fill.list.push_back(new_directory);

		}
		directory_queue.pop();
	}
	return true;
}

bool ModuleFileSystem::RemoveFile(const char * file, bool directory)
{
	bool ret = true;
	if (directory)
	{
		std::experimental::filesystem::remove_all(file);
		return true;
	}
	std::ifstream ifile(file);
	if (ifile)
	{
		ifile.close();
		std::remove(file);
		bool fail = !std::ifstream(file);
		if (fail)
		{
			ret = false;
			LOG("Error on Removefile: Fail on remove")
		}
	}
	else
	{
		LOG("Error on RemoveFile: File don't extist");
		ret = false;
	}

	return true;
}

const char * ModuleFileSystem::GetAssetsFolder() const
{
	return ASSETS_FOLDER;
}
