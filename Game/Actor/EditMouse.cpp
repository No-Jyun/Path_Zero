#include "EditMouse.h"
#include "Core/Input.h"
#include "Manager/MapManager.h"

EditMouse::EditMouse(const Vector2& position)
	: super(" ", position)
{
}

EditMouse::~EditMouse()
{
}

void EditMouse::Tick(float deltaTime)
{
	super::Tick(deltaTime);
	// 키보드 입력 처리
	/*
	"W키 : 생존자가 넘어갈 수 없는 벽을 생성합니다.",
	"F키 : 점점 넓어지는 불을 생성합니다 (최대 5개)",
	"E키 : 생존자가 탈출할 수 있는 탈출구를 생성합니다. (최대 3개)",
	"D키 : 선택한 영역을 빈 공간으로 변경합니다.",
	"R키 : 선택 영역이 초기화 됩니다.",
	"Q키 : 프로그램을 종료합니다.",
	*/
	// 게임 종료
	if (Input::Get().GetKeyDown('Q'))
	{
		QuitGame();
	}
	// 선택 영역 벽 생성
	if (Input::Get().GetKeyDown('W'))
	{
		MakeWall();
	}
	// 선택 영역 불 생성
	if (Input::Get().GetKeyDown('F'))
	{
		MakeFire();
	}
	// 선택 영역 탈출구 생성
	if (Input::Get().GetKeyDown('E'))
	{
		MakeExit();
	}
	// 선택 영역 빈 공간으로 초기화
	if (Input::Get().GetKeyDown('D'))
	{
		MakeEmpty();
	}
	// 선택 영역 초기화
	if (Input::Get().GetKeyDown('R'))
	{
		SelectPositionClear();
	}
}

void EditMouse::MakeWall()
{
	for (const Vector2& selPos : selectedPositionInConsole)
	{
		// Todo: 현재 변경하는 타일 확인 필요
		MapManager::Get().SetMapTile(selPos, '#');
	}

	SelectPositionClear();
}

void EditMouse::MakeFire()
{
	for (const Vector2& selPos : selectedPositionInConsole)
	{
		// Todo: 현재 변경하는 타일 확인 필요
		// Todo: 불 타일 개수 확인 필요
		MapManager::Get().SetMapTile(selPos, 'F');
	}

	SelectPositionClear();
}

void EditMouse::MakeExit()
{
	for (const Vector2& selPos : selectedPositionInConsole)
	{
		// Todo: 현재 변경하는 타일 확인 필요
		// Todo: 탈출구 타일 개수 확인 필요
		MapManager::Get().SetMapTile(selPos, 'X');
	}

	SelectPositionClear();
}

void EditMouse::MakeEmpty()
{
	for (const Vector2& selPos : selectedPositionInConsole)
	{
		// Todo: 현재 변경하는 타일 확인 필요
		MapManager::Get().SetMapTile(selPos, ' ');
	}

	SelectPositionClear();
}
