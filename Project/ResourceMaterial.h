#ifndef __RESOURCE_MATERIAL__
#define __RESOURCE_MATERIAL__

#include "Resource.h"
#include "ModuleTextures.h"


class ResourceMaterial : public Resource
{
public:
	ResourceMaterial(uint uid);
	virtual ~ResourceMaterial();

	uint GetTextureID()const;
	uint GetTextureWidth()const;
	uint GetTextureHeight()const;

	void LoadInMemory();
private:
	texture texture;

};



#endif //__RESOURCE_MATERIAL__