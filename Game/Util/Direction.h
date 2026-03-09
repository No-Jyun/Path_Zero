#pragma once
#include "Math/Vector2.h"

using namespace Wanted;

struct Direction
{
	static const int direction8Length = 8;

	static const Vector2 eightDirection[direction8Length];

	static const int direction4Length = 4;

	static const Vector2 fourDirection[direction4Length];

	static const int directionFireLength = 6;

	static const Vector2 fireDirection[directionFireLength];

	static const float directionCost[direction8Length];
};
