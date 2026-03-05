#pragma once
#include "Math/Vector2.h"

using namespace Wanted;

struct Direction
{
	static const int directionLength = 8;

	static const Vector2 eightDirection[directionLength];

	static const float directionCost[directionLength];
};
