#pragma once

#include "Object.h"
#include "Transform.h"
#include "Component.h"

class Script;
class Maps;
class GameObject :
    public Object
{
private:
	std::wstring m_name;
	std::unique_ptr<Transform> m_transform;
	std::vector<std::unique_ptr<Component>> m_components;
    std::vector<Component*> m_addedComponents;
    bool m_isInitialized = false;
    bool m_hasDestroyedComponent = false;

public:
	GameObject(const std::wstring& name = L"GameObject");
	~GameObject() override;

public:
	Transform* GetTransform() const;
    const std::wstring& GetName() const;
    void CleanupDestroyedComponents();
    bool HasAddedComponent() const;
    bool HasDestroyedComponents() const;
    void Initialize();
    void InitializeAddedComponents();
    void MarkComponentDestroyed();

private:
    void Destroy() override;

public:
    template<typename T, typename... Args>
    T* AddComponent(Args&&... args)
    {
        static_assert(std::is_base_of<Component, T>::value, "Component가 아닙니다");
        static_assert(!std::is_same<Transform, T>::value, "Transform 컴포넌트는 추가할 수 없습니다");

        std::unique_ptr<T> component = std::make_unique<T>(std::forward<Args>(args)...);

        component->SetOwner(this);

        if (m_isInitialized)
        {
            m_addedComponents.push_back(component.get());
        }

        m_components.push_back(std::move(component));

        return static_cast<T*>(m_components.back().get());
    }

    template<typename T>
    T* GetComponent() const
    {
        for (const auto& comp : m_components)
        {
            auto component = dynamic_cast<T*>(comp.get());

            if (component)
            {
                return component;
            }
        }

        return nullptr;
    }

    template<typename T>
    std::vector<T*> GetComponents() const
    {
        std::vector<T*> components;

        for (const auto& comp : m_components)
        {
            auto component = dynamic_cast<T*>(comp.get());

            if (component)
            {
                components.push_back(component);
            }
        }

        return components;
    }

public:
    static GameObject* Find(const std::wstring name);

    //friend class Maps;
};