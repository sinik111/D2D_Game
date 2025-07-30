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
    allManagedObjects.clear(); // �����ϴ� ��ü ��ϵ� �ʱ�ȭ
}

void Quadtree::Insert(Collider* object)
{
    if (!object || !root) return;

    // �̹� ���Ե� ��ü���� Ȯ�� (�ߺ� ���� ����)
    // ���� ���ӿ����� ��ü ID�� ����ϴ� ���� �� ȿ������ �� �ֽ��ϴ�.
    bool alreadyExists = false;
    for (Collider* existingObj : allManagedObjects)
    {
        if (existingObj == object) // ������ �ּҷ� ���ϼ� �Ǵ�
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

// ���� InsertObjectRecursive ������ ���� ����
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
        // �ڽ� ��尡 �������� ���� ��� (Subdivide�� ȣ����� �ʾ��� ��)
        // ���⿡ ���� ��ü�� �����ϴ� ���� �ƴ϶�,
        // InsertObjectRecursive�� �׻� �ڽ� ��尡 �����Ѵٴ� �����Ͽ� ȣ��Ǿ�� �մϴ�.
        // �� �κ��� Subdivide ������ ���� ����˴ϴ�.
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

// --- ���Ӱ� �߰��� Update �� Remove ���� ---

void Quadtree::Remove(Collider* object)
{
    if (!object || !root)
    {
        return;
    }

    // 1. allManagedObjects ����Ʈ���� ��ü ����
    // std::remove_if�� ���ǿ� �´� ��Ҹ� ������ ������ �� end ���ͷ����͸� ��ȯ�մϴ�.
    // �� ���� erase�� ȣ���Ͽ� ������ �����մϴ�.
    auto it_managed = std::remove(allManagedObjects.begin(), allManagedObjects.end(), object);
    if (it_managed != allManagedObjects.end())
    {
        allManagedObjects.erase(it_managed, allManagedObjects.end());
    }
    else {
        // ��ü�� allManagedObjects�� ������ ����Ʈ������ ���� ���ɼ��� �����ϴ�.
        // ������ Ȥ�� �� ��Ȳ�� ����� ��� �����մϴ�.
        // std::cout << "Warning: Object to remove not found in allManagedObjects list." << std::endl;
    }

    // 2. ����Ʈ�� ���鿡�� ��ü ����
    RemoveObjectRecursive(root.get(), object);
}

// ����Ʈ���� ���Ե� ��� ��ü �� dirty ������ ��ü���� ã�� ������Ʈ�մϴ�.
void Quadtree::Update()
{
    std::vector<Collider*> dirtyObjectsToReinsert;

    // ��� ��带 ��ȸ�ϸ� dirty ��ü�� ã�� ����
    std::function<void(QuadtreeNode*)> findAndRemoveDirty =
        [&](QuadtreeNode* node) {
            if (!node)
            {
                return;
            }

            // ���� ����� ��ü ��Ͽ��� dirty ��ü���� ã���ϴ�.
            node->objects.erase(
                std::remove_if(node->objects.begin(), node->objects.end(),
                    [&](Collider* obj) {
                        if (obj->GetTransform()->IsDirtyThisFrame())
                        {
                            dirtyObjectsToReinsert.push_back(obj);

                            return true; // ������ ��ü���� ǥ��
                        }

                        return false;
                    }),
                node->objects.end()
            );

            // �ڽ� ��� ��ȸ
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

    // ��Ʈ���� ��ȸ �����Ͽ� dirty ��ü���� �����ϰ� ����
    if (root)
    {
        findAndRemoveDirty(root.get());
    }

    // ������ dirty ��ü���� ����Ʈ���� ������մϴ�.
    for (Collider* obj : dirtyObjectsToReinsert)
    {
        InsertObjectRecursive(root.get(), obj); // ����� �ٿ�带 �������� �ٽ� ����
    }
}

// Ư�� ��忡�� ��ü�� �����ϴ� ��� �Լ�
// ��ü�� �߰ߵǾ� ���ŵǸ� true�� ��ȯ�մϴ�.
bool Quadtree::RemoveObjectRecursive(QuadtreeNode* node, Collider* object)
{
    if (!node) return false;

    // ��ü�� ���� ����� ���� ���� �ִ��� Ȯ�� (����ȭ)
    if (!Bounds::IsOverlap(node->bounds, object->GetSpatialBounds()))
    {
        return false;
    }

    // ���� ����� ��ü ��Ͽ��� ����
    auto it = std::remove(node->objects.begin(), node->objects.end(), object);
    if (it != node->objects.end())
    {
        node->objects.erase(it, node->objects.end());

        return true; // ���� ��忡�� ��ü�� ã�� ��������
    }

    // ���� ��尡 �ƴϸ� �ڽ� ��忡�� Ž�� �� ���� �õ�
    if (!node->isLeaf)
    {
        for (int i = 0; i < 4; ++i)
        {
            if (node->children[i] && RemoveObjectRecursive(node->children[i].get(), object)) {
                return true; // �ڽ� ��忡�� ��ü�� ���������� �� �̻� Ž���� �ʿ� ����
            }
        }
    }
    return false; // ��ü�� ã�� ������
}

void Quadtree::CollectAllNodeBoundsRecursive(const QuadtreeNode* node, std::vector<Bounds>& allBounds) const
{
    if (!node)
    {
        return;
    }

    // ���� ����� bounds�� ��� ���Ϳ� �߰��մϴ�.
    allBounds.push_back(node->bounds);

    // ���� ��尡 �ƴϸ� �ڽ� ������ ��������� ��ȸ�մϴ�.
    if (!node->isLeaf)
    {
        for (int i = 0; i < 4; ++i)
        {
            if (node->children[i]) // ��ȿ�� �ڽ� ����� ��쿡��
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