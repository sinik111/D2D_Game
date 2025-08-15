#pragma once
#include "Structure.h"
class Building10 : public Structure
{
	void Start() override;
public:
	Building10() { jsonfilepath = "Resource/building_circles10.json"; }
};

