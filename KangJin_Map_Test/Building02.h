#pragma once
#include "Structure.h"
class Building02 : public Structure
{
	void Start() override;
public:
	 Building02(){ jsonfilepath = "Resource/building_circles02.json"; }
};

