#ifndef __JSONConfig__
#define __JSONConfig__

#include "Globals.h"
#include "Application.h"

struct json_object_t;
typedef struct json_object_t JSON_Object;

struct json_array_t;
typedef struct json_array_t JSON_Array;

struct json_value_t;
typedef struct json_value_t  JSON_Value;

class JSONConfig
{
public:

	JSONConfig();
	JSONConfig(JSON_Object* object);
	~JSONConfig();

	bool ParseFile(const char* path);

	JSONConfig SetFocus(const char* name);

	JSONConfig SetFocusArray(const char* name, uint index) const;

	//getters

	int GetInt(const char* name) const;
	bool GetBool(const char* name) const;
	const char* GetString(const char* name)const;

	uint GetArraySize(const char* name)const;

	//setters

	void SetInt(int value, const char* name);
	void SetBool(bool value, const char* name);
	void SetString(std::string value, const char* name);

	void OpenArray(const char* name);
	void CloseArray(const JSONConfig& child);

	//save
	bool Save(const char* name);

private:

	JSON_Object* object = nullptr;
	JSON_Array* array = nullptr;
	JSON_Value* value = nullptr;
};

#endif