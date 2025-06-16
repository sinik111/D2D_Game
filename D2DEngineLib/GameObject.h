#pragma once

#include "Transform.h"
#include "Component.h"

class GameObject
{
private:
	std::wstring m_name;
	std::unique_ptr<Transform> m_transform;
	std::vector<std::unique_ptr<Component>> m_components;

public:
	GameObject();
	virtual ~GameObject();

public:
	Transform* GetTransform();

    // Class T�� �����ϴ� �Լ� , ���ڱ��� �����Ѵ�.
    template<typename T, typename... Args>
    T* AddComponent(Args&&... args)
    {
        if (typeid(Transform) == typeid(T))
        {
            return m_transform;
        }

        // ������ ������ T�� Component�� ��ӹ��� Ŭ���� ���� Ȯ��
        static_assert(std::is_base_of<Component, T>::value, "T must derive from Component");

        // ���ڱ��� �����ϸ鼭 ����
        T* component = new T(std::forward<Args>(args)...);

        component->SetOwner(this);

        m_components.push_back(component);

        component->OnEnable();

        return component;
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
            if (iter->get() == target) //������ ������ Ÿ�Ը�
            {
                (*iter)->OnDestroy();

                m_components.erase(iter);
                return true;
            }
        }

        return false;
    }
};