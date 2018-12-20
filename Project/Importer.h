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
	IMPORTER_SHADER,
};

class Importer
{
public:

	Importer(importer_type type);
	virtual ~Importer();

private:

	std::string source_file;
	std::string exported_file;

	importer_type type = IMPORTER_NONE;
};

#endif // !__IMPORTER__
