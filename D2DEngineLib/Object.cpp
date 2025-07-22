#include "pch.h"
#include "Object.h"

std::unordered_set<Object*> Object::s_validObjects;
std::vector<std::pair<Object*, float>> Object::s_delayDestroyObjects;
float Object::s_delayDestroyTimer = 0.0f;

Object::Object()
{
	s_validObjects.insert(this);
}

Object::~Object()
{
	if (m_isDelayedDestroy)
	{
		for (auto iter = s_delayDestroyObjects.begin(); iter != s_delayDestroyObjects.end(); ++iter)
		{
			if (iter->first == this)
			{
				std::swap(*iter, s_delayDestroyObjects.back());

				s_delayDestroyObjects.pop_back();

				break;
			}
		}
	}

	s_validObjects.erase(this);
}

bool Object::IsValid(Object* object)
{
	return object == nullptr ? false : (s_validObjects.find(object) != s_validObjects.end());
}

void Object::Destroy(Object* object, float delay)
{
	if (delay == 0.0f)
	{
		object->Destroy();
	}
	else
	{
		m_isDelayedDestroy = true;
		s_delayDestroyObjects.emplace_back(object, s_delayDestroyTimer + delay);
	}
}

void Object::UpdateDelayDestroy()
{
	s_delayDestroyTimer += MyTime::DeltaTime();

	for (size_t i = 0; i < s_delayDestroyObjects.size(); )
	{
		if (s_delayDestroyObjects[i].second <= s_delayDestroyTimer)
		{
			s_delayDestroyObjects[i].first->Destroy();

			std::swap(s_delayDestroyObjects[i], s_delayDestroyObjects.back());

			s_delayDestroyObjects.pop_back();

			continue;
		}

		++i;
	}
}

void Object::ClearDelayDestroyObjects()
{
	s_delayDestroyObjects.clear();
}