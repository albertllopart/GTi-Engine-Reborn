#ifndef __COMPONENT__
#define __COMPONENT__
#include "GameObject.h"
#include "MathGeoLib/Math/float4x4.h"
#include <string>
#include "Globals.h"

class GameObject;
class JSONConfig;

struct json_object_t;
typedef struct json_object_t JSON_Object;

struct json_value_t;
typedef struct json_value_t  JSON_Value;

enum COMPONENT_TYPE
{
	COMPONENT_NONE,
	COMPONENT_MESH,
	COMPONENT_MATERIAL,
	COMPONENT_TRANSFORM,
	COMPONENT_CAMERA
};

class Component
{
public:
	Component(COMPONENT_TYPE type);
	virtual ~Component();

	virtual void Update();
	virtual void PostUpdate();
	virtual void CleanUp();

	virtual void Enable();
	virtual void Disable();
	virtual void OnEditor();
	virtual void ShowInspectorWindow();
	virtual void OnUpdateMatrix(const math::float4x4 mat);

	GameObject* GetMyGo()const;
	bool IsActive() const;
	void SetMyGo(GameObject * my_go);
	void DestroyComponent();
	COMPONENT_TYPE GetType() const;

	void GenerateUID();
	const bool GetDelete()const;
	//save
	virtual bool OnSave(JSON_Value* array, uint go_uid);
	virtual bool OnLoad(JSONConfig data);

protected:

	uint uid = 0;
	std::string name;
	COMPONENT_TYPE type = COMPONENT_NONE;
	bool active = true;
	bool want_delete = false;
	GameObject* my_go = nullptr;
};

#endif // __COMPONENT__