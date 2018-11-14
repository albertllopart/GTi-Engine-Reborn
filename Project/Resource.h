#ifndef __RESOURCE__
#define __RESOURCE__


#include "Globals.h"
#include <string>

enum ResourceType
{
	RESOURCE_NONE,
	RESOURCE_MESH,
	RESOURCE_MATERIAL
	// Resource scene?
};

class Resource
{
public:
	Resource();
	~Resource();
	
	ResourceType GetType() const;

	//virtual void    Save() const;
	//virtual void    Load();

private:
	std::string file;
	std::string exported_file;

	ResourceType type = RESOURCE_NONE;

	uint loaded = 0;
	uint UID = 0;
};

#endif // !__RESOURCE__
