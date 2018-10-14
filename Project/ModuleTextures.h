#ifndef __MODULE_TEXTURES__
#define __MODULE_TEXTURES__

#include "Module.h"
#include "Globals.h"

struct texture
{
	uint width;
	uint height;
};

class ModuleTextures : public Module
{
public:
	ModuleTextures(Application* app, bool start_enabled = true);
	~ModuleTextures();

	bool Init(JSON_Object* data);
	bool CleanUp(JSON_Object* data);

	uint ImportImage(const char * image);
	void DeleteImage(uint id);

	//last texture info
	texture last_tex;
};

#endif // __MODULE_TEXTURES__