#ifndef __COMPONENT__
#define __COMPONENT__
#include "GameObject.h"
#include "MathGeoLib/Math/float4x4.h"
#include <string>
#include "Globals.h"

class GameObject;

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

	virtual void Enable();
	virtual void Disable();
	virtual void OnEditor();
	virtual void ShowInspectorWindow();

	GameObject* GetMyGo()const;
	bool IsActive() const;
	void SetMyGo(GameObject * my_go);
	void OnUpdateMatrix(const float4x4 mat);
	void DestroyComponent();
	COMPONENT_TYPE GetType() const;

	//save
	virtual bool OnSave(JSON_Value* value, JSON_Object* node, uint go_uid) const;

protected:

	uint uid = 0;
	std::string name;
	COMPONENT_TYPE type = COMPONENT_NONE;
	bool active = true;
	bool want_delete = false;
	GameObject* my_go = nullptr;
};

#endif // __COMPONENT__