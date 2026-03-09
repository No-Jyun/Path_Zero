#include "EditLevel.h"
#include "Manager/MapManager.h"
#include "Manager/LogManager.h"
#include "Game/Game.h"
#include "Render/Renderer.h"
#include "Actor/EditMouse.h"
#include "Actor/LogActor.h"
#include "Core/Input.h"

#include <string>

static const char* instructionString[] =
{
	"조작법",
	"클릭/드래그를 통해 변경할 곳을 선택합니다.",
	"특정 키를 입력하면 선택한 영역을 해당 타일로 변경합니다.",
	" ",
	"W키 : 생존자가 넘어갈 수 없는 벽을 생성합니다.",
	"F키 : 점점 넓어지는 불을 생성합니다 (최대 5개)",
	"E키 : 생존자가 탈출할 수 있는 탈출구를 생성합니다. (최대 3개)",
	"D키 : 선택한 영역을 빈 공간으로 변경합니다.",
	"R키 : 선택 영역이 초기화 됩니다.",
	"Q키 : 프로그램을 종료합니다.",
	"Enter키 : 맵 편집을 종료하고 시뮬레이션을 시작합니다.",
	"ESC키 : 메인메뉴로 돌아갑니다.",
	" ",
	"주의점",
	"불과 탈출구를 생성하지 않을 시 무작위로 생성됩니다.",
	"탈출구는 반드시 맵 경계에 생성해야 합니다.",
	"현재 상황에서 탈출 경로가 없는 경우 맵을 다시 제작해야 합니다.",
	" ",
	"현재 상황에서 탈출 경로가 없는 경우 맵을 다시 제작해야 합니다.",
	" ",
	"생존자의 이동속도를 조정하고 싶으면 클릭하세요.",
	"1 - 10 (기본 6) 높을수록 빨라집니다. / 현재 : ",
	"불의 확산속도를 조정하고 싶으면 클릭하세요.",
	"1 - 10 (기본 4) 높을수록 빨라집니다. / 현재 : ",
	" ",
	"로그",
	" ",
};

EditLevel::EditLevel()
{
	MapManager::Get().SetNewGame();

	AddNewActor(new EditMouse(Vector2(0, Game::Get().Height() - 1)));

	// 로그 위치 설정
	int offsetX = MapManager::Get().GetMapWidth() + 5;
	int offsetY = _countof(instructionString);
	
	// 로그 매니저에게 전달할 배열 생성
	for (int i = 0; i < LogManager::Get().GetLogCount(); i++)
	{
		LogActor* logActor = new LogActor(Vector2(offsetX, offsetY + i));
		AddNewActor(logActor);
		logVector.emplace_back(logActor);
	}
}

EditLevel::~EditLevel()
{
	std::vector<LogActor*>().swap(logVector);
}

void EditLevel::Tick(float deltaTime)
{
	super::Tick(deltaTime);

	// 마우스 클릭 검사
	if (Input::Get().GetMouseButtonDown(0))
	{
		Vector2 mousePos = Input::Get().MousePosition();

		// 마우스가 생존자 이동 속도 문구 위라면 생존자 이동 속도 증가
		if (mousePos.x >= survivorLogPosition.x && mousePos.x <= survivorLogPosition.x + 
			static_cast<int>(strlen(instructionString[survivorLogIndex])) - 1 &&
			mousePos.y == survivorLogPosition.y)
		{
			survivorMovementSpeed = survivorMovementSpeed % 10 + 1;
		}

		// 마우스가 불 확산 속도 문구 위라면 불 확산 속도 증가
		else if (mousePos.x >= fireLogPosition.x && mousePos.x <= fireLogPosition.x +
			static_cast<int>(strlen(instructionString[fireLogIndex])) - 1 &&
			mousePos.y == fireLogPosition.y)
		{
			fireSpredSpeed = fireSpredSpeed % 10 + 1;
		}

		Game::Get().SetSpeedIndexs(survivorMovementSpeed - 1, fireSpredSpeed - 1);
	}
}

void EditLevel::Draw()
{
	super::Draw();

	DrawMap();
	DrawInstruction();
}

void EditLevel::LevelSetting()
{
	MapManager::Get().SetNewGame();

	// 로그 매니저 사용 준비
	LogManager::Get().Initialize(logVector);

	survivorMovementSpeed = 6;
	fireSpredSpeed = 4;
	Game::Get().SetSpeedIndexs(survivorMovementSpeed - 1, fireSpredSpeed - 1);
}

void EditLevel::DrawMap()
{
	const std::vector<std::string>& map = MapManager::Get().GetMapData();

	int rows = MapManager::Get().GetMapHeight();
	int cols = MapManager::Get().GetMapWidth();

	for (int i = 0; i < rows; i++)
	{
		Vector2 drawPosition = Vector2::Zero;
		drawPosition.y += i;

		Renderer::Get().Submit(map[i].c_str(), drawPosition);
	}
}

void EditLevel::DrawInstruction()
{
	size_t length = sizeof(instructionString) / sizeof(instructionString[0]);

	int offsetX = MapManager::Get().GetMapWidth() + 5;

	static std::string logText[2];
	for (int i = 0; i < length; i++)
	{
		Vector2 drawPosition = Vector2(offsetX, 1);
		drawPosition.y += i;

		// 로그 안내문 색상 변경
		if (i == length - 2)
		{
			Renderer::Get().Submit(instructionString[i], drawPosition, Color::LightRed);
		}
		else if (i == survivorLogIndex)
		{
			survivorLogPosition = drawPosition;
			Renderer::Get().Submit(instructionString[i], drawPosition, Color::LightGreen);
		}
		else if (i == survivorLogIndex + 1)
		{
			logText[0] = std::string(instructionString[i]);
			logText[0] += std::to_string(survivorMovementSpeed);
			Renderer::Get().Submit(logText[0].c_str(), drawPosition);
		}
		else if (i == fireLogIndex)
		{
			fireLogPosition = drawPosition;
			Renderer::Get().Submit(instructionString[i], drawPosition, Color::LightRed);
		}
		else if (i == fireLogIndex + 1)
		{
			logText[1] = std::string(instructionString[i]);
			logText[1] += std::to_string(fireSpredSpeed);
			Renderer::Get().Submit(logText[1].c_str(), drawPosition);
		}
		else
		{
			Renderer::Get().Submit(instructionString[i], drawPosition);
		}
	}
}
