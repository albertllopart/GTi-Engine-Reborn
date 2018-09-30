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
	random_pSphere = new pSphere();
	random_pSphere->SetPos(0, 0, 0);

	random_pCylinder = new pCylinder();
	random_pCylinder->SetPos(0, 0, 0);

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
	//glMatrixMode(GL_PROJECTION);
	//glLoadIdentity();
	//glBegin(GL_QUADS);
	//glColor3f(1.0f, 0.0f, 0.0f);
	//glVertex3f(-1.0f, 1.0f, -1.0f);
	//glVertex3f(1.0f, 1.0f, -1.0f);
	//glVertex3f(1.0f, 1.0f, 1.0f);
	//glVertex3f(-1.0f, 1.0f, 1.0f);   
  


	return UPDATE_CONTINUE;
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


