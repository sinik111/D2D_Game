#pragma once
#include "Structure.h"

class Building08 : public Structure
{
	void Start() override;
public:
	Building08() { jsonfilepath = "Resource/building_circles8.json"; }
};
