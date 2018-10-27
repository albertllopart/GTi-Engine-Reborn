#include "E_Windows.h"
#include "ModuleEngineWindows.h"

E_Windows::E_Windows(ModuleEngineWindows * mom) : mom(mom)
{

}

E_Windows::~E_Windows()
{
	//delete mom;
	mom = nullptr;
}

bool E_Windows::Draw()
{
	return true;
}

void E_Windows::CleanUp()
{
}

bool E_Windows::IsActive() const
{
	return active;
}

void E_Windows::SetActive(bool to_set)
{
	active = to_set;
}
