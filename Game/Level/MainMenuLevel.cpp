#include "MainMenuLevel.h"
#include "Game/Game.h"
#include "Core/Input.h"
#include "Render/Renderer.h"

// 메뉴 로고 문자열
static const char* menuLogo[] =
{
	" .S_sSSs     .S_SSSs    sdSS_SSSSSSbs   .S    S.    sdSSSSSSSbs    sSSs   .S_sSSs      sSSs_sSSs    ",
	".SS~YS%%b   .SS~SSSSS   YSSS~S%SSSSSP  .SS    SS.   YSSSSSSSS%S   d%%SP  .SS~YS%%b    d%%SP~YS%%b   ",
	"S%S   `S%b  S%S   SSSS       S%S       S%S    S%S          S%S   d%S'    S%S   `S%b  d%S'     `S%b  ",
	"S%S    S%S  S%S    S%S       S%S       S%S    S%S         S&S    S%S     S%S    S%S  S%S       S%S  ",
	"S%S    d*S  S%S SSSS%S       S&S       S%S SSSS%S        S&S     S&S     S%S    d*S  S&S       S&S  ",
	"S&S   .S*S  S&S  SSS%S       S&S       S&S  SSS&S        S&S     S&S_Ss  S&S   .S*S  S&S       S&S  ",
	"S&S_sdSSS   S&S    S&S       S&S       S&S    S&S       S&S      S&S~SP  S&S_sdSSS   S&S       S&S  ",
	"S&S~YSSY    S&S    S&S       S&S       S&S    S&S      S*S       S&S     S&S~YSY%b   S&S       S&S  ",
	"S*S         S*S    S&S       S*S       S*S    S*S     S*S        S*b     S*S   `S%b  S*b       d*S  ",
	"S*S         S*S    S*S       S*S       S*S    S*S   .s*S         S*S.    S*S    S%S  S*S.     .S*S  ",
	"S*S         S*S    S*S       S*S       S*S    S*S   sY*SSSSSSSP   SSSbs  S*S    S&S   SSSbs_sdSSS   ",
	"S*S         SSS    S*S       S*S       SSS    S*S  sY*SSSSSSSSP    YSSP  S*S    SSS    YSSP~YSSY    ",
	"SP                 SP        SP               SP                         SP                         ",
	"Y                  Y         Y                Y                          Y                          ",
};

// 메뉴 항목 문자열 배열
static const char* startGameText[] =
{
	" --------------- ",
	"|  START GAME   |",
	" --------------- "
};

static const char* quitGameText[] =
{
	" --------------- ",
	"|   QUIT GAME   |",
	" --------------- "
};

MainMenuLevel::MainMenuLevel()
{
	// 메뉴 아이템 생성
	items.emplace_back(new MenuItem(
		startGameText,
		sizeof(startGameText) / sizeof(startGameText[0]),
		[]()
		{
			// 메뉴 토글 함수 호출
			Game::Get().ToggleMenu(1);
		}
	));

	items.emplace_back(new MenuItem(
		quitGameText,
		sizeof(quitGameText) / sizeof(quitGameText[0]),
		[]()
		{
			// 프로그램 종료
			Game::Get().QuitEngine();
		}
	));

	// 배열 크기 초기화
	size_t arrayLength = sizeof(menuLogo) / sizeof(menuLogo[0]);
	logoArrayLength = static_cast<int>(arrayLength);

	SetVectorPosition();
}

MainMenuLevel::~MainMenuLevel()
{
	// 메뉴 아이템 제거
	for (MenuItem*& item : items)
	{
		delete item;
		item = nullptr;
	}

	// 배열 초기화
	items.clear();
}

void MainMenuLevel::Tick(float deltaTime)
{
	super::Tick(deltaTime);

	// 마우스로 입력 처리
	Vector2 mousePosition = Input::Get().MousePosition();

	// 메뉴 아이템 항목을 벗어나면 Color 표시 X
	currentIndex = -1;

	for (int i = 0; i < items.size(); ++i)
	{
		MenuItem* item = items[i];

		// 마우스 X, Y 좌표가 메뉴 항목의 사각형 영역(topLeft ~ bottomRight) 안에 있는지 검사
		if (mousePosition.x >= item->topLeft.x && mousePosition.x <= item->bottomRight.x &&
			mousePosition.y >= item->topLeft.y && mousePosition.y <= item->bottomRight.y)
		{
			// 마우스가 영역 안에 들어왔으므로 현재 선택된 인덱스 갱신
			currentIndex = i;

			// 마우스 왼쪽 버튼(0)을 클릭했다면
			if (Input::Get().GetMouseButtonDown(0))
			{
				int selIndex = currentIndex;
				currentIndex = -1;

				// 해당 메뉴 항목의 함수 실행
				items[selIndex]->onSelected();
			}

			// 하나의 메뉴만 선택될 수 있으므로 찾았다면 반복문 종료
			break;
		}
	}
}

