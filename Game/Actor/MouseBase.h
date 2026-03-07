#pragma once
#include "Actor/Actor.h"
#include "Math/Vector2.h"

#include <vector>

using namespace Wanted;

class MouseBase : public Actor
{
	// RTTI 선언
	RTTI_DECLARATIONS(MouseBase, Actor)

public:
	MouseBase(const Vector2& position = Vector2::Zero);
	virtual ~MouseBase();

	virtual void Tick(float deltaTime) override;
	virtual void Draw() override;

protected:
	// 현재 선택한 영역을 초기화하는 함수
	void SelectPositionClear();

	// 드래그 영역을 배열에 저장하는 함수
	void DragProcess();

protected:
	Vector2 firstSelectedPosition;
	Vector2 lastSelectedPosition;

	bool isFirstClicked = true;

	// 마우스 클릭/드래그로 선택한 위치 배열
	std::vector<Vector2> selectedPositionInConsole;
};

