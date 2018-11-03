#include "Quadtree.h"
#include "Glew/include/glew.h"
#include "Globals.h"
#include "GameObject.h"
#include "MathGeoLib/Geometry/LineSegment.h"

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
	if (objects.size() == QUADTREE_MAX_SIZE)
	{
		if (IsLeaf())
		{
			Subdivide(); // subdivide in 4 
		}
		objects.push_back(gameObject);
		DistributeObjects();
	}
	else
	{
		objects.push_back(gameObject);
	}
		
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
	new_center.Set(center.x + new_size.x * 0.5f, center.y, center.z + new_size.z * 0.5f);
	new_bbox.SetFromCenterAndSize(new_center, new_size);
	childs[BOT_LEFT] = new QuadtreeNode(new_bbox, this);

	// bot right quad
	new_center.Set(center.x - new_size.x * 0.5f, center.y, center.z + new_size.z * 0.5f);
	new_bbox.SetFromCenterAndSize(new_center, new_size);
	childs[BOT_RIGHT] = new QuadtreeNode(new_bbox, this);

}

void QuadtreeNode::DistributeObjects()
{
	GameObject* item = nullptr;

	std::list<GameObject*>::const_iterator it;
	for (it = objects.begin(); it != objects.end();)
	{
		item = *it;

		bool intersecting[4];
		uint num_intersections = 0;
		//checking intersections between the 4 chils and the gameobject
		for (uint i = 0; i < 4; i++)
		{
			if (intersecting[i] = childs[i]->bbox.Intersects((const AABB)*item->bbox))
			{
				++num_intersections;
			}
		}
		if (num_intersections == 4)
		{
			it++;
		}
		else
		{
			it = objects.erase(it); //erase gameocject from father to add it to child nodes
			for (uint i = 0; i < 4; i++)
			{
				if (intersecting[i])
				{
					childs[i]->Insert(item);
				}
			}
		}
	}
}

void QuadtreeNode::DrawQuadtree()
{
	//same as boundingbox deugb draw
	for (uint i = 0; i < 12; i++)
	{
		glVertex3f(bbox.Edge(i).a.x, bbox.Edge(i).a.y, bbox.Edge(i).a.z);
		glVertex3f(bbox.Edge(i).b.x, bbox.Edge(i).b.y, bbox.Edge(i).b.z);
	}

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
	if (gameObject->bbox->Intersects(root->bbox))
	{
		root->Insert(gameObject);
	}

}
