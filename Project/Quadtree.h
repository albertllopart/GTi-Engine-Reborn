#ifndef __QUADTREE__
#define __QUADTREE__

#define QUADTREE_MAX_SIZE 1
#define QUADTREE_MAX_SUBDIVISIONS 5

#include "MathGeoLib/Geometry/AABB.h"
#include <list>

#include "Globals.h"

class GameObject;

enum QuadSubdivide
{
	TOP_LEFT,
	TOP_RIGHT,
	BOT_LEFT,
	BOT_RIGHT
};

class QuadtreeNode
{
public:

	QuadtreeNode(const AABB bbox, QuadtreeNode* parent = nullptr);
	virtual ~QuadtreeNode();

	bool IsLeaf();

	void Insert(GameObject* to_insert);
	void Remove(GameObject* to_remove); // If we remove something in the quadtree, better delete the current quadtree and generate a new one

	void Subdivide();
	void DistributeObjects();

	void DrawQuadtree();

public:

	AABB bbox;	
	std::list<GameObject*> objects;	
	QuadtreeNode* parent = nullptr; 
	QuadtreeNode* childs[4]; // TODO try it with 8 (octree)

	uint subdivisions = 0;
};

class Quadtree
{
public:

	Quadtree();
	~Quadtree();

	void Boundaries(AABB limits);
	void Clear();

	void Remove(GameObject* to_remove);
	void Insert(GameObject* gameObject);

	template<typename Type>
	void CollectIntersections(std::vector<GameObject*>& gameObjects, Type& primitive);

public:

	QuadtreeNode* root = nullptr;
};





template<typename Type>
inline void Quadtree::CollectIntersections(std::vector<GameObject*>& gameObjects, Type & primitive)
{
	if (primitive.Intersects(box))
	{
		for (std::list<GameObject*>::const_iterator it = this->objects.begin(); it != this->objects.end(); ++it)
		{
			if (primitive.Intersects((*it)->global_bbox))
				objects.push_back(*it);

		}
		for (int i = 0; i < 4; ++i)
			if (childs[i] != nullptr) childs[i]->CollectIntersections(objects, primitive);

	}
}



#endif // !__QUADTREE__