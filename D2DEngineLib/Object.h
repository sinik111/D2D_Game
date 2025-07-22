#pragma once

class Object
{
private:
	static std::unordered_set<Object*> s_validObjects;
	static std::vector<std::pair<Object*, float>> s_delayDestroyObjects;

	static float s_delayDestroyTimer;
	bool m_isDestroyed = false;
	bool m_isDelayedDestroy = false;

protected:
	Object();
	virtual ~Object();

public:
	static bool IsValid(Object* object);
	virtual void Destroy(Object* object, float delay = 0.0f);

	static void UpdateDelayDestroy();
	static void ClearDelayDestroyObjects();

private:
	virtual void Destroy() = 0;

	// ������ �������� ���� �����ϰ� �����
	friend class Scene;
	friend class GameObject;
	friend class Component;
};