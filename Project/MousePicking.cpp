#include "MousePicking.h"
#include "Application.h"

#include "MathGeoLib/Geometry/LineSegment.h"

MousePicking::MousePicking()
{

}

MousePicking::~MousePicking()
{

}

LineSegment MousePicking::RayfromMouse(int x, int y)
{
	UpdateScreenSize();
	/*numbers between 0.0 and 1.0 representing where the mouse
	is where 0.0 is the first pixel of the screen and 1.0 is the last
	in terms of width and height*/
	float x_coefficient = x;
	float y_coefficient = y;

	x_coefficient = - (1.0 - x_coefficient / screenWidth);
	y_coefficient = 1.0 - y_coefficient / screenHeight;

	return App->camera->GetCamera()->GetFrustum().UnProjectLineSegment(x_coefficient, y_coefficient);
}

void MousePicking::UpdateScreenSize()
{
	screenWidth = App->window->width;
	screenHeight = App->window->height;
}