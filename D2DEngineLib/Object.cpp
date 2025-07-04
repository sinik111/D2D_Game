#include "pch.h"
#include "Object.h"

std::unordered_set<Object*> Object::s_validObjects;

Object::Object()
{
	s_validObjects.insert(this);
}

Object::~Object()
{
	s_validObjects.erase(this);
}

bool Object::IsValid(Object* object)
{
	return object == nullptr ? false : (s_validObjects.find(object) != s_validObjects.end());
}

void Object::Destroy(Object* object)
{
	object->Destroy();
}