#pragma once

class Object
{
private:
	static std::unordered_set<Object*> s_validObjects;

	bool m_isDestroyed = false;

protected:
	Object();
	virtual ~Object();

public:
	static bool IsValid(Object* object);
	virtual void Destroy(Object* object);

private:
	virtual void Destroy() = 0;

	// 지정한 곳에서만 삭제 가능하게 열어둠
	friend class Scene;
	friend class GameObject;
	friend class Component;
};