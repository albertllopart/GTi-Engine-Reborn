#ifndef __ModuleRNG_H__
#define __ModuleRNG_H__

#include "Module.h"
#include "pcg-c-0.94/include/pcg_variants.h"
#include <list>

#include "Primitive.h"

class ModuleRNG : public Module
{
public:

	ModuleRNG(Application* app, bool start_enabled = true);
	~ModuleRNG();

	bool Init(JSON_Object* node);
	bool Start();
	bool CleanUp();

	update_status Update(float dt);

	float RandomFloat();
	uint32_t RandomInt(int min, int max);
	uint32_t Random32();

private:

	pcg32_random_t floatseed;
	pcg32_random_t intbound;
	pcg32_random_t seed;
	
};

#endif // __ModuleRNG_H__