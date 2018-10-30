#ifndef __ModuleSceneEditor_H__
#define __ModuleSceneEditor_H__
#include "Module.h"
#include "Globals.h"
#include "Primitive.h"

#include <list>

class GameObject;

struct json_object_t;
typedef struct json_object_t JSON_Object;

struct json_value_t;
typedef struct json_value_t  JSON_Value;

struct json_array_t;
typedef struct json_array_t JSON_Array;

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
	bool LoadScene();
	
private:
	GameObject* selected_go = nullptr;
	GameObject* root = nullptr;

	int testing = 0;
};
#endif 