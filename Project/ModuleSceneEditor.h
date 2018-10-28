#ifndef __ModuleSceneEditor_H__
#define __ModuleSceneEditor_H__
#include "Module.h"
#include "Globals.h"
#include "Primitive.h"

#include <list>

class GameObject;

class ModuleSceneEditor : public Module
{
public:

	ModuleSceneEditor(Application* app, bool startEnabled = true);
	~ModuleSceneEditor();

	bool Init(JSON_Object* data = nullptr);
	bool CleanUp(JSON_Object* data = nullptr);
	bool Start();
	update_status PreUpdate(float dt);
	update_status Update(float dt);
	update_status PostUpdate(float dt);

	void Draw();
	ComponentMaterial* LoadComponentMaterial(const char* path);
	GameObject* CreateEmptyGameObject(GameObject* parent);
	void ShowRoot();
	GameObject* GetRoot();
	GameObject* CreateNewGameObject(const char* path);
	void SetSelected(GameObject* to_select);
	GameObject* GetSelected()const;
	void AddToScene(GameObject* to_add);

	//save and load
	bool SaveScene() const;
	
private:
	GameObject* selected_go = nullptr;
	GameObject* root = nullptr;
};
#endif 