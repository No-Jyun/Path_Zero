#include "EditLevel.h"
#include "Manager/MapManager.h"
#include "Manager/LogManager.h"
#include "Engine/Engine.h"
#include "Render/Renderer.h"
#include "Actor/EditMouse.h"
#include "Actor/LogActor.h"

const char* instructionString[] =
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
	" ",
	"주의점",
	"불과 탈출구를 생성하지 않을 시 무작위로 생성됩니다.",
	"탈출구는 반드시 맵 경계에 생성해야 합니다.",
	"현재 상황에서 탈출 경로가 없는 경우 맵을 다시 제작해야 합니다.",
	" ",
	"현재 상황에서 탈출 경로가 없는 경우 맵을 다시 제작해야 합니다.",
	" ",
	" ",
	"로그",
	" ",
};

EditLevel::EditLevel()
{
	MapManager::Get().SetNewGame();

	AddNewActor(new EditMouse(Vector2(0, Engine::Get().Height() - 1)));

	// 로그 위치 설정
	int offsetX = MapManager::Get().GetMapWidth() + 5;
	int offsetY = _countof(instructionString);
	
	// Todo: 레벨 전환 생각해야함
	// 로그 매니저에게 전달할 배열 생성
	std::vector<LogActor*> logVector;
	for (int i = 0; i < LogManager::Get().GetLogCount(); i++)
	{
		LogActor* logActor = new LogActor(Vector2(offsetX, offsetY + i));
		AddNewActor(logActor);
		logVector.emplace_back(logActor);
	}

	// 로그 매니저 사용 준비
	LogManager::Get().Initialize(logVector);
}

EditLevel::~EditLevel()
{
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

	for (int i = 0; i < length; i++)
	{
		Vector2 drawPosition = Vector2(offsetX, 1);
		drawPosition.y += i;

		// 로그 안내문 색상 변경
		if (i == length - 2)
		{
			Renderer::Get().Submit(instructionString[i], drawPosition, Color::LightRed);
		}
		else
		{
			Renderer::Get().Submit(instructionString[i], drawPosition);
		}
	}
}
