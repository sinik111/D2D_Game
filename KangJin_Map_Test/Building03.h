#pragma once
#include "Structure.h"
class Building03 : public Structure
{
	void Start() override;
public:
	Building03() { jsonfilepath = "Resource/building_circles3.json"; }
};

