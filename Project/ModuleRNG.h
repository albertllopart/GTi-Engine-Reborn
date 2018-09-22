#ifndef __ModuleRNG_H__
#define __ModuleRNG_H__

#include "Module.h"
#include "pcg-c-0.94/include/pcg_variants.h"
#include <list>

class ModuleRNG : public Module
{
public:

	ModuleRNG(Application* app, bool start_enabled = true);
	~ModuleRNG();

	bool Init();
	bool CleanUp();

	float RandomFloat() const;
	int RandomInt(int min, int max) const;

private:

	
};

#endif // __ModuleRNG_H__