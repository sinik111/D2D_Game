#pragma once
#include "IngameGaugeController.h"
class HealthGaugeController :
	public IngameGaugeController
{
private:
	void Start() override;
public:

	// TODO : 게이지 바 비트맵 받아와서 setfill 해주기
};

