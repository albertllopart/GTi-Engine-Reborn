#ifndef SHADER_PROGRAM_MANAGER_H_
#define SHADER_PROGRAM_MANAGER_H_

#include "Glew/include/glew.h"
#include <list>
#include <vector>

class ShaderObject;
class ShaderProgram;

class ShaderProgramManager
{
public:

	ShaderProgramManager();
	~ShaderProgramManager();

	bool LoadShaderObject(ShaderObject* object);
	bool UnloadShaderObject(ShaderObject* object);

	ShaderProgram* CreateShaderProgram();

	ShaderProgram * CreateDefaultShaderProgram();

public:

	std::vector<ShaderObject*> objects;
	std::list<ShaderProgram*> programs;

	const GLchar* def_vertex_shader =
		"#version 330 core \n"
		"layout(location = 0) in vec3 position;\n"
		"layout(location = 1) in vec3 normal;\n"
		"layout(location = 2) in vec3 color;\n"
		"layout(location = 3) in vec2 texCoord;\n"
		"out vec3 ourColor;\n"
		"out vec2 TexCoord;\n"
		"uniform mat4 model_matrix;\n"
		"uniform mat4 view;\n"
		"uniform mat4 projection;\n"
		"void main()\n"
		"{\n"
		"gl_Position = projection * view * model_matrix * vec4(position, 1.0f);\n"
		"ourColor = color;\n"
		"TexCoord = texCoord;\n"
		"}\n";

	const GLchar* def_frag_shader =
		"#version 330 core \n"
		"in vec3 ourColor;\n"
		"in vec2 TexCoord;\n"
		"out vec4 color;\n"
		"uniform sampler2D ourTexture;\n"
		"void main()\n"
		"{\n"
		"color = texture(ourTexture, TexCoord);\n"
		"}\n";
};

enum shader_type
{
	GTI_FRAGMENT_SHADER,
	GTI_VERTEX_SHADER
};

class ShaderObject
{
public:
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

	bool UseProgram();

	GLuint id_shader_prog = 0;
};


//DEFAULT SHADER OBJECT




#endif