#include "pch.h"
#include "PlayerInput.h"

#include "ComponentSystem.h"

PlayerInput::PlayerInput()
{
	ComponentSystem::Get().PlayerInput().Register(this);
}

PlayerInput::~PlayerInput()
{
	ComponentSystem::Get().PlayerInput().Unregister(this);
}

void PlayerInput::UnregisterActionOnKey(short vKey, void* instance)
{
    m_singleKeyActions.erase(
        std::remove_if(
            m_singleKeyActions.begin(),
            m_singleKeyActions.end(),
            [vKey, instance](const SingleKeyActionData& entry) {
                return entry.vKey == vKey && entry.instance == instance;
            }),
        m_singleKeyActions.end()
    );
}

void PlayerInput::UnregisterActionOnKey(short vKey, InputCheckType checkType, void* instance)
{
    m_singleKeyActions.erase(
        std::remove_if(
            m_singleKeyActions.begin(),
            m_singleKeyActions.end(),
            [vKey, checkType, instance](const SingleKeyActionData& entry) {
                return entry.vKey == vKey && entry.checkType == checkType && entry.instance == instance;
            }),
        m_singleKeyActions.end()
    );
}

void PlayerInput::UnregisterCombinedAction(const std::vector<short>& vKeys, void* instance)
{
    m_combinedKeyActions.erase(
        std::remove_if(
            m_combinedKeyActions.begin(),
            m_combinedKeyActions.end(),
            [vKeys, instance](const CombinedKeyActionData& entry) {
                return entry.vKeys == vKeys && entry.instance == instance;
            }),
        m_combinedKeyActions.end()
    );
}

void PlayerInput::UnregisterCombinedAction(const std::vector<short>& vKeys, InputCheckType checkType, void* instance)
{
    m_combinedKeyActions.erase(
        std::remove_if(
            m_combinedKeyActions.begin(),
            m_combinedKeyActions.end(),
            [vKeys, checkType, instance](const CombinedKeyActionData& entry) {
                return entry.vKeys == vKeys && entry.checkType == checkType && entry.instance == instance;
            }),
        m_combinedKeyActions.end()
    );
}

void PlayerInput::UnregisterDirectionActions(void* instance)
{
    m_directionDelegateWASD.Remove(instance);
    m_directionDelegateArrow.Remove(instance);
}

void PlayerInput::CallActions()
{
    for (const auto& data : m_singleKeyActions)
    {
        bool shouldCall{ false };

        switch (data.checkType)
        {
        case InputCheckType::Pressed:
            if (ComponentSystem::Get().PlayerInput().IsKeyPressed(data.vKey))
            {
                shouldCall = true;
            }
            break;
        case InputCheckType::Released:
            if (ComponentSystem::Get().PlayerInput().IsKeyReleased(data.vKey))
            {
                shouldCall = true;
            }
            break;
        case InputCheckType::Held:
            if (ComponentSystem::Get().PlayerInput().IsKeyHeld(data.vKey))
            {
                shouldCall = true;
            }
            break;
        }

        if (shouldCall && data.action)
        {
            data.action();
        }
    }

    for (const auto& data : m_combinedKeyActions)
    {
        bool allKeysMatch{ true };
        for (short vKey : data.vKeys)
        {
            if (!ComponentSystem::Get().PlayerInput().IsKeyHeld(vKey))
            {
                allKeysMatch = false;
                break;
            }
        }

        if (allKeysMatch)
        {
            short lastKey{ data.vKeys.back() };

            bool shouldCallCombined{ false };
            switch (data.checkType)
            {
            case InputCheckType::Pressed:
                if (ComponentSystem::Get().PlayerInput().IsKeyPressed(lastKey))
                {
                    shouldCallCombined = true;
                }
                break;
            case InputCheckType::Held:
                if (ComponentSystem::Get().PlayerInput().IsKeyHeld(lastKey))
                {
                    shouldCallCombined = true;
                }
                break;
            case InputCheckType::Released:
                if (ComponentSystem::Get().PlayerInput().IsKeyReleased(lastKey))
                {
                    shouldCallCombined = true;
                }
                break;
            }

            if (shouldCallCombined && data.action)
            {
                data.action();
            }
        }
    }
}

void PlayerInput::CallArrowAction(float horizontal, float vertical)
{
    if (m_directionInputType == DirectionInputType::Arrow || m_directionInputType == DirectionInputType::Both)
    {
        if (horizontal != 0 || vertical != 0)
        {
            m_directionDelegateArrow.Invoke(Vector2(horizontal, vertical));
        }
    }
}

void PlayerInput::CallWASDAction(float horizontal, float vertical)
{
    if (m_directionInputType == DirectionInputType::WASD || m_directionInputType == DirectionInputType::Both)
    {
        if (horizontal != 0 || vertical != 0)
        {
            m_directionDelegateWASD.Invoke(Vector2(horizontal, vertical));
        }
    }
}

Vector2 PlayerInput::GetMousePosition() const
{
	return m_mousePosition;
}

void PlayerInput::SetMousePosition(float x, float y)
{
	m_mousePosition = Vector2(x, y);
}