#pragma once
#include "../D2DEngineLib/Script.h"
class StructureCreater : public Script
{
public:
	StructureCreater() = default;
	StructureCreater(int num, float x, float y, int id) { CreateBuilding(num, x, y, id); }
	
	void CreateBuilding(int number, float x, float y, int id);
};

