#include "Application.h"
#include "Importer.h"

Importer::Importer(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	name = "importer";
}

Importer::~Importer()
{

}

bool Importer::Init(JSON_Object* data)
{
	return true;
}

bool Importer::CleanUp(JSON_Object* data)
{
	return true;
}