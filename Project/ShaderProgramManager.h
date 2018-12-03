#ifndef SHADER_PROGRAM_MANAGER_H_
#define SHADER_PROGRAM_MANAGER_H_

#include "Glew/include/glew.h"

class ShaderProgramManager
{
public:

	ShaderProgramManager();
	~ShaderProgramManager();

	bool LoadShaderObject(ShaderObject* object);
	bool UnloadShaderObject(ShaderObject* object);

	bool CreateShaderProgram();

public:

	std::vector<ShaderObject*> objects;
	std::list<ShaderProgram*> programs;

};

enum shader_type
{
	GTI_FRAGMENT_SHADER,
	GTI_VERTEX_SHADER
};

class ShaderObject
{
	ShaderObject(shader_type type)
	{
		this->type = type;
	}
	~ShaderObject() {};

public:

	shader_type type;
	GLuint id = 0;
	GLchar* data = nullptr;
};

class ShaderProgram
{
public:

	GLuint id = 0;

	bool UseProgram()
	{
		glUseProgram(id);
	}
};

#endif