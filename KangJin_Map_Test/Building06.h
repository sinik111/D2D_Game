#pragma once
#include "Structure.h"
class Building06 : public Structure
{
	void Start() override;
public:
	Building06() { jsonfilepath = "Resource/building_circles6.json"; }
};

