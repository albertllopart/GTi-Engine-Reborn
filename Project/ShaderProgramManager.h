#ifndef SHADER_PROGRAM_MANAGER_H_
#define SHADER_PROGRAM_MANAGER_H_

#include "Glew/include/glew.h"
#include <list>
#include <vector>

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

class ShaderProgramManager
{
public:

	ShaderProgramManager();
	~ShaderProgramManager();

	bool Update(float dt);

	bool LoadShaderObject(ShaderObject* object);
	bool UnloadShaderObject(ShaderObject* object);

	ShaderProgram* CreateShaderProgram();

	ShaderProgram * CreateDefaultShaderProgram();

public:

	std::vector<ShaderObject*> objects;
	std::list<ShaderProgram*> programs;
	ShaderProgram default_shader;

	const GLchar* def_vertex_shader =
		"#version 330 core\n"
		"\n"
		"layout (location = 0) in vec3 position;\n"
		"layout (location = 1) in vec3 normal;\n"
		"layout (location = 2) in vec4 color;\n"
		"layout (location = 3) in vec2 texCoord;\n"
		"\n"
		"uniform mat4 view_matrix;\n"
		"uniform mat4 model_matrix;\n"
		"uniform mat4 proj_matrix;\n"
		"\n"
		"out vec4 ourColor;\n"
		"out vec2 ourTexCoord;\n"
		"\n"
		"void main()\n"
		"{\n"
		"    ourTexCoord = texCoord;\n"
		"    ourColor = color;\n"
		"    gl_Position = proj_matrix * view_matrix * model_matrix * vec4(position, 1.0);\n"
		"}\n";

	const GLchar* def_frag_shader =
		"#version 330 core\n"
		"\n"
		"in vec4 ourColor;\n"
		"in vec2 ourTexCoord;\n"
		"out vec4 FragColor;\n"
		"\n"
		"uniform sampler2D ourTexture;\n"
		"\n"
		"void main()\n"
		"{\n"
		"     FragColor = texture(ourTexture,ourTexCoord);\n"
		"}\n";
};




//DEFAULT SHADER OBJECT




#endif