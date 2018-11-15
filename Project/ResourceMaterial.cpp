#include "ResourceMaterial.h"
#include "ModuleTextures.h"

ResourceMaterial::ResourceMaterial(uint uid) : Resource(uid, RESOURCE_MATERIAL)
{
}

ResourceMaterial::~ResourceMaterial()
{
}

uint ResourceMaterial::GetTextureID() const
{
	return texture.texture_id;
}

uint ResourceMaterial::GetTextureWidth() const
{
	return texture.width;
}

uint ResourceMaterial::GetTextureHeight() const
{
	return texture.height;
}
