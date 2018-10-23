#include "Application.h"
#include "Importer.h"
#include "MaterialImporter.h"

MaterialImporter::MaterialImporter(importer_type type) : Importer(type)
{

}

MaterialImporter::~MaterialImporter()
{

}

bool MaterialImporter::Import(const char* file, const char* path, std::string& output_file)
{
	return true;
}

bool MaterialImporter::Import(const void* buffer, uint size, std::string& output_file)
{
	return true;
}

bool MaterialImporter::Load(const char* exported_file)
{
	return true;
}

bool MaterialImporter::LoadCheckers()
{
	return true;
}