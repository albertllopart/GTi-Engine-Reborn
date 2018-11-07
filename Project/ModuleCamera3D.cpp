#include "Globals.h"
#include "Application.h"
#include "ModuleCamera3D.h"
#include "ModuleInput.h"
#include "Glew/include/glew.h"
#include "MousePicking.h"
#include "parson/parson.h"

ModuleCamera3D::ModuleCamera3D(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	camera = new ComponentCamera();
	picking = LineSegment(float3::zero, float3::unitY);
	mouse_picker = new MousePicking();
}

ModuleCamera3D::~ModuleCamera3D()
{
	RELEASE(camera);
}

bool ModuleCamera3D::Init(JSON_Object* node)
{
	App->renderer3D->active_camera = camera;

	return true;
}
// -----------------------------------------------------------------
bool ModuleCamera3D::Start()
{
	LOG("Setting up the camera");
	App->imgui->consoleLogs.push_back("Setting up the camera");
	bool ret = true;

	return ret;
}

// -----------------------------------------------------------------
bool ModuleCamera3D::CleanUp()
{
	LOG("Cleaning camera");
	App->imgui->consoleLogs.push_back("Cleaning camera");

	App->renderer3D->active_camera = nullptr;

	return true;
}

// -----------------------------------------------------------------
update_status ModuleCamera3D::Update(float dt)
{
	if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_DOWN && mouse_picker!= nullptr)
	{
		picking = mouse_picker->RayfromMouse(App->input->GetMouseX(), App->input->GetMouseY());
	}

	glBegin(GL_LINES);
	glLineWidth(2.0f);
	glColor4f(1.0f, 1.0f, 0.0f, 1.0f);
	glVertex2f(picking.a.x, picking.a.y);
	glVertex2f(picking.b.x, picking.b.y);
	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

	glEnd();

	//end of ray

	Frustum* frustum = &camera->frustum;

	Move(dt);

	// Check motion for lookat / Orbit cameras
	int motion_x, motion_y;
	motion_x = App->input->GetMouseXMotion();
	motion_y = App->input->GetMouseYMotion();

	if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_REPEAT && (motion_x != 0 || motion_y != 0))
	{
		float dx = (float)-motion_x * 1.0f * dt;
		float dy = (float)-motion_y * 1.0f * dt;

		if (App->input->GetKey(SDL_SCANCODE_LCTRL) == KEY_REPEAT)
			Orbit(dx, dy);
		else
			LookAt(dx, dy);
	}

	//math::float3 newPos(0.0f, 0.0f, 0.0f);
	//float speed = 1.0f * dt;
	//
	//if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
	//{
	//	speed = 2.0f * dt;
	//}

	//if (free_move)
	//{
	//	if (App->input->GetKey(SDL_SCANCODE_R) == KEY_REPEAT) newPos.y += speed;
	//	if (App->input->GetKey(SDL_SCANCODE_E) == KEY_REPEAT) newPos.y -= speed;

	//	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) newPos -= Z * speed;
	//	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) newPos += Z * speed;


	//	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) newPos -= X * speed;
	//	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) newPos += X * speed;

	//	Position += newPos;
	//	Reference += newPos;
	//}
	//	//code for camera focus around objects
	//if (App->input->GetKey(SDL_SCANCODE_LALT) == KEY_REPEAT && App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_REPEAT)
	//{
	//	if (/*App->editor->GetMeshList().size() > 0*/true)
	//	{
	//		free_move = true; //THIS NEEDS A COMPLETE REWORK, BROKEN AF

	//		//focus = App->editor->GetMeshList().back()->mesh->pos; //TODO: fix this sheet
	//		LookAt(focus);
	//		//////////////////////////////////CAMERA ROTATION CODE///////////////////////////////
	//		int dx = -App->input->GetMouseXMotion();
	//		int dy = -App->input->GetMouseYMotion();

	//		float Sensitivity = 0.009f;

	//		Position -= Reference;

	//		if (dx != 0)
	//		{
	//			float DeltaX = (float)dx * Sensitivity;

	//			math::float3x3 rotationMatrix = math::float3x3::RotateY(DeltaX);
	//			X = rotationMatrix * X;
	//			Y = rotationMatrix * Y;
	//			Z = rotationMatrix * Z;
	//		}

	//		if (dy != 0)
	//		{
	//			float DeltaY = (float)dy * Sensitivity;

	//			math::float3x3 rotationMatrix = math::float3x3::RotateAxisAngle(X, DeltaY);
	//			Y = rotationMatrix * Y;
	//			Z = rotationMatrix * Z;

	//			if (Y.y < 0.0f)
	//			{
	//				Z = math::float3(0.0f, Z.y > 0.0f ? 1.0f : -1.0f, 0.0f);
	//				Y = math::Cross(Z, X);
	//			}
	//		}

	//		Position = Reference + Z * Position.Length();
	//		///////////////////////////////////////END/////////////////////////////////////////////
	//	}
	//	
	//}
	//else
	//{
	//	free_move = true;
	//}

	//if (App->input->GetKey(SDL_SCANCODE_F) == KEY_DOWN)
	//{
	//	if (/*App->editor->GetMeshList().size() > 0*/true)
	//	{
	//		free_move = false;
	//		//CenterToMesh(App->editor->GetMeshList().back());
	//	}
	//}

	//	// Mouse motion ----------------
	//if (App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT)
	//{
	//	free_move = true;

	//	int dx = -App->input->GetMouseXMotion();
	//	int dy = -App->input->GetMouseYMotion();

	//	float Sensitivity = 0.009f;

	//	Position -= Reference;

	//	if (dx != 0)
	//	{
	//		float DeltaX = (float)dx * Sensitivity;

	//		math::float3x3 rotationMatrix = math::float3x3::RotateY(DeltaX);
	//		X = rotationMatrix * X;
	//		Y = rotationMatrix * Y;
	//		Z = rotationMatrix * Z;
	//	}

	//	if (dy != 0)
	//	{
	//		float DeltaY = (float)dy * Sensitivity;

	//		math::float3x3 rotationMatrix = math::float3x3::RotateAxisAngle(X, DeltaY);
	//		Y = rotationMatrix * Y;
	//		Z = rotationMatrix * Z;

	//		if (Y.y < 0.0f)
	//		{
	//			Z = math::float3(0.0f, Z.y > 0.0f ? 1.0f : -1.0f, 0.0f);
	//			Y = math::Cross(Z, X);
	//		}
	//	}

	//	Position = Reference + Z * Position.Length();
	//}
	//else
	//{
	//	free_move = false;
	//}

	//

	//if (App->input->GetMouseZ() == 1)
	//{
	//	newPos -= Z * speed;
	//	Position += newPos;
	//	Reference += newPos;
	//}
	//else if (App->input->GetMouseZ() == -1)
	//{
	//	newPos += Z * speed;
	//	Position += newPos;
	//	Reference += newPos;
	//}

	//// Recalculate matrix -------------
	//CalculateViewMatrix();


	return UPDATE_CONTINUE;
}

