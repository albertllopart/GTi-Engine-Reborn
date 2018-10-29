#include "Globals.h"
#include "Application.h"
#include "ModuleRNG.h"
#include "parson/parson.h"
#include "Glew/include/glew.h"
#include <list>

ModuleRNG::ModuleRNG(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	name = "Rng";
}

// Destructor
ModuleRNG::~ModuleRNG()
{}

// Called before render is available
bool ModuleRNG::Init(JSON_Object* node)
{
	LOG("Loading RNG");
	App->imgui->AddConsoleLog("Loading RNG");
	bool ret = true;

	return ret;
}

bool ModuleRNG::Start()
{
	pcg32_srandom_r(&seed, NULL, (intptr_t)&seed);

	return true;
}

// Called before quitting
bool ModuleRNG::CleanUp()
{
	LOG("Freeing RNG");
	App->imgui->AddConsoleLog("Freeing RNG");

	return true;
}

update_status ModuleRNG::Update(float dt)
{
	return UPDATE_CONTINUE;
}

float ModuleRNG::RandomFloat()
{
	return ldexp(pcg32_random_r(&floatseed), -32);
}

uint32_t ModuleRNG::RandomInt(int min, int max)
{
	uint bound = max - min + 1;

	return pcg32_boundedrand_r(&intbound, bound);
}

uint32_t ModuleRNG::Random32()
{
	return pcg32_random_r(&seed);
}


