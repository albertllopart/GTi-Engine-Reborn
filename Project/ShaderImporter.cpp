#include "ShaderImporter.h"
#include "Application.h"
#include <algorithm>

#include "Glew\include\glew.h"

ShaderImporter::ShaderImporter(importer_type type) : Importer(type)
{

}

ShaderImporter::~ShaderImporter()
{
}

bool ShaderImporter::Import(const char* source, std::string output_file) const
{
	char* buffer;
	uint size;

	//bool result = App->filesystem->LoadFile(source, &buffer, size, FILE_SHADER);

	std::string clean(buffer);

	clean.erase(std::remove(clean.begin(), clean.end(), '\r'), clean.end());
	clean.erase(std::remove(clean.begin(), clean.end(), 'ý'), clean.end());

	char* clean_char = (char*)clean.c_str();
	size = clean.length();

	//App->filesystem->SaveFile(output_file, clean_char, size, FILE_SHADER);

	return true;
}

bool ShaderImporter::Load(const char * exported_file, shader_type type) const
{
	uint id = 0;
	ShaderObject* object;
	bool result = false;

	char* buffer;
	uint size = 0;

	if (type == GTI_VERTEX_SHADER)
	{
		object = new ShaderObject(GTI_VERTEX_SHADER);
		result = App->filesystem->LoadFile(exported_file, &buffer, size, FILE_VERTEX_SHADER);
	}
	else if (type == GTI_FRAGMENT_SHADER)
	{
		object = new ShaderObject(GTI_FRAGMENT_SHADER);
		result = App->filesystem->LoadFile(exported_file, &buffer, size, FILE_FRAG_SHADER);
	}

	std::string clean(buffer);

	clean.erase(std::remove(clean.begin(), clean.end(), '\r'), clean.end());
	clean.erase(std::remove(clean.begin(), clean.end(), 'ý'), clean.end());

	if (result)
	{
		object->data = (GLchar*)clean.c_str();
	}

	return true;
}

bool ShaderImporter::NewShaderFile(const char * name, shader_type type) const
{
	char* buffer;

	if (type == GTI_VERTEX_SHADER)
	{
		buffer = (char*)App->renderer3D->shaders_manager->def_vertex_shader;

		std::string str_name(name);
		std::string size_calc(buffer);

		for (int i = 0; i < size_calc.length(); i++)
		{
			if (size_calc[i] == '\n')
			{
				size_calc.insert(size_calc.begin() + i, '\r');
				i++;
			}
		}

		uint size = size_calc.length();

		App->filesystem->SaveFile(str_name, (char*)size_calc.c_str(), size, file_type::FILE_VERTEX_SHADER);
	}
	else if (type == GTI_FRAGMENT_SHADER)
	{
		buffer = (char*)App->renderer3D->shaders_manager->def_frag_shader;

		std::string str_name(name);
		std::string size_calc(buffer);

		for (int i = 0; i < size_calc.length(); i++)
		{
			if (size_calc[i] == '\n')
			{
				size_calc.insert(size_calc.begin() + i, '\r');
				i++;
			}
		}

		uint size = size_calc.length();

		App->filesystem->SaveFile(str_name, (char*)size_calc.c_str(), size, file_type::FILE_FRAG_SHADER);
	}

	return true;
}


