#pragma once

#include "Actor/Actor.h"
#include "Util/Timer.h"

#include <vector>

using namespace Wanted;

class Survivor: public Actor
{
	// RTTI ¼±¾ð
	RTTI_DECLARATIONS(Survivor, Actor)

public:
	Survivor(const Vector2& position, Color color);
	~Survivor();

	virtual void Tick(float deltaTime) override;
	virtual void Draw() override;

	void Move();

private:
	bool FindPath();

	Color GetPathColor();

private:
	class AStar* astar = nullptr;

	std::vector<class Node*> path;

	Timer timer;
};

