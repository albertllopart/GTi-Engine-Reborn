#include "Quadtree.h"
#include "Glew/include/glew.h"
#include "Globals.h"
#include "GameObject.h"
#include "MathGeoLib/Geometry/LineSegment.h"
#include "MathGeoLib/Geometry/AABB.h"

QuadtreeNode::QuadtreeNode(const AABB bbox, QuadtreeNode* parent): bbox(bbox), parent(parent)
{
	for (uint i = 0; i < 4; ++i)
	{
		childs[i] = nullptr;
	}
	
}

QuadtreeNode::~QuadtreeNode()
{
}

bool QuadtreeNode::IsLeaf() //only if you are a leaf you subdivide by 4 once you get out of space
{
	if (childs[0] == nullptr)
	{
		return true;
	}
	return false;
}

void QuadtreeNode::Insert(GameObject * gameObject)
{
	// If the node has space for the gameobject, add it to its list
	if (IsLeaf() && (objects.size() < QUADTREE_MAX_SIZE ||
		(bbox.HalfSize().LengthSq() <= QUADTREE_MAX_SUBDIVISIONS * QUADTREE_MAX_SUBDIVISIONS)))
	{
		objects.push_back(gameObject);
	}

	else
	{
		if (IsLeaf())
		{
			//Divide the root node into 4 childs
			Subdivide();

			objects.push_back(gameObject);
			// All gameobjects of the father node have to be inside at least one of the childs
			DistributeObjects();
		}
		else
		{
			for (uint i = 0; i < 4; i++)
			{
				if (childs[i]->bbox.Intersects(*gameObject->bbox))
				{
					childs[i]->Insert(gameObject);
				}
			}
		}
	}
	//if (objects.size() == QUADTREE_MAX_SIZE)
	//{
	//	if (IsLeaf() && subdivisions < QUADTREE_MAX_SUBDIVISIONS)
	//	{
	//		Subdivide(); // subdivide in 4 
	//	}
	//	objects.push_back(gameObject);
	//	DistributeObjects();
	//}
	//else
	//{
	//	objects.push_back(gameObject);
	//}
		
}

void QuadtreeNode::Remove(GameObject * to_remove)
{
	std::list<GameObject*>::iterator it = std::find(objects.begin(), objects.end(), to_remove);

	if (it != objects.end())
	{
		objects.erase(it);
	}
	if (!IsLeaf())
	{
		for (uint i = 0; i < 4; i++)
		{
			childs[i]->Remove(to_remove);
		}
	}
}

void QuadtreeNode::Subdivide()
{
	//dividing by 4 the box
	float3 size = bbox.Size();
	float3 new_size(size.x*0.5f, size.y, size.z*0.5f);
	float3 center = bbox.CenterPoint();
	float3 new_center;

	AABB new_bbox;

	//up left quad
	new_center.Set(center.x - new_size.x * 0.5f, center.y, center.z - new_size.z * 0.5f);
	new_bbox.SetFromCenterAndSize(new_center, new_size);
	childs[TOP_LEFT] = new QuadtreeNode(new_bbox, this);

	// up right quad
	new_center.Set(center.x + new_size.x * 0.5f, center.y, center.z - new_size.z * 0.5f);
	new_bbox.SetFromCenterAndSize(new_center, new_size);
	childs[TOP_RIGHT] = new QuadtreeNode(new_bbox, this);

	//bot left quad
	new_center.Set(center.x + new_size.x * 0.5f, center.y, center.z + new_size.z* 0.5f);
	new_bbox.SetFromCenterAndSize(new_center, new_size);
	childs[BOT_LEFT] = new QuadtreeNode(new_bbox, this);

	// bot right quad
	new_center.Set(center.x - new_size.x * 0.5f, center.y, center.z + new_size.z* 0.5f);
	new_bbox.SetFromCenterAndSize(new_center, new_size);
	childs[BOT_RIGHT] = new QuadtreeNode(new_bbox, this);


	childs[TOP_LEFT]->subdivisions = childs[TOP_RIGHT]->subdivisions = childs[BOT_LEFT]->subdivisions = childs[BOT_RIGHT]->subdivisions = subdivisions + 1;
}

void QuadtreeNode::DistributeObjects()
{
	GameObject* object = nullptr;

	std::list<GameObject*>::iterator it;
	for (it = objects.begin(); it != objects.end();)
	{
		object = *it;
		bool intersecting[4];
		uint num_intersections = 0;

		for (uint i = 0; i < 4; i++)
		{
			if (intersecting[i] = childs[i]->bbox.Intersects(*object->bbox))
			{
				num_intersections++;
			}
		}

		if (num_intersections == 4)
		{
			it++;
		}
		else
		{
			// Erase this game object from the father list to add it to childs nodes
			it = objects.erase(it);
			for (uint i = 0; i < 4; i++)
			{
				if (intersecting[i])
				{
					// Insert the Game Object into the correct child
					childs[i]->Insert(object);
				}
			}
		}
	}
}

void QuadtreeNode::DrawQuadtree()
{
	//same as boundingbox deugb draw
	glBegin(GL_LINES);
	glLineWidth(1.0f);
	glColor4f(0.7f, 1.0f, 1.0f, 0.6f);

	for (uint i = 0; i < 12; i++)
	{
		glVertex3f(bbox.Edge(i).a.x, bbox.Edge(i).a.y, bbox.Edge(i).a.z);
		glVertex3f(bbox.Edge(i).b.x, bbox.Edge(i).b.y, bbox.Edge(i).b.z);
	}

	glColor4f(1.0f, 1.0f, 1.0f, 1.0f);

	glEnd();

	if (childs[0] != nullptr) 
	{
		for (uint i = 0; i < 4; i++)
		{
			childs[i]->DrawQuadtree();
		}
	}
}

////////////////////////////////QUADTREE/////////////////////////////////////////////////////

Quadtree::Quadtree()
{
}

Quadtree::~Quadtree()
{
	Clear();
}

void Quadtree::Boundaries(AABB limits)
{
	Clear();
	root = new QuadtreeNode(limits);
}

void Quadtree::Clear()
{
	delete root;
	root = nullptr;
}

void Quadtree::Remove(GameObject * to_remove)
{
	if (root != nullptr)
	{
		root->Remove(to_remove);
	}
}

void Quadtree::Insert(GameObject * gameObject)
{
	if (root != nullptr)
	{
		if (gameObject->bbox->IsFinite())
		{
			if (gameObject->bbox->Intersects(root->bbox))
			{
				root->Insert(gameObject);
			}
		}
	}

}
