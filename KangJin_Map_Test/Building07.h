#pragma once
#include "Structure.h"
class Building07 : public Structure
{
	void Start() override;
public:
	Building07() { jsonfilepath = "Resource/building_circles7.json"; }
};

