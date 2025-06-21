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
    // Class T를 생성하는 함수 , 인자까지 전달한다.
    template<typename T, typename... Args>
    T* AddComponent(Args&&... args)
    {
        if (typeid(Transform) == typeid(T))
        {
            return (T*)m_transform.get();
        }

        // 컴파일 시점에 T가 Component를 상속받은 클래스 인지 확인
        static_assert(std::is_base_of<Component, T>::value, "T must derive from Component");

        // 인자까지 전달하면서 생성
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
            // 실행도중(Runtime)에 comp가 가르키는 인스턴스가 RTTI정보 이용하여 
            // T이거나 T의 자식 클래스 이면 주소를 리턴한다. 클래스가 가상 함수를 하나라도 가지면,
            // 컴파일러는 해당 클래스에 대해 vtable + RTTI(Run-Time Type Information, 런타임 타입 정보)
            // 정보 블록을 생성합니다.

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
            if (typeid(*(iter->get())) == typeid(T)) //완전히 동일한 타입만
            {
                return static_cast<T*>(iter->get()); //안전하게 static_cast
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