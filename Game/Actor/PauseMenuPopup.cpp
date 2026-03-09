#include "PauseMenuPopup.h"
#include "Game/Game.h"
#include "Core/Input.h"
#include "Render/Renderer.h"
#include "Manager/MapManager.h"

// 메뉴 항목 문자열 배열
static const char* resumeGameText[] =
{
	" =============== ",
	"|  Resume Game  |",
	" =============== "
};

static const char* backToMainText[] =
{
	" ================ ",
	"|  Back To Main  |",
	" ================ "
};

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
	// 메뉴 아이템 제거
	for (PauseMenuItem*& item : menuItems)
	{
		delete item;
		item = nullptr;
	}

	// 배열 초기화
	menuItems.clear();

}

void PauseMenuPopup::Tick(float deltaTime)
{
	if (!isActive)
	{
		return;
	}

	// 현재 마우스 위치
	Vector2 mousePos = Input::Get().MousePosition();

	// 마우스가 영역 위에 없다면 색상 X
	currentIndex = -1;

	for (int i = 0; i < menuItems.size(); ++i)
	{
		PauseMenuItem* item = menuItems[i];

		// 마우스 X, Y 좌표가 메뉴 항목의 사각형 영역(topLeft ~ bottomRight) 안에 있는지 검사
		if (mousePos.x >= item->topLeft.x && mousePos.x <= item->bottomRight.x &&
			mousePos.y >= item->topLeft.y && mousePos.y <= item->bottomRight.y)
		{
			// 마우스가 영역 안에 들어왔으므로 현재 선택된 인덱스 갱신
			currentIndex = i;

			// 마우스 왼쪽 버튼(0)을 클릭했다면
			if (Input::Get().GetMouseButtonDown(0))
			{
				int selIndex = currentIndex;
				currentIndex = -1;

				// 해당 메뉴 항목의 함수 실행
				menuItems[selIndex]->onSelected();
			}

			// 하나의 메뉴만 선택될 수 있으므로 찾았다면 반복문 종료
			break;
		}
	}
}

void PauseMenuPopup::Draw()
{
	if (!isActive)
	{
		return;
	}

	DrawLogo();
	DrawItem();
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

	boxSize = Vector2(60, 24);
	boxPos = Vector2((width - boxSize.x) / 2, (height - boxSize.y) / 2);

	int centerY = boxPos.y + (boxSize.y / 2);
	int leftZoneCenter = boxPos.x + (boxSize.x / 4);
	int rightZoneCenter = boxPos.x + (boxSize.x * 3 / 4);

	// 메뉴 아이템 생성
	menuItems.emplace_back(new PauseMenuItem(
		resumeGameText,
		sizeof(resumeGameText) / sizeof(resumeGameText[0]),
		Vector2(leftZoneCenter - (static_cast<int>(strlen(resumeGameText[0])) / 2), centerY + 4),
		[this]()
		{
			// 일시정지 메뉴 중지
			isActive = false;
		}
	));

	menuItems.emplace_back(new PauseMenuItem(
		backToMainText,
		sizeof(backToMainText) / sizeof(backToMainText[0]),
		Vector2(rightZoneCenter - (static_cast<int>(strlen(backToMainText[0])) / 2), centerY + 4),
		[this]()
		{
			// 메인 메뉴로 레벨 토글
			isActive = false;
			Game::Get().ToggleMenu(0);
		}
	));
}

void PauseMenuPopup::DrawLogo()
{
	// 배경 박스 그리기
	for (int y = 0; y < boxSize.y; y++)
	{
		for (int x = 0; x < boxSize.x; x++)
		{
			// 박스의 둘레만 흰 바탕으로 칠하기
			if (x == 0 || x == boxSize.x - 1 || y == 0 || y == boxSize.y - 1)
			{
				Renderer::Get().Submit(
					" ", Vector2(boxPos.x + x, boxPos.y + y), 
					Color::BackGoundWhite, sortingOrder - 1);
			}
		}
	}

	int logoX = boxPos.x + (boxSize.x / 2) - 10;
	int logoY = boxPos.y + (boxPos.y * 2 / 5);

	// 배경 박스 (간단한 형태)
	Renderer::Get().Submit("==================", 
		Vector2(logoX, logoY), Color::White, sortingOrder);

	Renderer::Get().Submit("||  PAUSE MENU  ||", 
		Vector2(logoX, logoY + 1), Color::White, sortingOrder);

	Renderer::Get().Submit("==================", 
		Vector2(logoX, logoY + 2), Color::White, sortingOrder);
}

void PauseMenuPopup::DrawItem()
{
	// 메뉴 항목 그리기
	for (int i = 0; i < static_cast<int>(menuItems.size()); ++i)
	{
		Vector2 drawPos = menuItems[i]->topLeft;

		// 하이라이트 색상 결정 (마우스가 올라가 있으면 초록색 배경)
		Color textColor = 
			(currentIndex == i) ? Color::Green : Color::White;

		// 아이템의 모든 줄을 순회하며 그리기
		for (int line = 0; line < menuItems[i]->lineCount; line++)
		{
			Vector2 lineDrawpos = drawPos;
			lineDrawpos.y += line;

			Renderer::Get().Submit(menuItems[i]->text[line], lineDrawpos, textColor, sortingOrder);
		}

		int textLength = static_cast<int>(strlen(menuItems[i]->text[0]));

		// 우하단 위치 저장
		menuItems[i]->bottomRight.x = menuItems[i]->topLeft.x + textLength - 1;

		// y 끝점은 현재 y 위치에서 lineCount - 1 만큼 더한 값
		menuItems[i]->bottomRight.y = menuItems[i]->topLeft.y + menuItems[i]->lineCount - 1;
	}
}
