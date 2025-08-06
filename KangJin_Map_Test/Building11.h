#pragma once
#include "Structure.h"
class Building11 : public Structure
{
	void Start() override;
public:
	Building11() { jsonfilepath = "Resource/building_circles11.json"; }
};

