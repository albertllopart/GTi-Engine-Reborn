#include "Application.h"
#include "TextureImporter.h"

TextureImporter::TextureImporter(Application* app, bool start_enabled) : Importer(app, start_enabled)
{

}

TextureImporter::~TextureImporter()
{

}

bool TextureImporter::Import(const char* file, const char* path, std::string& output_file)
{
	return true;
}

bool TextureImporter::Import(const void* buffer, uint size, std::string& output_file)
{
	return true;
}

bool TextureImporter::Load(const char* exported_file)
{
	return true;
}

bool TextureImporter::LoadCheckers()
{
	return true;
}