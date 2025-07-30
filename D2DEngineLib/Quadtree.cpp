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
    allManagedObjects.clear(); // 관리하는 객체 목록도 초기화
}

void Quadtree::Insert(Collider* object)
{
    if (!object || !root) return;

    // 이미 삽입된 객체인지 확인 (중복 삽입 방지)
    // 실제 게임에서는 객체 ID를 사용하는 것이 더 효율적일 수 있습니다.
    bool alreadyExists = false;
    for (Collider* existingObj : allManagedObjects)
    {
        if (existingObj == object) // 포인터 주소로 동일성 판단
        {
            alreadyExists = true;
            break;
        }
    }

    if (!alreadyExists)
    {
        allManagedObjects.push_back(object);
    }

    InsertObjectRecursive(root.get(), object);
}

// 기존 InsertObjectRecursive 로직은 변경 없음
void Quadtree::InsertObjectRecursive(QuadtreeNode* node, Collider* object)
{
    if (!Bounds::IsOverlap(node->bounds, object->GetSpatialBounds()))
    {
        return;
    }

    if (node->isLeaf || node->depth >= maxDepth)
    {
        node->objects.push_back(object);

        if (node->objects.size() > maxObjectsPerNode && node->depth < maxDepth)
        {
            Subdivide(node);
        }

        return;
    }

    for (int i = 0; i < 4; ++i)
    {
        // 자식 노드가 생성되지 않은 경우 (Subdivide가 호출되지 않았을 때)
        // 여기에 직접 객체를 삽입하는 것이 아니라,
        // InsertObjectRecursive는 항상 자식 노드가 존재한다는 가정하에 호출되어야 합니다.
        // 이 부분은 Subdivide 로직에 의해 보장됩니다.
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
                if (node->children[i]->objects.size() > maxObjectsPerNode && node->children[i]->depth < maxDepth)
                {
                    Subdivide(node->children[i].get());
                }
            }
        }
    }
}

// --- 새롭게 추가된 Update 및 Remove 로직 ---

void Quadtree::Remove(Collider* object)
{
    if (!object || !root)
    {
        return;
    }

    // 1. allManagedObjects 리스트에서 객체 제거
    // std::remove_if는 조건에 맞는 요소를 끝으로 보내고 새 end 이터레이터를 반환합니다.
    // 그 다음 erase를 호출하여 실제로 제거합니다.
    auto it_managed = std::remove(allManagedObjects.begin(), allManagedObjects.end(), object);
    if (it_managed != allManagedObjects.end())
    {
        allManagedObjects.erase(it_managed, allManagedObjects.end());
    }
    else {
        // 객체가 allManagedObjects에 없으면 쿼드트리에도 없을 가능성이 높습니다.
        // 하지만 혹시 모를 상황을 대비해 계속 진행합니다.
        // std::cout << "Warning: Object to remove not found in allManagedObjects list." << std::endl;
    }

    // 2. 쿼드트리 노드들에서 객체 제거
    RemoveObjectRecursive(root.get(), object);
}

// 쿼드트리에 삽입된 모든 객체 중 dirty 상태인 객체들을 찾아 업데이트합니다.
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
    if (!node) return false;

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