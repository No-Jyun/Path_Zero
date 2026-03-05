#include "EscapeLevel.h"
#include "Manager/MapManager.h"
#include "Actor/Survivor.h"
#include "Render/Renderer.h"

static const Color survivorColors[6] =
{
	Color::Red, Color::Yellow, Color::Green, Color::Sky, Color::Blue, Color::Purple
};

EscapeLevel::EscapeLevel()
{	
}

EscapeLevel::~EscapeLevel()
{
}

void EscapeLevel::Draw()
{
	super::Draw();

	DrawMap();
}

void EscapeLevel::LevelSetting()
{
	// 현재 생성되었던 액터 메모리 정리
	for (Actor*& actor : actors)
	{
		if (actor)
		{
			delete actor;
			actor = nullptr;
		}
	}

	// 배열 초기화
	actors.clear();

	// 레벨 세팅
	Initialize();
}

void EscapeLevel::Initialize()
{
	MapManager::Get().StartGame();

	// 액터 생성
	auto survivorPos = MapManager::Get().GetSurvivorPositions();
	int survivorIndex = 0;
	for (; survivorIndex < static_cast<int>(survivorPos.size()); survivorIndex++)
	{
		AddNewActor(new Survivor(survivorPos[survivorIndex], survivorColors[survivorIndex]));
	}
}

void EscapeLevel::DrawMap()
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
