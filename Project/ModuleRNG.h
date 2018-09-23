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

	float RandomFloat();
	int RandomInt(int min, int max);

private:

	pcg32_random_t floatseed;
	pcg_state_setseq_64 intbound;
};

#endif // __ModuleRNG_H__