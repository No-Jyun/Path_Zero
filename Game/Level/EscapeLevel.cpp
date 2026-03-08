#include "EscapeLevel.h"
#include "Manager/MapManager.h"
#include "Manager/LogManager.h"
#include "Actor/Survivor.h"
#include "Actor/LogActor.h"
#include "Actor/EscapeMouse.h"
#include "Render/Renderer.h"
#include "Game/Game.h"
#include "Util/Util.h"

static const Color survivorColors[6] =
{
	Color::Red, Color::Yellow, Color::Green, Color::Sky, Color::Blue, Color::Purple
};

static const char* instructionString[] =
{
	"로그",
	" ",
};

EscapeLevel::EscapeLevel()
{	
	// Todo: 속도 조정
	fireSpreadTimer.SetTargetTime(200.0f);
	survivorMoveTimer.SetTargetTime(0.2f);

	// 마우스 액터 생성
	AddNewActor(new EscapeMouse(Vector2(0, Game::Get().Height() - 1), &survivorVector));

	// 로그 위치 설정
	int offsetX = MapManager::Get().GetMapWidth() + 5;
	int offsetY = _countof(instructionString);

	// 로그 액터 생성 및 저장
	for (int i = 0; i < LogManager::Get().GetLogCount(); i++)
	{
		LogActor* logActor = new LogActor(Vector2(offsetX, offsetY + i));
		AddNewActor(logActor);
		logVector.emplace_back(logActor);
	}
}

EscapeLevel::~EscapeLevel()
{
	std::vector<LogActor*>().swap(logVector);
}

void EscapeLevel::Tick(float deltaTime)
{
	super::Tick(deltaTime);

	// 불 확산 타이머
	fireSpreadTimer.Tick(deltaTime);
	survivorMoveTimer.Tick(deltaTime);

	// 불 확산 시간 만료시
	if (fireSpreadTimer.IsTimeOut())
	{
		// 타이머 리셋
		fireSpreadTimer.Reset();
		
		// 색상 변경 표시
		colorSwap = !colorSwap;

		// 불 타일 확산
		MapManager::Get().SpreadFire();
	}

	// 생존자 이동 시간 만료시
	if (survivorMoveTimer.IsTimeOut())
	{
		// 타이머 리셋
		survivorMoveTimer.Reset();

		// 생존자 이동 순서는 랜덤으로 결정
		Util::ShuffleVector(survivorVector);

		// 생존자의 사망 처리를 위해 이터레이터를 통해 순회
		for (auto iterator = survivorVector.begin(); iterator != survivorVector.end();)
		{
			Survivor* curSurv = *iterator;

			// 생존자 이동
			curSurv->MoveToExitOrTarget();

			// 생존자 이동 후 파괴 요청 (탈출구 도달 등) 을 받았다면
			if (curSurv->DestroyRequested())
			{
				// survivorVector에서 생존자 제거 및 다음 요소 가리키게 이동
				iterator = survivorVector.erase(iterator);
			}
			else
			{
				// 파괴되지 않았다면 다음 생존자
				iterator++;
			}
		}
	}
}

void EscapeLevel::Draw()
{
	super::Draw();

	DrawMap();
	DrawInstruction();
}

void EscapeLevel::LevelSetting()
{
	// 현재 생성되었던 생존자 메모리 정리
	for (Actor* actor : survivorVector)
	{
		actor->Destroy();
	}

	// 배열 초기화
	std::vector<Survivor*>().swap(survivorVector);

	// 레벨 세팅
	Initialize();
}

void EscapeLevel::Initialize()
{
	MapManager::Get().StartGame();

	// 생존자 액터 생성
	auto survivorPos = MapManager::Get().GetSurvivorPositions();
	int survivorIndex = 0;
	for (; survivorIndex < static_cast<int>(survivorPos.size()); survivorIndex++)
	{
		Survivor* newSurvivor = new Survivor(survivorPos[survivorIndex], survivorColors[survivorIndex]);
		AddNewActor(newSurvivor);
		survivorVector.emplace_back(newSurvivor);
	}

	// 로그 매니저 사용 준비
	LogManager::Get().Initialize(logVector);
}

void EscapeLevel::DrawMap()
{
	const std::vector<std::string>& map = MapManager::Get().GetMapData();

	int rows = MapManager::Get().GetMapHeight();
	int cols = MapManager::Get().GetMapWidth();

	for (int i = 0; i < rows; i++)
	{
		for (int j = 0;j < cols;j++)
		{
			Vector2 drawPosition = Vector2(j, i);
			
			Color color = Color::White;
			if (MapManager::Get().GetMapPositionData(drawPosition) == 'F')
			{
				color = colorSwap ? Color::Red : Color::LightRed;
			}

			Renderer::Get().Submit(&map[i][j], drawPosition, color);
		}
	}
}

void EscapeLevel::DrawInstruction()
{
	size_t length = sizeof(instructionString) / sizeof(instructionString[0]);

	int offsetX = MapManager::Get().GetMapWidth() + 5;

	for (int i = 0; i < length; i++)
	{
		Vector2 drawPosition = Vector2(offsetX, 1);
		drawPosition.y += i;

		Renderer::Get().Submit(instructionString[i], drawPosition, Color::LightRed);
	}
}
