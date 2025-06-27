#pragma once

#include "Enum.h"

class IRenderer
{
public:
	virtual SpaceType GetSpaceType() const = 0;
	virtual int GetSortOrder() const = 0;
};