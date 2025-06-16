#pragma once

#include "Component.h"

class Script :
	public Component
{
public:
	Script();
	~Script() override;

private:
	virtual void Start();
	virtual void Update();
	virtual void LateUpdate();

	friend class ScriptSystem;
};