// -----------------------------------------------------------------
void ModuleCamera3D::Look(const float3& position)
{
	camera->Look(position);
}

void ModuleCamera3D::CenterOn(const float3& position, float distance)
{
	float3 v = camera->frustum.front.Neg();
	camera->frustum.pos = position + (v * distance);
	looking_at = position;
	looking = true;
}

// -----------------------------------------------------------------
void ModuleCamera3D::LookAt( const float3 &Spot)
{
	Reference = Spot;
	// GEOLIB
	Z = (Position - Reference).Normalized();
	X = math::Cross(math::float3(0.0f, 1.0f, 0.0f), Z).Normalized();
	Y = math::Cross(Z, X);

	CalculateViewMatrix();
}


// -----------------------------------------------------------------
void ModuleCamera3D::Move(float dt)
{
	Frustum* frustum = &camera->frustum;

	float speed = 1.0f * dt;
	
	if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
	{
		speed = 2.0f * dt;
	}

	float3 right(frustum->WorldRight());
	float3 forward(frustum->front);

	float3 movement(float3::zero);

	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) movement += forward;
	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) movement -= forward;
	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) movement -= right;
	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) movement += right;
	if (App->input->GetKey(SDL_SCANCODE_R) == KEY_REPEAT) movement += float3::unitY;
	if (App->input->GetKey(SDL_SCANCODE_F) == KEY_REPEAT) movement -= float3::unitY;

	if (movement.Equals(float3::zero) == false)
	{
		frustum->Translate(movement * speed);
	}
}

