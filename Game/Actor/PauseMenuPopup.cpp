#include "PauseMenuPopup.h"
#include "Game/Game.h"
#include "Core/Input.h"
#include "Render/Renderer.h"
#include "Manager/MapManager.h"

PauseMenuPopup::PauseMenuPopup()
	: super(" ", Vector2::Zero)
{
	// 생성되자마자 일시정지 상태 해제
	isActive = false;

	// 최상단에 그려지도록 정렬 순서 설정
	sortingOrder = 10;
	Initialize();
}

PauseMenuPopup::~PauseMenuPopup()
{
}

void PauseMenuPopup::Tick(float deltaTime)
{
	if (!isActive)
	{
		return;
	}

	Vector2 mousePos = Input::Get().MousePosition();
	currentIndex = -1;

	// 마우스 위치와 메뉴 항목 충돌 체크
	for (int i = 0; i < (int)menuItems.size(); ++i)
	{
		const auto& item = menuItems[i];

		// 텍스트 영역 안에 마우스가 있는지 확인
		if (mousePos.x >= item.position.x && mousePos.x < item.position.x + item.size.x &&
			mousePos.y == item.position.y)
		{
			currentIndex = i;

			// 왼쪽 클릭 시 동작 실행
			if (Input::Get().GetMouseButtonDown(0))
			{
				// 게임 재개
				if (item.id == 0)
				{
					SetActive(false);
				}
				// 메인 메뉴로 돌아가기
				else if (item.id == 1)
				{
					isActive = false;
					Game::Get().ToggleMenu(0);
				}
			}
		}

	}
}

void PauseMenuPopup::Draw()
{
	if (!isActive)
	{
		return;
	}

	// 배경 박스 그리기
	for (int y = 0; y < boxSize.y; y++)
	{
		for (int x = 0; x < boxSize.x; x++)
		{
			// 박스의 둘레만 흰 바탕으로 칠하기
			if (x == 0 || x == boxSize.x - 1 || y == 0 || y == boxSize.y - 1)
			{
				Renderer::Get().Submit(
					" ", Vector2(boxPos.x + x, boxPos.y + y), Color::BackGoundWhite, sortingOrder - 1);
			}
		}
	}

	// 테두리 및 타이틀
	Renderer::Get().Submit(
		"--- PAUSE MENU ---", Vector2(boxPos.x + boxSize.x / 2 - 9, boxPos.y + 2), Color::White, sortingOrder);

	static char buffer[4][128];

	// 메뉴 항목 그리기
	for (int i = 0; i < (int)menuItems.size(); ++i)
	{
		// 혹시 모르는 예외처리
		if (i >= 4)
		{
			break;
		}

		const auto& item = menuItems[i];

		Vector2 drawPos = Vector2(boxPos.x + boxSize.x / 2 - 6, boxPos.y + 6 + (i * 4));

		// 하이라이트 색상 결정 (마우스가 올라가 있으면 초록색 배경)
		Color textColor = (currentIndex == i) ? Color::Green : Color::White;

		// 문자열 조립
		sprintf_s(buffer[i], 128, "%s %s", (currentIndex == i) ? ">" : " ", menuItems[i].text.c_str());

		Renderer::Get().Submit(buffer[i], drawPos, textColor, sortingOrder);

		// 마우스 클릭을 위해 menuItems 내부의 데이터(충돌 영역)도 실시간으로 동기화해줍니다.
		menuItems[i].position = drawPos;
		menuItems[i].size = Vector2(15, 1); // 클릭 영역 넉넉히
	}
}

void PauseMenuPopup::SetActive(bool active)
{
	isActive = active;

	// 활성화 될 때 선택 인덱스 초기화
	currentIndex = -1;
}

void PauseMenuPopup::Initialize()
{
	int width = Game::Get().Width();
	int height = Game::Get().Height();

	boxSize = Vector2(48, 16);
	boxPos = Vector2(width / 2 - boxSize.x / 2, height / 2 - boxSize.y / 2);

	// 메뉴 항목 설정
	menuItems.push_back({
		"RESUME GAME", Vector2(boxPos.x + boxSize.x / 2 - 6, boxPos.y + 6), Vector2(15,1), 0
		});

	menuItems.push_back({
		"EXIT TO MENU", Vector2(boxPos.x + boxSize.x / 2 - 6, boxPos.y + 10), Vector2(15,1), 1
		});
}






/*
void EscapeLevel::DrawPauseMenu()
{
	// 맵의 중앙 좌표 계산
	int centerX = Game::Get().Width() / 2 - 9;
	int centerY = Game::Get().Height() / 2 - 2;

	// 배경 박스 (간단한 형태)
	Renderer::Get().Submit("==================", Vector2(centerX, centerY));
	Renderer::Get().Submit("||  PAUSE MENU  ||", Vector2(centerX, centerY + 1));
	Renderer::Get().Submit("==================", Vector2(centerX, centerY + 2));

	// 0번 메뉴: 계속하기
	Color color0 = (pauseMenuIndex == 0) ? Color::Green : Color::White;
	Renderer::Get().Submit(
		(pauseMenuIndex == 0) ? "> Resume Game" : "  Resume Game",
		Vector2(centerX + 2, centerY + 4), color0);

	// 1번 메뉴: 메인 메뉴로
	Color color1 = (pauseMenuIndex == 1) ? Color::Green : Color::White;
	Renderer::Get().Submit(
		(pauseMenuIndex == 1) ? "> Exit to Menu" : "  Exit to Menu",
		Vector2(centerX + 2, centerY + 5), color1);
}

*/