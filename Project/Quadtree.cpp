#include "Quadtree.h"

QuadtreeNode::QuadtreeNode(const AABB bbox)
{
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
			Subdivide(); //subdivide in 4 
		}
		objects.push_back(gameObject);
		DistributeObjects();
	}
	else
	{
		objects.push_back(gameObject);
	}
		
}

void QuadtreeNode::Remove(GameObject * obj)
{
	//TODO?¿
}

void QuadtreeNode::Subdivide()
{
	//TODO
}

void QuadtreeNode::DistributeObjects()
{
	// TODO
}

void QuadtreeNode::DrawQuadtree()
{
	//TODO
}

////////////////////////////////QUADTREE/////////////////////////////////////////////////////