void ModuleCamera3D::Orbit(float dx, float dy)
{
	float3 point = looking_at;

	if (looking == false)
	{
		LineSegment picking = camera->frustum.UnProjectLineSegment(0.f, 0.f);
		float distance;
		GameObject* hit = App->editor->GetSelected();

		if (hit != nullptr)
			point = picking.GetPoint(distance);
		else
			point = camera->frustum.pos + camera->frustum.front * 50.0f;

		looking = true;
		looking_at = point;
	}

	float3 focus = camera->frustum.pos - point;

	Quat qy(camera->frustum.up, dx);
	Quat qx(camera->frustum.WorldRight(), dy);

	focus = qx.Transform(focus);
	focus = qy.Transform(focus);

	camera->frustum.pos = focus + point;

	Look(point);
}

void ModuleCamera3D::LookAt(float dx, float dy)
{
	looking = false;

	if (dx != 0.f)
	{
		Quat q = Quat::RotateY(dx);
		camera->frustum.front = q.Mul(camera->frustum.front).Normalized();
		camera->frustum.up = q.Mul(camera->frustum.up).Normalized();
	}

	if (dy != 0.f)
	{
		Quat q = Quat::RotateAxisAngle(camera->frustum.WorldRight(), dy);

		float3 new_up = q.Mul(camera->frustum.up).Normalized();

		if (new_up.y > 0.0f)
		{
			camera->frustum.up = new_up;
			camera->frustum.front = q.Mul(camera->frustum.front).Normalized();
		}
	}
}

void ModuleCamera3D::CameraRotation() const
{
}

// -----------------------------------------------------------------
float* ModuleCamera3D::GetViewMatrix()
{
	return ViewMatrix.ptr();
}

ComponentCamera * ModuleCamera3D::GetCamera() const
{
	return camera;
}

ComponentCamera * ModuleCamera3D::SetCamera(ComponentCamera * cam)
{
	return camera = cam;
}

// -----------------------------------------------------------------
void ModuleCamera3D::CalculateViewMatrix()
{
	// GEOLIB
	ViewMatrix = math::float4x4(X.x, Y.x, Z.x, 0.0f, X.y, Y.y, Z.y, 0.0f, X.z, Y.z, Z.z, 0.0f, -math::Dot(X, Position), -math::Dot(Y, Position), -math::Dot(Z, Position), 1.0f);
	ViewMatrixInverse = ViewMatrix.Inverted();
}

//void ModuleCamera3D::CenterToMesh(Mesh * mesh)
//{
//	if (mesh != nullptr)
//	{
//		float3 temp = mesh->bbox.Centroid();
//		float3 size = mesh->bbox.Size() * 0.5f;
//		Position = float3(temp.x + size.x, temp.y + size.y, temp.z + 5.0f + size.z);
//		Reference = float3(temp.x + size.x, temp.y + size.y, temp.z + size.z);
//		LookAt(float3(temp.x, temp.y, temp.z));
//	}
//}
// -----------------------------------------------------------------

float3 ModuleCamera3D::GetPosition() const
{
	return camera->GetFrustum().pos;
}