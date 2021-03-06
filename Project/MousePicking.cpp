#include "MousePicking.h"
#include "Application.h"
#include "GameObject.h"
#include "ComponentMesh.h"

#include "MathGeoLib/Geometry/LineSegment.h"
#include "MathGeoLib/Geometry/Triangle.h"

MousePicking::MousePicking()
{

}

MousePicking::~MousePicking()
{

}

math::LineSegment MousePicking::RayfromMouse(int x, int y)
{
	UpdateScreenSize();
	/*numbers between 0.0 and 1.0 representing where the mouse
	is where 0.0 is the first pixel of the screen and 1.0 is the last
	in terms of width and height*/
	float x_coefficient = x;
	float y_coefficient = y;

	x_coefficient = - (1.0 - ((x_coefficient * 2.0f) / screenWidth));
	y_coefficient = 1.0 - ((y_coefficient * 2.0f) / screenHeight);

	my_ray = App->camera->GetCamera()->GetFrustum().UnProjectLineSegment(x_coefficient, y_coefficient);

	return my_ray;
}

GameObject* MousePicking::PickFromRay() const
{
	//set far plane as first distance check
	float nearest_distance = 10000.0f;

	
	//nearest selected GameObject
	GameObject* ret = App->editor->GetSelected();
	//Exception for cameras! they do not have a bounding box :(
	//if (App->editor->GetSelected()->IsCamera())
	//	ret = App->editor->GetSelected();

	//gameObject vector
	std::vector<GameObject*> collided;
	
	//dynamic
	std::vector<GameObject*> all_go = App->editor->GetAllGO();
	for (int i = 0; i < all_go.size(); i++)
	{
		if (all_go[i]->is_static == false && all_go[i] != App->editor->GetRoot())
		{
			if (all_go[i]->FindComponent(COMPONENT_MESH) != nullptr)
				collided.push_back(all_go[i]);
		}
	}
	//static
	App->editor->quadtree.CollectIntersections(collided, my_ray);

	//check every mesh triangle
	for (int i = 0; i < collided.size(); i++)
	{
		math::LineSegment local_ray = my_ray;
		local_ray.Transform(collided[i]->GetGlobalMatrix().Inverted());

		ComponentMesh* mesh = (ComponentMesh*)collided[i]->FindComponent(COMPONENT_MESH);

		for (int j = 0; j < mesh->mesh->mesh.num_index; j++)
		{
			math::Triangle triangle;

			triangle.a = math::float3(mesh->mesh->mesh.vertex[j * 3], mesh->mesh->mesh.vertex[j * 3 + 1], mesh->mesh->mesh.vertex[j * 3 + 2]);
			j++;
			triangle.b = math::float3(mesh->mesh->mesh.vertex[j * 3], mesh->mesh->mesh.vertex[j * 3 + 1], mesh->mesh->mesh.vertex[j * 3 + 2]);
			j++;
			triangle.c = math::float3(mesh->mesh->mesh.vertex[j * 3], mesh->mesh->mesh.vertex[j * 3 + 1], mesh->mesh->mesh.vertex[j * 3 + 2]);

			float distance = 10000.0f;
			math::float3 hit_point;

			if (local_ray.Intersects(triangle, &distance, &hit_point))
			{
				if (distance < nearest_distance)
				{
					nearest_distance = distance;
					ret = collided[i];
				}
			}
		}
	}

	return ret;
}

void MousePicking::UpdateScreenSize()
{
	screenWidth = App->window->screen_surface->w;
	screenHeight = App->window->screen_surface->h;
}