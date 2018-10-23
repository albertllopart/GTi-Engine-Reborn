#include "Application.h"
#include "Importer.h"
#include "MeshImporter.h"

MeshImporter::MeshImporter(importer_type type) : Importer(type)
{

}

MeshImporter::~MeshImporter()
{

}

bool MeshImporter::Import(const char* file, const char* path, std::string& output_file)
{
	return true;
}

bool MeshImporter::Import(const void* buffer, uint size, std::string& output_file)
{
	return true;
}

bool MeshImporter::Load(const char* exported_file)
{
	return true;
}

bool MeshImporter::LoadCheckers()
{
	return true;
}