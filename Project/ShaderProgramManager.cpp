#include "ShaderProgramManager.h"
#include "Glew/include/glew.h"
#include "Application.h"

ShaderProgramManager::ShaderProgramManager()
{

}

ShaderProgramManager::~ShaderProgramManager()
{

}

bool ShaderProgramManager::LoadShaderObject(ShaderObject* object)
{
	if (object->type == GTI_FRAGMENT_SHADER)
	{
		object->id = glCreateShader(GL_FRAGMENT_SHADER);
	}
	else
	{
		object->id = glCreateShader(GL_VERTEX_SHADER);
	}

	glShaderSource(object->id, 1, &object->data, NULL);
	glCompileShader(object->id);
	int success;
	glGetShaderiv(object->id, GL_COMPILE_STATUS, &success);
	if (success == GL_FALSE)
	{
		char infoLog[512];
		glGetShaderInfoLog(object->id, 512, NULL, infoLog);
		App->imgui->AddConsoleLog(("Shader compilation error : %s", infoLog));
	}

	objects.push_back(object);

	return success;
}

bool ShaderProgramManager::UnloadShaderObject(ShaderObject* object)
{
	glDeleteShader(object->id);
	return true;
}

bool ShaderProgramManager::CreateShaderProgram()
{
	GLuint programid = glCreateProgram();
	
	for (int i = 0; i < objects.size(); i++)
	{
		glAttachShader(programid, objects[i]->id);
	}

	glLinkProgram(programid);
	int success;
	glGetProgramiv(programid, GL_LINK_STATUS, &success);
	if (!success) {
		char infoLog[512];
		glGetProgramInfoLog(programid, 512, NULL, infoLog);
		LOG("Shader link error: %s", infoLog);
	}
	else
	{
		ShaderProgram* new_program = new ShaderProgram();
		new_program->id = programid;
		programs.push_back(new_program);
	}

	return success;
}