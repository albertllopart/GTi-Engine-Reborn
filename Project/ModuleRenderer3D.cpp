#include "Globals.h"
#include "Application.h"
#include "ModuleRenderer3D.h"
#include "ComponentMaterial.h"
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

ModuleRenderer3D::ModuleRenderer3D(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	name = "Render";
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

	// Projection matrix for
	OnResize(App->window->width, App->window->height);

	return ret;
}

bool ModuleRenderer3D::Start()
{
	return true;
}

// PreUpdate: clear buffer
update_status ModuleRenderer3D::PreUpdate(float dt)
{
	ComponentCamera* cam = App->camera->GetCamera();

	if (cam->projection_changed == true)
	{
		RefreshProjection();
		cam->projection_changed = false;
	}

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(camera->GetOpenGLViewMatrix());

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
		glPushMatrix();
		glMultMatrixf((float*)&to_draw->GetMyGo()->GetTransMatrix());

		if (to_draw->GetMyGo()->FindComponent(COMPONENT_MATERIAL) != nullptr)
		{
			ComponentMaterial* temp = (ComponentMaterial*)to_draw->GetMyGo()->FindComponent(COMPONENT_MATERIAL);

			if (to_draw->mesh->id_texcoord != 0)
				glBindTexture(GL_TEXTURE_2D, temp->GetID());
		}


		glEnableClientState(GL_VERTEX_ARRAY);
		glBindBuffer(GL_ARRAY_BUFFER, to_draw->mesh->id_vertex);
		glVertexPointer(3, GL_FLOAT, 0, NULL);

		if (to_draw->mesh->id_texcoord != 0)
		{
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			glBindBuffer(GL_ARRAY_BUFFER, to_draw->mesh->id_texcoord);
			glTexCoordPointer(2, GL_FLOAT, 0, NULL);
		}

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, to_draw->mesh->id_index);

		glDrawElements(GL_TRIANGLES, to_draw->mesh->num_index, GL_UNSIGNED_INT, NULL);



		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
		glDisableClientState(GL_VERTEX_ARRAY);
		glBindTexture(GL_TEXTURE_2D, 0);

		if (show_normals)//draw normals
		{
			for (int i = 0; i < to_draw->mesh->num_vertex; i += 3)
			{
				pLine line(to_draw->mesh->vertex[i], to_draw->mesh->vertex[i + 1], to_draw->mesh->vertex[i + 2], to_draw->mesh->normals[i] + to_draw->mesh->vertex[i], to_draw->mesh->normals[i + 1] + to_draw->mesh->vertex[i + 1], to_draw->mesh->normals[i + 2] + to_draw->mesh->vertex[i + 2]);
				line.Render();
			}
		}

		glPopMatrix();
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