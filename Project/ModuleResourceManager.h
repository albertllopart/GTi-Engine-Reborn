#ifndef __MODULE_RESOURCE_MANAGER__
#define __MODULE_RESOURCE_MANAGER__

#include "Module.h"
#include "Globals.h"
#include "Resource.h"
#include <vector>
#include <map>

#define UPDATE_RESOURCES 3.0f 

class ModuleResourceManager : public Module
{
public:
	ModuleResourceManager(Application* app, bool start_enabled = true);
	~ModuleResourceManager();

	bool Start();
	bool CleanUp();
	update_status PreUpdate(float dt);
	update_status Update(float dt);

	uint ResourceWindow(ResourceType type);
	uint ImportFile(const char* file_path);
	Resource* CreateResource(ResourceType type, uint UID = 0);
	Resource* Get(uint UID)const;
	uint Find(const char * asset_file) const;

private:
	void SearchResources();
	void RemoveResources();
	ResourceType GetResourceFromFile(const char* file_path)const;

	

	//SaveMeta
	//LoadMeta
private:
	bool want_delete = false;
	std::vector<std::string> files;
	std::map<uint, Resource*> resources;
	float time_update = 0.0f;
};


#endif //__MODULE_RESOURCE_MANAGER__