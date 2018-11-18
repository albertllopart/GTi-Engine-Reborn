	#include "Globals.h"
#include "Application.h"
#include "ModuleCamera3D.h"
#include "ModuleInput.h"
#include "Glew/include/glew.h"
#include "MousePicking.h"
#include "ImGuizmo/ImGuizmo.h"
#include "ComponentTransform.h"
#include "parson/parson.h"
#include "ImGui/imgui.h"

ModuleCamera3D::ModuleCamera3D(Application* app, bool start_enabled) : Module(app, start_enabled)
{

	picking = LineSegment(float3::zero, float3::unitY);
	mouse_picker = new MousePicking();
}

ModuleCamera3D::~ModuleCamera3D()
{
	RELEASE(camera);
}

bool ModuleCamera3D::Init(JSON_Object* node)
{
	camera = new ComponentCamera();
	camera->frustum.pos.y += 2.0f;
	camera->frustum.farPlaneDistance += 1000.f;
	return true;
}
// -----------------------------------------------------------------
bool ModuleCamera3D::Start()
{
	LOG("Setting up the camera");

	App->imgui->consoleLogs.push_back("Setting up the camera");
	bool ret = true;

	App->renderer3D->camera = camera;

	return ret;
}

// -----------------------------------------------------------------
bool ModuleCamera3D::CleanUp()
{
	LOG("Cleaning camera");
	App->imgui->consoleLogs.push_back("Cleaning camera");

	return true;
}

// -----------------------------------------------------------------
update_status ModuleCamera3D::Update(float dt)
{
	if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_DOWN && mouse_picker != nullptr && !ImGui::IsMouseHoveringAnyWindow())
	{
		if (ImGui::GetIO().WantCaptureMouse == false)
		{
			//App->editor->FillQuadtree();
			picking = mouse_picker->RayfromMouse(App->input->GetMouseX(), App->input->GetMouseY());
			App->editor->SetSelected(mouse_picker->PickFromRay());
		}
	}

	if (show_ray)
	{
		glBegin(GL_LINES);
		glLineWidth(2.0f);
		glColor4f(1.0f, 1.0f, 0.0f, 1.0f);

		glVertex3fv((GLfloat*)&picking.a);
		glVertex3fv((GLfloat*)&picking.b);

		glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

		glEnd();
		glLineWidth(1.0f);
	}
	//end of ray

	if (ImGui::GetIO().WantCaptureKeyboard == false)
	{
		Move(dt);
	}


	// Check motion for lookat / Orbit cameras
	int motion_x, motion_y;
	motion_x = App->input->GetMouseXMotion();
	motion_y = App->input->GetMouseYMotion();

	if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_REPEAT && (motion_x != 0 || motion_y != 0) && !ImGui::IsMouseHoveringAnyWindow())
	{
		float dx = (float)-motion_x * 1.0f * dt;
		float dy = (float)-motion_y * 1.0f * dt;

		if (App->input->GetKey(SDL_SCANCODE_LALT) == KEY_REPEAT)
		{
			ImGuizmo::Enable(false);

			if (App->editor->GetSelected() != nullptr && App->editor->GetSelected() != App->editor->GetRoot())
				reference = App->editor->GetSelected()->GetTransform()->GetPosition();
			else
				reference = float3::zero;

			Orbit(dx, dy);
		}
	}

	if (App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT && (motion_x != 0 || motion_y != 0) && !ImGui::IsMouseHoveringAnyWindow())
	{
		float dx = (float)-motion_x * 1.0f * dt;
		float dy = (float)-motion_y * 1.0f * dt;

		ImGuizmo::Enable(false);
		Rotate(dx, -dy);
	}

	int wheel = App->input->GetMouseZ();
	if (wheel != 0)
		Zoom(wheel);

	return UPDATE_CONTINUE;
}

// -----------------------------------------------------------------
void ModuleCamera3D::Look(const float3& position)
{
	camera->Look(position);
}

