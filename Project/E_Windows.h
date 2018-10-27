#ifndef __ENGINE_WINDOWS__
#define __ENGINE_WINDOWS__

#include <string>

class ModuleEngineWindows;
class E_Windows
{
public:

	E_Windows(ModuleEngineWindows* mom);
	virtual ~E_Windows();

	virtual bool Draw();
	virtual void CleanUp();

	bool IsActive() const;
	void SetActive(bool to_set);

public: 
	bool active = true;
	std::string name = "none";
protected:
	ModuleEngineWindows* mom = nullptr;

};



#endif // !__ENGINE_WINDOWS__
