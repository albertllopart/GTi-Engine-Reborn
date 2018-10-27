#ifndef __ENGINE_HIERARCHY
#define __ENGINE_HIERARCHY
#include "E_Windows.h"

class GameObject;
class E_Hierarchy : public E_Windows
{

public:

	E_Hierarchy(ModuleEngineWindows* mom);
	~E_Hierarchy();

	bool Draw();
	void CleanUp();
	void SetSceneGameObject(GameObject* to_set);

public :
	GameObject* scene = nullptr;


};
#endif // !__ENGINE_HIERARCHY
