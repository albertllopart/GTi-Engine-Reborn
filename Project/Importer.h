#ifndef __IMPORTER__
#define __IMPORTER__

#include "Module.h"
#include "Globals.h"
#include <string>

enum importer_type
{
	IMPORTER_NONE,
	IMPORTER_MATERIAL,
	IMPORTER_MESH,
};

class Importer
{
public:

	Importer(importer_type type);
	virtual ~Importer();

	virtual bool Import(const char* file, const char* path, std::string& output_file) 
	{
		return true;
	};

	virtual bool Import(const void* buffer, uint size, std::string& output_file)
	{
		return true;
	};

	virtual bool Load(const char* exported_file)
	{
		return true;
	};

	virtual bool LoadCheckers() 
	{
		return true;
	};

private:

	std::string source_file;
	std::string exported_file;

	importer_type type = IMPORTER_NONE;
};

#endif // !__IMPORTER__
