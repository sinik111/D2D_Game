#pragma once
#include "Structure.h"
class Building09 : public Structure
{
	void Start() override;
public:
	Building09() { jsonfilepath = "Resource/building_circles9.json"; }
};

