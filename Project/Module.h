#ifndef __MODULE__
#define __MODULE__

#include "parson/parson.h"

class Application;

class Module
{
private :

	bool enabled;

public:
	Application* App;

	Module(Application* parent, bool start_enabled = true) : App(parent)
	{}

	virtual ~Module()
	{}

	virtual bool Init(JSON_Object* node) 
	{
		return true; 
	}

	virtual bool Start()
	{
		return true;
	}

	virtual update_status PreUpdate(float dt)
	{
		return UPDATE_CONTINUE;
	}

	virtual update_status Update(float dt)
	{
		return UPDATE_CONTINUE;
	}

	virtual update_status PostUpdate(float dt)
	{
		return UPDATE_CONTINUE;
	}

	virtual bool CleanUp() 
	{ 
		return true; 
	}

	virtual void Save(JSON_Object* node)
	{};
	virtual void Load(JSON_Object* node)
	{};

public:

	char* name = nullptr;

};

#endif // __MODULE__