#ifndef __Mouse_Picking__
#define __Mouse_Picking__

#include "Globals.h"
#include "MathGeoLib/Geometry/LineSegment.h"

class GameObject;

class MousePicking
{
public:

	MousePicking();
	~MousePicking();

	math::LineSegment RayfromMouse(int x, int y);
	GameObject* PickFromRay() const;
	void UpdateScreenSize();

public:

	math::LineSegment my_ray;
	uint screenWidth = 0;
	uint screenHeight = 0;
};

#endif