#pragma once

#include "Object.h"
#include "Transform.h"
#include "Component.h"

class Script;

class GameObject :
    public Object
{
private:
	std::wstring m_name;
	std::unique_ptr<Transform> m_transform;
	std::vector<std::unique_ptr<Component>> m_components;
    bool m_isActiveSelf = true;
    bool m_isActiveInHierarchy = true;
    bool m_isInitialized = false;

public:
	GameObject(const std::wstring& name = L"GameObject");
	virtual ~GameObject();

public:
	Transform* GetTransform() const;
    const std::wstring& GetName() const;
    bool IsActive() const;
    bool IsActiveSelf() const;
    void SetActive(bool active);

private:
    void SetActiveInHierarchy(bool activeInHierarchy);

public:
    void Initialize();
    void CleanupDestroyedComponents();
    void CallComponentsInitialize();
    void RegisterComponentsToSystem();
    void UnregisterComponentsFromSystem();
    void CallComponentsOnDestroy(Script* script);

private:
    void Destroy() override;

public:
    template<typename T, typename... Args>
    T* AddComponent(Args&&... args)
    {
        assert(typeid(Transform) != typeid(T) && L"Transform 컴포넌트는 추가할 수 없습니다");
        static_assert(std::is_base_of<Component, T>::value, "Component가 아닙니다");

        std::unique_ptr<T> component = std::make_unique<T>(std::forward<Args>(args)...);

        component->SetOwner(this);


        if (m_isInitialized)
        {
            if (m_isActiveSelf && m_isActiveInHierarchy)
            {
                ((Component*)component.get())->Initialize();
                ((Component*)component.get())->RegisterToSystem();
            }
        }

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

public:
    static GameObject* Find(const std::wstring name);
};