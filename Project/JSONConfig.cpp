#include "Application.h"
#include "JSONConfig.h"
#include "parson/parson.h"

JSONConfig::JSONConfig()
{
	value = json_value_init_object();
	object = json_value_get_object(value);
}

JSONConfig::JSONConfig(JSON_Object * object)
{
	this->object = object;
}

JSONConfig::~JSONConfig()
{
	if (value)
		json_value_free(value);
}

bool JSONConfig::ParseFile(const char * path)
{
	bool ret = true;
	json_value_free(value);

	value = json_parse_file(path);

	if (value == nullptr)
		ret = false;

	object = json_value_get_object(value);

	return ret;
}

JSONConfig JSONConfig::SetFocus(const char * name)
{
	return json_object_dotget_object(object, name);
}

JSONConfig JSONConfig::SetFocusArray(const char * name, uint index) const
{
	JSONConfig ret = nullptr;
	JSON_Array* array = json_object_get_array(object, name);
	if (array != nullptr)
	{
		ret.object = json_array_get_object(array, index);
	}
	return ret;
}

//GETTERS-----------------------------------------------------------

int JSONConfig::GetInt(const char * name) const
{
	JSON_Value* value = json_object_get_value(object, name);

	return (int)json_value_get_number(value);
}

float JSONConfig::GetFloat(const char * name) const
{
	JSON_Value* value = json_object_get_value(object, name);
	
	return (float)json_value_get_number(value);
}

bool JSONConfig::GetBool(const char * name) const
{
	JSON_Value* value = json_object_get_value(object, name);

	return json_value_get_boolean(value);
}

const char * JSONConfig::GetString(const char * name) const
{
	JSON_Value* value = json_object_get_value(object, name);

	return json_value_get_string(value);
}

uint JSONConfig::GetArraySize(const char * name) const
{
	JSON_Array* array = json_object_get_array(object, name);

	return json_array_get_count(array);
}

//SETTERS-----------------------------------------------------------------

void JSONConfig::SetInt(int value, const char * name)
{
	json_object_set_number(object, name, value);
}

void JSONConfig::SetBool(bool value, const char * name)
{
	json_object_set_boolean(object, name, value);
}

void JSONConfig::SetString(std::string value, const char * name)
{
	json_object_set_string(object, name, value.c_str());
}

void JSONConfig::OpenArray(const char * name)
{
	JSON_Value* new_array = json_value_init_array();
	array = json_value_get_array(new_array);
	json_object_set_value(object, name, new_array);
}

void JSONConfig::CloseArray(const JSONConfig & child)
{
	if (array != nullptr)
	{
		json_array_append_value(array, json_value_deep_copy(child.value));
	}
	else
	{
		App->imgui->AddConsoleLog("JSON Array == nullptr");
	}
}

bool JSONConfig::Save(const char * name)
{
	return App->filesystem->SaveJSONintoOWN(value, name);
}





