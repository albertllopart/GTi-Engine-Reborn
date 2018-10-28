#ifndef __ENGINE_INSPECTOR__
#define __ENGINE_INSPECTOR__
#include "E_Windows.h"
#include <map>

enum COMPONENT_TYPE;
class E_Inspector : public E_Windows
{
public:

	E_Inspector(ModuleEngineWindows* my_editor);
	~E_Inspector();

	bool Draw();
	void InspectorComponents();
	void CleanUp();

private:
	std::map<COMPONENT_TYPE, const char*> comp_names;

};



#endif // !__ENGINE_INSPECTOR__