void MainMenuLevel::Draw()
{
	super::Draw();

	// 로고 출력
	DrawLogo();

	// 메뉴 아이템 출력
	DrawItem();
}

void MainMenuLevel::LevelSetting()
{
}

void MainMenuLevel::SetVectorPosition()
{
	// 기본 값으로 초기화
	consoleCenter = Vector2::Zero;
	logoPosition = Vector2::Zero;

	// 콘솔의 정중앙 설정
	consoleCenter.x = Game::Get().Width() / 2;
	consoleCenter.y = Game::Get().Height() / 2;

	// 로고 문자열 하나의 길이
	int logoStringLength = static_cast<int>(strlen(menuLogo[0]));

	// 그릴 문자열이 콘솔의 중앙에 오도록 정렬
	logoPosition.x = consoleCenter.x - (logoStringLength / 2);

	// 로고가 크므로 중앙의 4칸 위로 설정
	logoPosition.y = consoleCenter.y - 6;

	// 로고의 중앙 위치 계산
	int logoCenter = logoPosition.x + (logoStringLength / 2);

	// 메뉴 아이템 문자열 하나의 길이
	int itemLength = static_cast<int>(strlen(items[0]->text[0]));

	// 메뉴 아이템 문자열이 로고와 같이 중앙에 오도록 정렬
	itemPosition.x = logoCenter - (itemLength / 2);

	// 로고의 2칸 아래에 그리도록 설정
	itemPosition.y = logoPosition.y + logoArrayLength + 2;
}

void MainMenuLevel::DrawLogo()
{
	// 배열을 순회하며 Draw
	for (int i = 0; i < logoArrayLength; i++)
	{
		// 로고를 그리기로 정한 위치에서
		Vector2 drawPosition = logoPosition;

		// y값을 증가시켜 아래로 한칸씩 그리기
		drawPosition.y += i;

		// 로고 색상을 3분의 1로 나눠서 표현
		Color logoColor = Color::White;

		// 하단 3분의 1은 빨강
		if (i >= logoArrayLength * 2 / 3)
		{
			logoColor = Color::Red;
		}
		// 중단 3분의 1은 연빨강
		else if (i >= logoArrayLength / 3)
		{
			logoColor = Color::LightRed;
		}

		Renderer::Get().Submit(menuLogo[i], drawPosition, logoColor);
	}
}

void MainMenuLevel::DrawItem()
{
	int curYOffset = 0;

	// 메뉴 항목 배열 순회
	for (int i = 0; i < static_cast<int>(items.size()); i++)
	{
		// 메뉴 아이템을 그리기로 정한 위치에서
		Vector2 drawPosition = itemPosition;

		// y값을 증가시켜 아래로 한칸씩 그리기
		drawPosition.y += curYOffset;

		// 아이템 색상 확인 (선택됐는지 여부)
		Color textColor =
			(i == currentIndex) ? selectedColor : unselectedColor;

		// 아이템의 모든 줄을 순회하며 그리기
		for (int line = 0;line < items[i]->lineCount;line++)
		{
			Vector2 lineDrawpos = drawPosition;
			lineDrawpos.y += line;

			Renderer::Get().Submit(items[i]->text[line], lineDrawpos, textColor);
		}

		// 좌상단 위치 저장
		items[i]->topLeft = drawPosition;

		int textLength = static_cast<int>(strlen(items[i]->text[0]));

		// 우하단 위치 저장
		items[i]->bottomRight.x = drawPosition.x + textLength - 1;

		// y 끝점은 현재 y 위치에서 lineCount - 1 만큼 더한 값
		items[i]->bottomRight.y = drawPosition.y + items[i]->lineCount - 1;

		// 다음 메뉴 아이템 y 간격 누적
		curYOffset += items[i]->lineCount + 2;
	}
}
