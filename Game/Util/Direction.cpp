#include "Direction.h"
#include "Util/Util.h"

// 대각선 방향 비용
const float diagnalCost = 1.41421345f;

// 8방향 (콘솔 좌표 기준으로 북/북동/동/남동/남/남서/서/북서)
const Vector2 Direction::eightDirection[directionLength] = {
    Vector2(0, -1), Vector2(1, -1), Vector2(1, 0), Vector2(1, 1),
    Vector2(0, 1), Vector2(-1, 1), Vector2(-1, 0), Vector2(-1, -1)
};

// 8방향 별 이동 비용
const float Direction::directionCost[directionLength]
{
    1.0f, diagnalCost, 1.0f, diagnalCost,
    1.0f, diagnalCost, 1.0f, diagnalCost
};