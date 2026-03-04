#include "MouseBase.h"
#include "Render/Renderer.h"
#include "Core/Input.h"
#include "Game/Game.h"
#include "Manager/MapManager.h"

#include <cmath>
#include <vector>

MouseBase::MouseBase(const char* image,	const Vector2& position)
	: super(image, position)
{
	sortingOrder = 3;
	SelectPositionClear();
}

MouseBase::~MouseBase()
{
}

void MouseBase::Tick(float deltaTime)
{
	super::Tick(deltaTime);

	// 마우스 입력 처리
	if (Input::Get().GetMouseButton(0))
	{
		Vector2 mousePosition = Input::Get().MousePosition();
		char buffer[256] = {};
		sprintf_s(
			buffer,
			256,
			"현재 커서 위치 : (%d, %d)",
			mousePosition.x,
			mousePosition.y
		);

		ChangeImage(buffer);

		// 첫번째 클릭이면
		if (isFirstClicked)
		{
			isFirstClicked = false;
			firstSelectedPosition = mousePosition;
		}

		lastSelectedPosition = mousePosition;

		DragProcess();
	}

	if (Input::Get().GetMouseButtonUp(0))
	{
		isFirstClicked = true;
	}
}

void MouseBase::Draw()
{
	super::Draw();

	for (const Vector2& selPos : selectedPosition)
	{
		Renderer::Get().Submit(" ", selPos, Color::BackGoundWhite, sortingOrder + 3);
	}
}

void MouseBase::SelectPositionClear()
{
	firstSelectedPosition = Vector2::Zero;
	lastSelectedPosition = Vector2::Zero;

	selectedPosition.clear();
}

void MouseBase::DragProcess()
{
	// 현재 배열 비우기
	selectedPosition.clear();

	// x(Row), y(Col)의 시작과 끝을 계산 (어느 방향으로 드래그해도 대응 가능하게)
	int startX = min(firstSelectedPosition.x, lastSelectedPosition.x);
	int endX = max(firstSelectedPosition.x, lastSelectedPosition.x);
	int startY = min(firstSelectedPosition.y, lastSelectedPosition.y);
	int endY = max(firstSelectedPosition.y, lastSelectedPosition.y);

	// 맵의 최대 크기 (경계 검사용)
	int maxRow = MapManager::Get().GetMapWidth();
	int maxCol = MapManager::Get().GetMapHeight();

	// 루프 돌며 영역 선택
	for (int x = startX; x <= endX; ++x)
	{
		// 맵 범위 밖으로 나가는 것 방지
		if (x < 0 || x >= maxRow) continue;

		for (int y = startY; y <= endY; ++y)
		{
			if (y < 0 || y >= maxCol) continue;

			//선택 영역 저장
			selectedPosition.emplace_back(Vector2(x, y));
		}
	}
}

void MouseBase::QuitGame()
{
	// 엔진 종료 요청
	Game::Get().QuitEngine();
}
