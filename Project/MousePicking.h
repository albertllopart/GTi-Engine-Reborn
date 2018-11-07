#ifndef __Mouse_Picking__
#define __Mouse_Picking__

#include "Globals.h"
#include "MathGeoLib/Geometry/LineSegment.h"

class MousePicking
{
public:

	MousePicking();
	~MousePicking();

	LineSegment RayfromMouse(int x, int y);
	void UpdateScreenSize();

public:

	uint screenWidth = 0;
	uint screenHeight = 0;
};

#endif