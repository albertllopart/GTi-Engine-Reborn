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
		new_program->id_shader_prog = programid;
		programs.push_back(new_program);
	}

	return success;
}

//--------------------------------------------------------------------------------------------------------

bool ShaderProgram::CreateProgram()
{
	id_shader_prog = glCreateProgram();
	if (id_shader_prog == 0)
	{
		App->imgui->AddConsoleLog("Failed creating ShaderProgram");
		return false;
	}

	/*
	glAttachShader(id_shader_prog, vertexShader);
	glAttachShader(id_shader_prog, fragmentShader);
	glLinkProgram(id_shader_prog);
	glGetProgramiv(id_shader_prog, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(id_shader_prog, 512, NULL, infoLog);
		LOG("Shader link error: %s", infoLog);
	}
	*/
}
bool ShaderProgram::UseProgram()
{
	//ONLY ONCE COMPILED
	glUseProgram(id_shader_prog);
}