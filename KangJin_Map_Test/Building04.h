#pragma once
#include "Structure.h"
class Building04 : public Structure
{
	void Start() override;
public:
	Building04(){ jsonfilepath = "Resource/building_circles4.json"; }
};