// -----------------------------------------------------------------
void ModuleCamera3D::Move(float dt)
{
	float speed = 1.0f * dt;

	if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
	{
		speed = 2.0f * dt;
	}

	float3 right(camera->frustum.WorldRight());
	float3 forward(camera->frustum.front);

	float3 movement(float3::zero);

	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) movement += forward;
	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) movement -= forward;
	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) movement -= right;
	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) movement += right;
	if (App->input->GetKey(SDL_SCANCODE_R) == KEY_REPEAT) movement += float3::unitY;
	if (App->input->GetKey(SDL_SCANCODE_F) == KEY_REPEAT) movement -= float3::unitY;

	if (movement.Equals(float3::zero) == false)
	{
		camera->frustum.Translate(movement * speed);
	}

	camera->UpdateMatrix();
}

void ModuleCamera3D::Orbit(float dx, float dy)
{
	float3 point = camera->GetFrustum().pos - reference;

	Quat quatx(camera->GetFrustum().WorldRight(), dy * 0.01f);
	Quat quaty(camera->GetFrustum().up, dx * 0.01f);

	point = quatx.Transform(point);
	point = quaty.Transform(point);

	camera->frustum.pos = point + reference;
	LookAt(reference);
	camera->UpdateMatrix();
}

void ModuleCamera3D::Rotate(float dx, float dy)
{
	if (dx != 0.0f)
	{
		Quat quat_x = Quat::RotateY(0.3*dx*DEGTORAD);
		camera->frustum.front = quat_x.Transform(camera->frustum.front).Normalized();
		camera->frustum.up = quat_x.Transform(camera->frustum.up).Normalized();
	}

	if (dy != 0.0f)
	{
		Quat quat_y = Quat::RotateAxisAngle(camera->frustum.WorldRight(), -0.3*dy * DEGTORAD);
		camera->frustum.up = quat_y.Transform(camera->frustum.up).Normalized();
		camera->frustum.front = quat_y.Transform(camera->frustum.front).Normalized();

		float3 new_axis = quat_y.Mul(camera->frustum.up).Normalized();

		if (new_axis.y != 0)
		{
			camera->frustum.up = new_axis;
			camera->frustum.front = quat_y.Mul(camera->frustum.front).Normalized();
		}
	}

	float3 distance = reference - camera->frustum.pos;
	reference = camera->frustum.pos + (camera->frustum.front.Normalized() * distance.Length());
	camera->UpdateMatrix();
}

void ModuleCamera3D::Zoom(float zoom)
{
	float distance = reference.Distance(camera->frustum.pos);
	float3 new_pos = camera->frustum.pos + camera->frustum.front * zoom * distance * 0.05f;
	camera->frustum.pos = new_pos;
	camera->UpdateMatrix();
}

void ModuleCamera3D::LookAt(const float3& position)
{
	camera->Look(position);
	reference = position;
}

void ModuleCamera3D::CameraRotation() const
{
}

ComponentCamera * ModuleCamera3D::GetCamera() const
{
	return camera;
}

ComponentCamera * ModuleCamera3D::SetCamera(ComponentCamera * cam)
{
	return camera = cam;
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

void ModuleCamera3D::SetPosition(float3 pos)
{
	float3 difference = pos - camera->frustum.pos;
	camera->frustum.pos = pos;
	reference += difference;
}

void ModuleCamera3D::CenterToMesh(ComponentMesh * mesh)
{
	if (mesh != nullptr)
	{
		float3 temp = mesh->mesh->mesh.bbox.Centroid();
		float3 size = mesh->mesh->mesh.bbox.Size() * 0.5f;
		SetPosition(float3(temp.x + size.x, temp.y + size.y, temp.z + 5.0f + size.z));
		reference = float3(temp.x + size.x, temp.y + size.y, temp.z + size.z);
		LookAt(float3(temp.x, temp.y, temp.z));
	}
}