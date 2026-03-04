#pragma once
#include "Actor/Actor.h"

class MouseBase : public Wanted::Actor
{
	// RTTI ¼±¾ð
	RTTI_DECLARATIONS(MouseBase, Actor)

public:
	MouseBase();
	virtual ~MouseBase();

	virtual void Tick(float deltaTime) override;
	virtual void Draw() override;

private:
	void SelectPositionClear();

	void DragProcess();

private:
	Vector2 firstSelectedPosition;
	Vector2 lastSelectedPosition;

	bool isFirstClicked = true;

	std::vector<Vector2> selectedPosition;
};

