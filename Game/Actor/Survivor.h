#pragma once

#include "Actor/Actor.h"
#include "Util/Timer.h"

#include <vector>

using namespace Wanted;

class Survivor: public Actor
{
	// RTTI 선언
	RTTI_DECLARATIONS(Survivor, Actor)

public:
	Survivor(const char* text, const Vector2& position, Color color);
	~Survivor();

	virtual void BeginPlay() override;
	virtual void Tick(float deltaTime) override;
	virtual void Draw() override;

	void MoveToExitOrTarget();

	void CommandMoveTo(const Vector2& targetPosition);

private:
	bool FindPath();

	bool FindPathToTarget(const Vector2& targetPosition);

	Color GetPathColor();

private:
	class AStar* astar = nullptr;

	std::vector<class Node*> path;

	// 목적지 도달 후 대기 시간
	Timer timer;

	// 플레이어가 직접 이동 명령을 내렸는지 저장하는 변수
	bool hasCommandMove = false;

	// 목적지에 도달했는지 확인하는 함수
	bool isReachTarget = false;

	// 명령한 목적지 좌표
	Vector2 commandTargetPosition;

	// 불을 피해 도망치는 중인지 상태를 저장하는 변수
	bool isNoExitAndRunAway = false;
};

