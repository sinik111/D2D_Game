#pragma once

#include "Bounds.h"

class Collider;

struct QuadtreeNode
{
	Bounds bounds;
	std::vector<Collider*> objects;
	std::unique_ptr<QuadtreeNode> children[4];

	int depth;
	bool isLeaf = true;

	QuadtreeNode(const Bounds& bounds, int currentDepth);
	~QuadtreeNode() = default;
};

class Quadtree
{
public:
    Quadtree(const Bounds& worldBounds, int maxDepth, int maxObjectsPerNode);
    ~Quadtree() = default;

    void Clear();
    void Insert(Collider* object);
    void Remove(Collider* object);

    void Relocate(Collider* object);

    // ������ ���Ե� ��� ��ü �� dirty ������ ��ü�� ������Ʈ�մϴ�.
    void Update();

    std::vector<Collider*> Query(const Bounds& queryBounds);
    std::vector<Collider*> GetPotentialCollisions(Collider* object);

    std::vector<Bounds> GetAllNodeBounds() const;

private:
    std::unique_ptr<QuadtreeNode> root;
    int maxDepth;
    int maxObjectsPerNode;

    void Subdivide(QuadtreeNode* node);
    void InsertObjectRecursive(QuadtreeNode* node, Collider* object);
    void QueryRecursive(QuadtreeNode* node, const Bounds& queryBounds, std::vector<Collider*>& result);
    void ClearRecursiveObjects(QuadtreeNode* node);

    // ��ü�� ����Ʈ������ �����ϴ� ����� �Լ� (Update���� ���)
    bool RemoveObjectRecursive(QuadtreeNode* node, Collider* object);

    void CollectAllNodeBoundsRecursive(const QuadtreeNode* node, std::vector<Bounds>& allBounds) const;
};