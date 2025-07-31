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
    if (root)
    {
        ClearRecursiveObjects(root.get());
        root->objects.clear();
        for (int i = 0; i < 4; ++i)
        {
            root->children[i].reset();
        }
        root->isLeaf = true;
    }
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
        node->objects.push_back(object);
        
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

    for (Collider* obj : oldObjects)
    {
        for (int i = 0; i < 4; ++i)
        {
            if (node->children[i] && Bounds::IsOverlap(node->children[i]->bounds, obj->GetSpatialBounds()))
            {
                node->children[i]->objects.push_back(obj);

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

                    break;
                }
            }
        }
    }

    nodes.clear();

    Insert(object);
}

void Quadtree::Update()
{
    std::vector<Collider*> dirtyObjectsToReinsert;

    // 모든 노드를 순회하며 dirty 객체를 찾고 제거
    std::function<void(QuadtreeNode*)> findAndRemoveDirty =
        [&](QuadtreeNode* node) {
            if (!node)
            {
                return;
            }

            // 현재 노드의 객체 목록에서 dirty 객체들을 찾습니다.
            node->objects.erase(
                std::remove_if(node->objects.begin(), node->objects.end(),
                    [&](Collider* obj) {
                        if (obj->GetTransform()->IsDirtyThisFrame())
                        {
                            dirtyObjectsToReinsert.push_back(obj);

                            return true; // 제거할 객체임을 표시
                        }

                        return false;
                    }),
                node->objects.end()
            );

            // 자식 노드 순회
            if (!node->isLeaf)
            {
                for (int i = 0; i < 4; ++i)
                {
                    if (node->children[i])
                    {
                        findAndRemoveDirty(node->children[i].get());
                    }
                }
            }
        };

    // 루트부터 순회 시작하여 dirty 객체들을 수집하고 제거
    if (root)
    {
        findAndRemoveDirty(root.get());
    }

    // 수집된 dirty 객체들을 쿼드트리에 재삽입합니다.
    for (Collider* obj : dirtyObjectsToReinsert)
    {
        InsertObjectRecursive(root.get(), obj); // 변경된 바운드를 기준으로 다시 삽입
    }
}

// 특정 노드에서 객체를 제거하는 재귀 함수
// 객체가 발견되어 제거되면 true를 반환합니다.
bool Quadtree::RemoveObjectRecursive(QuadtreeNode* node, Collider* object)
{
    if (!node)
    {
        return false;
    }

    // 객체가 현재 노드의 영역 내에 있는지 확인 (최적화)
    if (!Bounds::IsOverlap(node->bounds, object->GetSpatialBounds()))
    {
        return false;
    }

    // 현재 노드의 객체 목록에서 제거
    auto it = std::remove(node->objects.begin(), node->objects.end(), object);
    if (it != node->objects.end())
    {
        node->objects.erase(it, node->objects.end());

        return true; // 현재 노드에서 객체를 찾고 제거했음
    }

    // 리프 노드가 아니면 자식 노드에서 탐색 및 제거 시도
    if (!node->isLeaf)
    {
        for (int i = 0; i < 4; ++i)
        {
            if (node->children[i] && RemoveObjectRecursive(node->children[i].get(), object)) {
                return true; // 자식 노드에서 객체를 제거했으면 더 이상 탐색할 필요 없음
            }
        }
    }
    return false; // 객체를 찾지 못했음
}

void Quadtree::CollectAllNodeBoundsRecursive(const QuadtreeNode* node, std::vector<Bounds>& allBounds) const
{
    if (!node)
    {
        return;
    }

    // 현재 노드의 bounds를 결과 벡터에 추가합니다.
    allBounds.push_back(node->bounds);

    // 리프 노드가 아니면 자식 노드들을 재귀적으로 순회합니다.
    if (!node->isLeaf)
    {
        for (int i = 0; i < 4; ++i)
        {
            if (node->children[i]) // 유효한 자식 노드인 경우에만
            {
                CollectAllNodeBoundsRecursive(node->children[i].get(), allBounds);
            }
        }
    }
}

std::vector<Collider*> Quadtree::Query(const Bounds& queryBounds)
{
    std::vector<Collider*> result;
    if (root)
    {
        QueryRecursive(root.get(), queryBounds, result);
    }

    std::sort(result.begin(), result.end());
    result.erase(std::unique(result.begin(), result.end()), result.end());

    return result;
}

void Quadtree::QueryRecursive(QuadtreeNode* node, const Bounds& queryBounds, std::vector<Collider*>& result)
{
    if (!node || !Bounds::IsOverlap(node->bounds, queryBounds))
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
            if (node->children[i])
            {
                QueryRecursive(node->children[i].get(), queryBounds, result);
            }
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
    if (root)
    {
        CollectAllNodeBoundsRecursive(root.get(), allBounds);
    }
    return allBounds;
}

void Quadtree::ClearRecursiveObjects(QuadtreeNode* node)
{
    if (!node)
    {
        return;
    }

    node->objects.clear();

    if (!node->isLeaf)
    {
        for (int i = 0; i < 4; ++i)
        {
            if (node->children[i])
            {
                ClearRecursiveObjects(node->children[i].get());
            }
        }
    }
}