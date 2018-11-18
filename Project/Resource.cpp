#include "Resource.h"

Resource::Resource(uint UID, ResourceType type) : UID(UID), type(type)
{

}

Resource::~Resource()
{
}

ResourceType Resource::GetType() const
{
	return type;
}

uint Resource::GetUID() const
{
	return UID;
}

const char * Resource::GetFile() const
{
	return file.c_str();
}

const char * Resource::GetExportedFile() const
{
	return exported_file.c_str();
}

bool Resource::GetDelete() const
{
	return to_delete;
}

bool Resource::IsLoaded() const
{
	return loaded == 0;
}

void Resource::LoadInMemory()
{
	loaded++;
}

void Resource::UnLoadInMemory()
{
	loaded--;
}

uint Resource::CountReferences() const
{
	return loaded;
}

