#include "pch.h"
#include "Quadtree.h"

#include "Collider.h"
#include "Transform.h"

QuadtreeNode::QuadtreeNode(const Bounds& bounds, int currentDepth)
	: bounds{ bounds }, depth{ currentDepth }
{
}

Quadtree::Quadtree(const Bounds& worldBounds, int maxDepth, int maxObjectsPerNode)
    : maxDepth(maxDepth), maxObjectsPerNode(maxObjectsPerNode)
{
    root = std::make_unique<QuadtreeNode>(worldBounds, 0);
}

void Quadtree::Clear()
{
    ClearRecursiveObjects(root.get());
    root->objects.clear();
    for (int i = 0; i < 4; ++i)
    {
        root->children[i].reset();
    }
    root->isLeaf = true;
}

void Quadtree::Insert(Collider* object)
{
    if (!object || !root)
    {
        return;
    }

    InsertObjectRecursive(root.get(), object);
}

void Quadtree::InsertObjectRecursive(QuadtreeNode* node, Collider* object)
{
    if (!Bounds::IsOverlap(node->bounds, object->GetSpatialBounds()))
    {
        return;
    }

    if (node->isLeaf || node->depth >= maxDepth)
    {
        if (node->objectsLookup.find(object) == node->objectsLookup.end())
        {
            node->objects.push_back(object);
            node->objectsLookup.insert(object);
        }
        
        if (object->GetRigidBody2D() != nullptr)
        {
            object->AddBelongingNode(node);
        }

        if (node->objects.size() > maxObjectsPerNode && node->depth < maxDepth)
        {
            Subdivide(node);
        }

        return;
    }

    for (int i = 0; i < 4; ++i)
    {
        InsertObjectRecursive(node->children[i].get(), object);
    }
}

void Quadtree::Subdivide(QuadtreeNode* node)
{
    if (!node->isLeaf || node->depth >= maxDepth)
    {
        return;
    }

    node->isLeaf = false;

    Vector2 center = node->bounds.center;
    Vector2 extents = node->bounds.extents;

    node->children[0] = std::make_unique<QuadtreeNode>(Bounds(center + Vector2(-extents.x * 0.5f, extents.y * 0.5f), extents * 0.5f), node->depth + 1);
    node->children[1] = std::make_unique<QuadtreeNode>(Bounds(center + Vector2(extents.x * 0.5f, extents.y * 0.5f), extents * 0.5f), node->depth + 1);
    node->children[2] = std::make_unique<QuadtreeNode>(Bounds(center + Vector2(-extents.x * 0.5f, -extents.y * 0.5f), extents * 0.5f), node->depth + 1);
    node->children[3] = std::make_unique<QuadtreeNode>(Bounds(center + Vector2(extents.x * 0.5f, -extents.y * 0.5f), extents * 0.5f), node->depth + 1);

    std::vector<Collider*> oldObjects = std::move(node->objects);
    node->objects.clear();
    node->objectsLookup.clear();

    for (Collider* obj : oldObjects)
    {
        for (int i = 0; i < 4; ++i)
        {
            if (Bounds::IsOverlap(node->children[i]->bounds, obj->GetSpatialBounds()))
            {
                if (node->objectsLookup.find(obj) == node->objectsLookup.end())
                {
                    node->objects.push_back(obj);
                    node->objectsLookup.insert(obj);
                }

                if (obj->GetRigidBody2D() != nullptr)
                {
                    obj->AddBelongingNode(node);
                }

                if (node->children[i]->objects.size() > maxObjectsPerNode && node->children[i]->depth < maxDepth)
                {
                    Subdivide(node->children[i].get());
                }
            }
        }
    }
}

void Quadtree::Remove(Collider* object)
{
    if (!object || !root)
    {
        return;
    }

    RemoveObjectRecursive(root.get(), object);
}

void Quadtree::Relocate(Collider* object)
{
    auto& nodes = object->GetBelongingNode();
    if (!nodes.empty())
    {
        for (const auto& node : nodes)
        {
            for (auto iter = node->objects.begin(); iter != node->objects.end(); ++iter)
            {
                if (*iter == object)
                {
                    std::swap(*iter, node->objects.back());

                    node->objects.pop_back();
                    node->objectsLookup.erase(object);

                    break;
                }
            }
        }
    }

    nodes.clear();

    Insert(object);
}

bool Quadtree::RemoveObjectRecursive(QuadtreeNode* node, Collider* object)
{
    // 객체가 현재 노드의 영역 내에 있는지 확인 (최적화)
    if (!Bounds::IsOverlap(node->bounds, object->GetSpatialBounds()))
    {
        return false;
    }

    for (auto iter = node->objects.begin(); iter != node->objects.end(); ++iter)
    {
        if (*iter == object)
        {
            std::swap(*iter, node->objects.back());

            node->objects.pop_back();
            node->objectsLookup.erase(object);

            break;
        }
    }

    // 리프 노드가 아니면 자식 노드에서 탐색 및 제거 시도
    if (!node->isLeaf)
    {
        for (int i = 0; i < 4; ++i)
        {
            RemoveObjectRecursive(node->children[i].get(), object);
        }
    }
    return false; // 객체를 찾지 못했음
}

void Quadtree::CollectAllNodeBoundsRecursive(const QuadtreeNode* node, std::vector<Bounds>& allBounds) const
{
    allBounds.push_back(node->bounds);

    if (!node->isLeaf)
    {
        for (int i = 0; i < 4; ++i)
        {
            CollectAllNodeBoundsRecursive(node->children[i].get(), allBounds);
        }
    }
}

std::vector<Collider*> Quadtree::Query(const Bounds& queryBounds)
{
    std::vector<Collider*> result;

    QueryRecursive(root.get(), queryBounds, result);

    std::sort(result.begin(), result.end());
    result.erase(std::unique(result.begin(), result.end()), result.end());

    return result;
}

void Quadtree::QueryRecursive(QuadtreeNode* node, const Bounds& queryBounds, std::vector<Collider*>& result)
{
    if (!Bounds::IsOverlap(node->bounds, queryBounds))
    {
        return;
    }

    for (Collider* obj : node->objects)
    {
        if (Bounds::IsOverlap(obj->GetSpatialBounds(), queryBounds))
        {
            result.push_back(obj);
        }
    }

    if (!node->isLeaf)
    {
        for (int i = 0; i < 4; ++i)
        {
            QueryRecursive(node->children[i].get(), queryBounds, result);
        }
    }
}

std::vector<Collider*> Quadtree::GetPotentialCollisions(Collider* object)
{
    if (!object)
    {
        return {};
    }

    return Query(object->GetSpatialBounds());
}

std::vector<Bounds> Quadtree::GetAllNodeBounds() const
{
    std::vector<Bounds> allBounds;

    CollectAllNodeBoundsRecursive(root.get(), allBounds);

    return allBounds;
}

void Quadtree::ClearRecursiveObjects(QuadtreeNode* node)
{
    node->objects.clear();
    node->objectsLookup.clear();

    if (!node->isLeaf)
    {
        for (int i = 0; i < 4; ++i)
        {
            ClearRecursiveObjects(node->children[i].get());
        }
    }
}