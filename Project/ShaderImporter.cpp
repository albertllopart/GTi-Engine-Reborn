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

bool ShaderImporter::Import(std::string output_file) const
{
	return false;
}

bool ShaderImporter::Load(const char * exported_file, shader_type type) const
{
	uint id = 0;
	ShaderObject* object;

	if (type == GTI_VERTEX_SHADER)
	{
		object = new ShaderObject(GTI_VERTEX_SHADER);
	}
	else if (type == GTI_FRAGMENT_SHADER)
	{
		object = new ShaderObject(GTI_FRAGMENT_SHADER);
	}

	char* buffer;
	uint size;

	bool result = App->filesystem->LoadFile(exported_file, &buffer, size, FILE_SHADER);

	std::string clean(buffer);

	clean.erase(std::remove(clean.begin(), clean.end(), '\r'), clean.end());
	clean.erase(std::remove(clean.begin(), clean.end(), 'ý'), clean.end());

	if (result)
	{
		object->data = (GLchar*)clean.c_str();
	}

	return false;
}
