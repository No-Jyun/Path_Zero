#include "EscapeMouse.h"
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

		Game::Get().ToggleMenu(0);
	}

}
