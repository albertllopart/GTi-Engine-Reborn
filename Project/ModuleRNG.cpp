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

bool ModuleRNG::Start()
{
	random_pSphere = new pSphere();
	random_pSphere->SetPos(0, 0, 0);

	random_pCylinder = new pCylinder();
	random_pCylinder->SetPos(0, 0, 0);
}

// Called before quitting
bool ModuleRNG::CleanUp()
{
	LOG("Freeing RNG");

	return true;
}

float ModuleRNG::RandomFloat()
{
	return ldexp(pcg32_random_r(&floatseed), -32);
}

int ModuleRNG::RandomInt(int min, int max)
{
	uint bound = max - min + 1;

	return pcg32_boundedrand_r(&intbound, bound);
}


