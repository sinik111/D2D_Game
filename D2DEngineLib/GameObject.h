#pragma once

#include "Transform.h"
#include "Component.h"

class GameObject
{
private:
	std::wstring m_name;
	std::unique_ptr<Transform> m_transform;
	std::vector<std::unique_ptr<Component>> m_components;

    bool m_isDestroyed = false;

public:
	GameObject(const std::wstring& name = L"GameObject");
	virtual ~GameObject() = default;

public:
	Transform* GetTransform() const;
    const std::wstring& GetName() const;
    void Destroy();
    bool IsDestroyed() const;

public:
    // Class T�� �����ϴ� �Լ� , ���ڱ��� �����Ѵ�.
    template<typename T, typename... Args>
    T* AddComponent(Args&&... args)
    {
        if (typeid(Transform) == typeid(T))
        {
            return (T*)m_transform.get();
        }

        // ������ ������ T�� Component�� ��ӹ��� Ŭ���� ���� Ȯ��
        static_assert(std::is_base_of<Component, T>::value, "T must derive from Component");

        // ���ڱ��� �����ϸ鼭 ����
        std::unique_ptr<T> component{ std::make_unique<T>(std::forward<Args>(args)...) };

        component->SetOwner(this);

        m_components.push_back(std::move(component));

        return static_cast<T*>(m_components.back().get());
    }

    template<typename T>
    std::vector<T*> GetComponents()
    {
        std::vector<T*> result;

        for (auto iter = m_components.begin(); iter != m_components.end(); ++iter)
        {
            // ���൵��(Runtime)�� comp�� ����Ű�� �ν��Ͻ��� RTTI���� �̿��Ͽ� 
            // T�̰ų� T�� �ڽ� Ŭ���� �̸� �ּҸ� �����Ѵ�. Ŭ������ ���� �Լ��� �ϳ��� ������,
            // �����Ϸ��� �ش� Ŭ������ ���� vtable + RTTI(Run-Time Type Information, ��Ÿ�� Ÿ�� ����)
            // ���� ����� �����մϴ�.

            if (auto casted = dynamic_cast<T*>(iter->get()))
            {
                result.push_back(casted);
            }
        }

        return result;
    }

    template<typename T>
    T* GetComponent()
    {
        for (auto iter = m_components.begin(); iter != m_components.end(); ++iter)
        {
            if (typeid(*(iter->get())) == typeid(T)) //������ ������ Ÿ�Ը�
            {
                return static_cast<T*>(iter->get()); //�����ϰ� static_cast
            }
        }

        return nullptr;
    }

    template<typename T>
    bool RemoveComponent(T* target)
    {
        for (auto iter = m_components.begin(); iter != m_components.end(); ++iter)
        {
            if (iter->get() == target)
            {
                m_components.erase(iter);

                return true;
            }
        }

        return false;
    }
};