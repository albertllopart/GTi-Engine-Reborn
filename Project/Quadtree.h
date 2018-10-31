#ifndef __QUADTREE__
#define __QUADTREE__

#define QUADTREE_MAX_SIZE 5
#define QUADTREE_MAX_SUBDIVISIONS 5

#include "MathGeoLib/Geometry/AABB.h"
#include <list>

class GameObject;
class QuadtreeNode
{
public:
	QuadtreeNode(const AABB bbox);
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
};

class Quadtree
{
public:

	Quadtree();
	~Quadtree();

	void SetBoundary(const AABB& limits);
	void Clear();

	void Insert(GameObject* gameObject);

	template<typename Type>
	void CollectIntersections(std::vector<GameObject*>& gameObjects, Type& primitive);

public:

	QuadtreeNode* root = nullptr;
};



#endif // !__QUADTREE__
