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

std::string ShaderImporter::Load(const char * exported_file, shader_type type) const
{
	char* buffer;
	uint size = 0;

	if (type == GTI_VERTEX_SHADER)
	{
		App->filesystem->LoadFile(exported_file, &buffer, size, FILE_VERTEX_SHADER);
	}
	else if (type == GTI_FRAGMENT_SHADER)
	{
		App->filesystem->LoadFile(exported_file, &buffer, size, FILE_FRAG_SHADER);
	}

	std::string clean(buffer);

	clean.erase(std::remove(clean.begin(), clean.end(), '\r'), clean.end());
	clean.erase(std::remove(clean.begin(), clean.end(), 'ý'), clean.end());

	return clean;
}

std::string ShaderImporter::NewShaderFile(const char * name, shader_type type) const
{
	char* buffer;
	std::string ret("error");

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

		ret = size_calc;
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

		ret = size_calc;
	}

	return ret;
}

bool ShaderImporter::SaveShaderFile(const char * name, std::string buffer, shader_type type) const
{
	bool ret = false;

	for (int i = 0; i < buffer.length(); i++)
	{
		if (buffer[i] == '\n')
		{
			buffer.insert(buffer.begin() + i, '\r');
			i++;
		}
	}

	uint size = buffer.length();

	if (type == GTI_VERTEX_SHADER)
	{
		ret = App->filesystem->SaveFile(name, (char*)buffer.c_str(), size, file_type::FILE_VERTEX_SHADER);
	}
	else if (type == GTI_FRAGMENT_SHADER)
	{
		ret = App->filesystem->SaveFile(name, (char*)buffer.c_str(), size, file_type::FILE_FRAG_SHADER);
	}

	return ret;
}


