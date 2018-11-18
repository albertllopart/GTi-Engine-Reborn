#ifndef __RESOURCE__
#define __RESOURCE__


#include "Globals.h"
#include <string>
#include <map>

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
	Resource(uint UID, ResourceType type);
	virtual ~Resource();
	
	ResourceType GetType() const;
	uint GetUID()const;
	const char* GetFile() const;
	const char* GetExportedFile() const;

	bool GetDelete() const;

	bool IsLoaded()const;
	void LoadInMemory();
	void UnLoadInMemory();
	uint CountReferences()const;

	//virtual void    Save() const;
	//virtual void    Load();
	//virtual void	  LoadInMemory() = 0;
protected:
	std::string file;
	std::string exported_file;

	ResourceType type = RESOURCE_NONE;

	uint loaded = 0;
	uint UID = 0;
	bool to_delete = false;
};

#endif // !__RESOURCE__
