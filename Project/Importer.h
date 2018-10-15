#ifndef __IMPORTER__
#define __IMPORTER__

#include "Module.h"
#include "Globals.h"

class Importer : Module
{
public:
	Importer(Application* app, bool start_enabled = true);
	~Importer();

	bool Init(JSON_Object* data);
	bool CleanUp(JSON_Object* data = nullptr);
};

#endif // !__IMPORTER__
