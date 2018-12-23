#ifndef SHADER_PROGRAM_MANAGER_H_
#define SHADER_PROGRAM_MANAGER_H_

#include "Globals.h"
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

	void UpdateDate(const char* code, int lenght)
	{
		RELEASE_ARRAY(data);
		data = new char[lenght + 1];
		strcpy_s(data, lenght + 1, code);
	}
public:

	shader_type type;
	GLuint id = 0;
	char* data = nullptr;
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
	ShaderProgram * CreateWaterShaderProgram();

public:

	std::vector<ShaderObject*> objects;
	std::list<ShaderProgram*> programs;
	ShaderProgram default_shader;
	ShaderProgram water_shader;

	const GLchar* water_vertex_shader =
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
		"uniform float ftime;\n"
		"\n"
		"out vec4 ourColor;\n"
		"out vec2 ourTexCoord;\n"
		"out float ourPosition;\n"
		"out float ourTime;\n"
		"out vec3 ourNormal;\n"
		"\n"
		"void main()\n"
		"{\n"
		"    ourTexCoord = texCoord;\n"
		"    ourColor = color;\n"
		"	 ourTime = ftime;\n"
		"	 ourNormal = mat3(model_matrix) * normal;\n"
		"	 vec3 pos = position;\n"
		"	 pos.z = position.z + 1.4 * (sin(ftime + pos.x - pos.y) + 0.8 * sin(ftime + pos.x + pos.y));\n"
		"	 ourPosition = position.z + 0.8 * (sin(ftime+pos.x-pos.y));\n"
		"    gl_Position = proj_matrix * view_matrix * model_matrix * vec4(pos, 1.0);\n"
		"}\n";

	const GLchar* water_frag_shader =
		"#version 330 core\n"
		"\n"
		"in vec4 ourColor;\n"
		"in vec2 ourTexCoord;\n"
		"in float ourPosition;\n"
		"in float ourTime;\n"
		"in vec3 ourNormal;\n"
		"out vec4 FragColor;\n"
		"\n"
		"uniform sampler2D ourTexture;\n"
		"\n"
		"void main()\n"
		"{\n"
		"	 if(ourPosition > 0.6)\n"
			 "{\n"
		"	  FragColor = vec4(1.0,1.0,1.0,1.0);\n"
			 "}\n"
		"	 else \n"
			"{\n"
		"	  FragColor = vec4(ourPosition/4,ourPosition/2,1.0,1.0);\n"
			"}\n"

		"}\n";

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
		"uniform float ftime;\n"
		"\n"
		"out vec4 ourColor;\n"
		"out vec2 ourTexCoord;\n"
		"out float ourPosition;\n"
		"out float ourTime;\n"
		"out vec3 ourNormal;\n"
		"\n"
		"void main()\n"
		"{\n"
		"    ourTexCoord = texCoord;\n"
		"    ourColor = color;\n"
		"	 ourTime = ftime;\n"
		"	 ourNormal = mat3(model_matrix) * normal;\n"
		"    gl_Position = proj_matrix * view_matrix * model_matrix * vec4(position, 1.0);\n"
		"}\n";

	const GLchar* def_frag_shader =
		"#version 330 core\n"
		"\n"
		"in vec4 ourColor;\n"
		"in vec2 ourTexCoord;\n"
		"in float ourPosition;\n"
		"in float ourTime;\n"
		"in vec3 ourNormal;\n"
		"out vec4 FragColor;\n"
		"\n"
		"uniform sampler2D ourTexture;\n"
		"\n"
		"void main()\n"
		"{\n"
		"	  FragColor = texture(ourTexture, ourTexCoord);\n"
		"}\n";
};

/*		"	 else if(ourPosition > -0.3)\n"
			"{\n"
		"	  FragColor = vec4(0.2,0.4,1.0,1.0);\n"
			"}\n"
		"	 else if(ourPosition > -1)\n"
			"{\n"
		"	  FragColor = vec4(0.1,0.2,1.0,1.0);\n"
			"}\n"*/




//DEFAULT SHADER OBJECT




#endif