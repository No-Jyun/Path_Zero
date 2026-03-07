#include "EscapeMouse.h"
#include "Actor/Survivor.h"
#include "Core/Input.h"
#include "Game/Game.h"

EscapeMouse::EscapeMouse(const Vector2& position)
	: super(position)
{
}

EscapeMouse::~EscapeMouse()
{
}

void EscapeMouse::Tick(float deltaTime)
{
	super::Tick(deltaTime);

	// Todo: 임시 코드
	// 엔터 입력시 맵 토글
	if (Input::Get().GetKeyDown(VK_RETURN))
	{
		SelectPositionClear();

		Game::Get().ToggleMenu(0);
	}

	// 마우스 좌클릭을 뗐을 때
	if (Input::Get().GetMouseButtonUp(0))
	{
		// 선택 영역 초기화
		std::vector<Survivor*>().swap(selectedSurvivor);

		// Todo: 선택 영역을 확인해서 생존자 배열에 추가
		
	}
}
