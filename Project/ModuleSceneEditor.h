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
	void SetToWireframe(bool wframe);

	void AddCube(float3 size, float3 pos = float3(0, 0, 0));
	void AddCube2(float3 size, float3 pos);
	void AddMesh(Mesh* model);
	void LoadTexture2AllMesh(const char* path);
	void CreateEmptyGameObject();
	void ShowRoot();
	GameObject* GetRoot();
	GameObject* CreateNewGameObject(const char* path);

	std::list<Mesh*> GetMeshList() const;
private:

	GameObject* root = nullptr;
	std::list<pCube*> cubes_list; //list of cubes mades  with vertex array
	std::list<pCube2*> indcubes_list; //list of cubes mades  with indices
	//scene meshes
	std::list<Mesh*> mesh_list;//TODO CAMBIAR AIXO A COMPONENTS
};
#endif 