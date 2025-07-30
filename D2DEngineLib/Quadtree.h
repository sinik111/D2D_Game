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

    // 이전에 삽입된 모든 객체 중 dirty 상태인 객체를 업데이트합니다.
    void Update();

    std::vector<Collider*> Query(const Bounds& queryBounds);
    std::vector<Collider*> GetPotentialCollisions(Collider* object);

    std::vector<Bounds> GetAllNodeBounds() const;

private:
    std::unique_ptr<QuadtreeNode> root;
    int maxDepth;
    int maxObjectsPerNode;

    // 쿼드트리에 현재 삽입되어 있는 모든 객체들을 추적합니다.
    // unique_ptr나 shared_ptr로 객체 소유권을 관리할 수도 있지만,
    // 현재는 ICollidable* 를 사용하므로 이 목록도 raw pointer로 유지합니다.
    // 객체 생명주기는 Quadtree 외부에서 관리된다고 가정합니다.
    std::vector<Collider*> allManagedObjects;

    void Subdivide(QuadtreeNode* node);
    void InsertObjectRecursive(QuadtreeNode* node, Collider* object);
    void QueryRecursive(QuadtreeNode* node, const Bounds& queryBounds, std::vector<Collider*>& result);
    void ClearRecursiveObjects(QuadtreeNode* node);

    // 객체를 쿼드트리에서 제거하는 도우미 함수 (Update에서 사용)
    bool RemoveObjectRecursive(QuadtreeNode* node, Collider* object);

    void CollectAllNodeBoundsRecursive(const QuadtreeNode* node, std::vector<Bounds>& allBounds) const;
};