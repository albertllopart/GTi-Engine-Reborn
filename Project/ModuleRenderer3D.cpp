#include "Globals.h"
#include "Application.h"
#include "ModuleRenderer3D.h"
#include "ComponentMaterial.h"
#include "ComponentTransform.h"
#include "Glew/include/glew.h"
#include "Devil\include\ilut.h"
#include "Devil\include\il.h"
#include "SDL\include\SDL_opengl.h"
#include <vector>

#pragma comment (lib, "glu32.lib")    
#pragma comment (lib, "opengl32.lib") 
#pragma comment (lib, "Glew/libx86/glew32.lib")
#pragma comment( lib, "Devil/libx86/DevIL.lib" ) 
#pragma comment( lib, "Devil/libx86/ILU.lib" )
#pragma comment( lib, "Devil/libx86/ILUT.lib" ) 

#define BUFFER_OFFSET(i) ((char *)NULL + (i))

ModuleRenderer3D::ModuleRenderer3D(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	name = "Render";
	shaders_manager = new ShaderProgramManager();
	
}

// Destructor
ModuleRenderer3D::~ModuleRenderer3D()
{}

// Called before render is available
bool ModuleRenderer3D::Init(JSON_Object* node)
{

	LOG("Creating 3D Renderer context");
	App->imgui->AddConsoleLog("Creating 3D Renderer context");
	bool ret = true;

	//Create context
	context = SDL_GL_CreateContext(App->window->window);
	if(context == NULL)
	{
		LOG("OpenGL context could not be created! SDL_Error: %s\n", SDL_GetError());
		App->imgui->AddConsoleLog(("OpenGL context could not be created! SDL_Error: %s\n", SDL_GetError()));
		ret = false;
	}
	
	if(ret == true)
	{
		GLint GlewInitResult = glewInit();
		if (GLEW_OK != GlewInitResult)
		{
			LOG("ERROR: %s\n", glewGetErrorString(GlewInitResult));
			return false;
		}

		//Use Vsync
		if (VSYNC && SDL_GL_SetSwapInterval(1) < 0) {
			LOG("Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError());
			App->imgui->AddConsoleLog(("Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError()));
		}

		//Initialize Projection Matrix
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		//Check for error
		GLenum error = glGetError();
		if(error != GL_NO_ERROR)
		{
			LOG("Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}

		//Initialize Modelview Matrix
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

		//Check for error
		error = glGetError();
		if(error != GL_NO_ERROR)
		{
			LOG("Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}
		
		glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
		glClearDepth(1.0f);
		
		//Initialize clear color
		glClearColor(0.f, 0.f, 0.f, 1.f);

		//Check for error
		error = glGetError();
		if(error != GL_NO_ERROR)
		{
			LOG("Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}
		
		GLfloat LightModelAmbient[] = {0.0f, 0.0f, 0.0f, 1.0f};
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, LightModelAmbient);
		
		lights[0].ref = GL_LIGHT0;
		lights[0].ambient.Set(0.25f, 0.25f, 0.25f, 1.0f);
		lights[0].diffuse.Set(0.75f, 0.75f, 0.75f, 1.0f);
		lights[0].SetPos(0.0f, 0.0f, 2.5f);
		lights[0].Init();
		lights[0].Active(true);
		
		GLfloat MaterialAmbient[] = {1.0f, 1.0f, 1.0f, 1.0f};
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, MaterialAmbient);

		GLfloat MaterialDiffuse[] = {1.0f, 1.0f, 1.0f, 1.0f};
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MaterialDiffuse);
		

		if (depthTest)
		{
			glEnable(GL_DEPTH_TEST);
		}
		if (cullFace)
		{
			glEnable(GL_CULL_FACE);
		}
		if (lighting)
		{
			glEnable(GL_LIGHTING);
		}
		if (colorMaterial)
		{
			glEnable(GL_COLOR_MATERIAL);
		}
		if (texture2D)
		{
			glEnable(GL_TEXTURE_2D);
		}
	}

	GLubyte defaultImage[128][128][4];
	for (int i = 0; i < 128; i++) {
		for (int j = 0; j < 128; j++) {

			defaultImage[i][j][0] = (GLubyte)255;
			defaultImage[i][j][1] = (GLubyte)255;
			defaultImage[i][j][2] = (GLubyte)255;
			defaultImage[i][j][3] = (GLubyte)255;
		}
	}
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glGenTextures(1, &DefaultTexture);
	glBindTexture(GL_TEXTURE_2D, DefaultTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 128, 128,
		0, GL_RGBA, GL_UNSIGNED_BYTE, defaultImage);

	// Projection matrix for
	OnResize(App->window->width, App->window->height);


	//default shader
	shaders_manager->default_shader = *shaders_manager->CreateDefaultShaderProgram();
	shaders_manager->water_shader = *shaders_manager->CreateWaterShaderProgram();
	DefaultTexture = App->textures->importer->Load("white");
	return ret;
}

bool ModuleRenderer3D::Start()
{
	return true;
}

// PreUpdate: clear buffer
update_status ModuleRenderer3D::PreUpdate(float dt)
{
	shader_dt = App->time->GetRealTime();

	ComponentCamera* cam = App->camera->GetCamera();

	if (cam->projection_changed == true)
	{
		RefreshProjection();
		cam->projection_changed = false;
	}

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(camera->GetOpenGLViewMatrix().ptr());

	math::float4x4 test = camera->GetOpenGLProjectionMatrix();

	// Light 0 on cam pos
	//lights[0].SetPos(camera->frustum.pos.x, camera->frustum.pos.y, camera->frustum.pos.z);
	/*if (camera != nullptr)
	{
		FrustumCulling();
	}*/
	

	for (uint i = 0; i < MAX_LIGHTS; ++i)
		lights[i].Render();

	return UPDATE_CONTINUE;
}

// PostUpdate present buffer to screen
update_status ModuleRenderer3D::PostUpdate(float dt)
{
	if (wireframe)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	App->editor->Draw();

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	App->imgui->Draw();

	SDL_GL_SwapWindow(App->window->window);

	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleRenderer3D::CleanUp()
{
	LOG("Destroying 3D Renderer");
	App->imgui->AddConsoleLog("Destroying 3D Renderer");

	SDL_GL_DeleteContext(context);

	return true;
}

void ModuleRenderer3D::FrustumCulling() const
{
	App->camera->camera->Culling();
}

void ModuleRenderer3D::Draw(ComponentMesh* to_draw)
{
	if (to_draw->GetMyGo()->visible)
	{
		math::float4x4 matrixfloat = to_draw->GetMyGo()->GetTransform()->GetGlobalMatrix();
		GLfloat matrix[16] =
		{
			matrixfloat[0][0],matrixfloat[1][0],matrixfloat[2][0],matrixfloat[3][0],
			matrixfloat[0][1],matrixfloat[1][1],matrixfloat[2][1],matrixfloat[3][1],
			matrixfloat[0][2],matrixfloat[1][2],matrixfloat[2][2],matrixfloat[3][2],
			matrixfloat[0][3],matrixfloat[1][3],matrixfloat[2][3],matrixfloat[3][3]
		};

		uint offset = sizeof(float)* (3 + 3 + 4 + 2);
		if (to_draw->mesh->mesh.texCoords == nullptr)
			offset -= sizeof(float) * 2;
		if (to_draw->mesh->mesh.normals == nullptr)
			offset -= sizeof(float) * 3;

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		ShaderProgram* shader_active = &shaders_manager->default_shader;
		shader_active->UseProgram();

		if (to_draw->GetMyGo()->FindComponent(COMPONENT_MATERIAL) != nullptr)
		{
			ComponentMaterial* text = (ComponentMaterial*)to_draw->GetMyGo()->FindComponent(COMPONENT_MATERIAL);
			if (to_draw->mesh->mesh.id_texcoord != 0 && text->IsAlphaTest())
				glEnable(GL_ALPHA_TEST);
			glAlphaFunc(GL_GREATER, text->GetAlphaValue());
			glBindTexture(GL_TEXTURE_2D, text->GetID());

			if (text->sample_shader)
			{
				shader_active = &shaders_manager->water_shader;
				if (shader_active != nullptr)
					shader_active->UseProgram();
			}
			else if (text->own_shader)
			{
				shader_active = text->GetShader();
				if(shader_active!=nullptr)
					shader_active->UseProgram();
			}

			if (shader_active != nullptr)
			{
				glUniform1i(glGetUniformLocation(shader_active->id_shader_prog, "ourTexture"), 0);
				glUniform1f(glGetUniformLocation(shader_active->id_shader_prog, "ftime"), shader_dt);
			}

		}
		else
		{
			glBindTexture(GL_TEXTURE_2D, DefaultTexture);
			glUniform1i(glGetUniformLocation(shaders_manager->default_shader.id_shader_prog, "ourTexture"), 0);
		}


		glBindBuffer(GL_ARRAY_BUFFER, to_draw->mesh->mesh.id_vertex_info);
		glEnableVertexAttribArray(0);  
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, offset, 0); //VBO

		if (to_draw->mesh->mesh.normals != nullptr) {
			glEnableVertexAttribArray(1);    
			glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, offset, BUFFER_OFFSET(sizeof(float) * 3)); //normals, 12 bytes from start
		}

		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, offset, BUFFER_OFFSET(sizeof(float) *(3 + 3))); //color, 24 bytes from start

		if (to_draw->mesh->mesh.texCoords != nullptr) {
			glEnableVertexAttribArray(3);
			glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, offset, BUFFER_OFFSET(sizeof(float) * (3 + 3 + 4))); //texcoords, 40 bytes from start
		}

		if (to_draw->mesh->mesh.id_index != NULL && shader_active != nullptr)
		{
			GLint view2Loc = glGetUniformLocation(shader_active->id_shader_prog, "view_matrix");
			math::float4x4 temp = App->camera->camera->GetOpenGLViewMatrix();
			glUniformMatrix4fv(view2Loc, 1, GL_FALSE, temp.ptr());

			GLint modelLoc = glGetUniformLocation(shader_active->id_shader_prog, "model_matrix");
			temp = to_draw->GetMyGo()->GetTransform()->GetGlobalMatrix();
			glUniformMatrix4fv(modelLoc, 1, GL_TRUE, temp.ptr());

			GLint viewLoc = glGetUniformLocation(shader_active->id_shader_prog, "proj_matrix");
			temp = App->renderer3D->camera->GetOpenGLProjectionMatrix();
			glUniformMatrix4fv(viewLoc, 1, GL_FALSE, temp.ptr());


			if (to_draw->mesh->mesh.id_index != NULL)
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, to_draw->mesh->mesh.id_index);

			glDrawElements(GL_TRIANGLES, to_draw->mesh->mesh.num_index, GL_UNSIGNED_INT, NULL);
		}



		glBindTexture(GL_TEXTURE_2D, 0);
		glDisable(GL_TEXTURE_2D);
		glDisable(GL_ALPHA_TEST);
		glDisable(GL_BLEND);

		glUseProgram(NULL);

		ShowNormals(to_draw);
	}
}

void ModuleRenderer3D::UseTexture(uint shader_id, uint i, uint num)
{
	std::string var_name = "";
	if (num == 0)
		var_name = "_texture";
	else if (num == 1)
		var_name = "_normal_map";
	else if (num > 1 && num <= 10) { //10 is arbitrary max tex number
		char number = num + '0';
		num -= 1;
		std::string temp_name = "_texture";
		temp_name += number;
		var_name = temp_name;
	}
	else return;
	if (i == 0)
		i = DefaultTexture;
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindTexture(GL_TEXTURE_2D, i);
	//change shader
	int tex_id = glGetUniformLocation(shader_id, var_name.c_str());
	glUniform1i(tex_id, num);
}

void ModuleRenderer3D::ShowNormals(ComponentMesh * to_draw)
{
	if (show_normals)//draw normals
	{
		for (int i = 0; i < to_draw->mesh->mesh.num_vertex; i += 3)
		{
			pLine line(to_draw->mesh->mesh.vertex[i], to_draw->mesh->mesh.vertex[i + 1], to_draw->mesh->mesh.vertex[i + 2], to_draw->mesh->mesh.normals[i] + to_draw->mesh->mesh.vertex[i], to_draw->mesh->mesh.normals[i + 1] + to_draw->mesh->mesh.vertex[i + 1], to_draw->mesh->mesh.normals[i + 2] + to_draw->mesh->mesh.vertex[i + 2]);
			line.Render();
		}
	}
}

void ModuleRenderer3D::Draw(GameObject* to_draw)
{
	if (to_draw != nullptr)
	{
		std::vector<Component*> components = to_draw->GetComponents();
		ComponentMesh* local_mesh = nullptr;
		ComponentMaterial* local_mat = nullptr;
		for (int i = 0; i < components.size(); i++)
		{
			if (components[i]->GetType() == COMPONENT_MESH)
			{
				local_mesh = (ComponentMesh*)components[i];
			}
			{
				Draw(local_mesh);
			}
		}
	}
}


void ModuleRenderer3D::OnResize(int width, int height)
{
	if (height == 0)
	{
		return;
	}
	glViewport(0, 0, width, height);
	App->camera->camera->SetAspectRatio((float)width / (float)height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//// GEOLIB
	//ProjectionMatrix = perspective(60.0f, (float)width / (float)height, 0.125f, 512.0f);
	glLoadMatrixf((GLfloat*)(App->camera->camera->GetOpenGLProjectionMatrix().ptr()));

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}


void ModuleRenderer3D::SetDepthTest()
{
	if (depthTest)
	{
		glEnable(GL_DEPTH_TEST);
	}
	else
	{
		glDisable(GL_DEPTH_TEST);
	}
}
void ModuleRenderer3D::SetCullFace()
{
	if (cullFace)
	{
		glEnable(GL_CULL_FACE);
	}
	else
	{
		glDisable(GL_CULL_FACE);
	}
}
void ModuleRenderer3D::SetLighting()
{
	if (lighting)
	{
		glEnable(GL_LIGHTING);
	}
	else
	{
		glDisable(GL_LIGHTING);
	}
}

void ModuleRenderer3D::SetColorMaterial()
{
	if (colorMaterial)
	{
		glEnable(GL_COLOR_MATERIAL);
	}
	else
	{
		glDisable(GL_COLOR_MATERIAL);
	}
}

void ModuleRenderer3D::SetTexture2D()
{
	if (texture2D)
	{
		glEnable(GL_TEXTURE_2D);
	}
	else
	{
		glDisable(GL_TEXTURE_2D);
	}
}

void ModuleRenderer3D::SetWireFrame()
{
	if (wireframe)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
}

void ModuleRenderer3D::SetCamera(ComponentCamera * cam)
{
	if (cam == nullptr)
	{
		camera = App->camera->GetCamera();
	}
	else
	{
		camera = cam;
	}
}

void ModuleRenderer3D::SetMainCamera(ComponentCamera* cam) //Main camera gets affected by culling
{
	camera = cam;
}

void ModuleRenderer3D::RefreshProjection()
{
	ComponentCamera* cam = App->camera->GetCamera();

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	math::float4x4 test = cam->GetOpenGLProjectionMatrix();
	glLoadMatrixf((GLfloat*)(cam->GetOpenGLProjectionMatrix().ptr()));

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}