#pragma once
#include "Component.h"
#include "Delegate.h"
#include "Physics.h"

class RigidBody2D;
class Script;
class CircleCollider : public Component
{
	Transform* m_transform = nullptr;
	RigidBody2D* m_rigidBody2D = nullptr;

	std::vector<Script*> m_scriptsForCallBack;

public:
	CircleCollider() = default;
	~CircleCollider() = default;

public :
	void Initialize() override;
	void RegisterToSystem() override;
	void UnregisterFromSystem() override;

	void RegisterScript(Script* script);
	void UnregisterScript(Script* script);

public:
	RigidBody2D* GetRigidBody2D() const;

public:
	void Update();

	
};

