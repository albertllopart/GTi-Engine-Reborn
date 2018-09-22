#include "Globals.h"
#include "Application.h"
#include "ModuleRNG.h"
#include <list>

ModuleRNG::ModuleRNG(Application* app, bool start_enabled) : Module(app, start_enabled)
{}

// Destructor
ModuleRNG::~ModuleRNG()
{}

// Called before render is available
bool ModuleRNG::Init()
{
	LOG("Loading RNG");
	bool ret = true;

	return ret;
}

// Called before quitting
bool ModuleRNG::CleanUp()
{
	LOG("Freeing RNG");

	return true;
}

float ModuleRNG::RandomFloat() const
{
	pcg32_random_t myrng;
	return ldexp(pcg32_random_r(&myrng), -32);
}

int ModuleRNG::RandomInt(int min, int max) const
{
	return 1;
}


