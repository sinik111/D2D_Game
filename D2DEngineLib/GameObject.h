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
    template<typename T, typename... Args>
    T* AddComponent(Args&&... args)
    {
        if (typeid(Transform) == typeid(T))
        {
            return (T*)m_transform.get();
        }

        static_assert(std::is_base_of<Component, T>::value, "T must derive from Component");

        std::unique_ptr<T> component = std::make_unique<T>(std::forward<Args>(args)...);

        component->SetOwner(this);

        m_components.push_back(std::move(component));

        return static_cast<T*>(m_components.back().get());
    }

    template<typename T>
    T* GetComponent()
    {
        for (const auto& comp : m_components)
        {
            if (typeid(*(comp.get())) == typeid(T))
            {
                return static_cast<T*>(comp.get());
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

public:
    static GameObject* Find(const std::wstring name);
    static bool IsValid(GameObject* gameObject);
};