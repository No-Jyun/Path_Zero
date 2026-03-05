#include "MapManager.h"
#include "Engine/Engine.h"
#include "Util/Util.h"

// 정적 변수 초기화
MapManager* MapManager::instance = nullptr;

MapManager::MapManager()
{
	instance = this;

	mapWidth = (Engine::Get().Width() / 5) * 3;
	mapHeight = Engine::Get().Height() - 2;
}

MapManager::~MapManager()
{
	// 빈 벡터와 mapData를 swap하여 메모리를 완전히 해제
	std::vector<std::string>().swap(mapData);
}

void MapManager::SetMapTile(const Vector2& position, const char tile)
{
	// 마우스로 선택된 영역은 map 영역을 벗어나지 않으므로 생략
	mapData[position.x][position.y] = tile;
}

void MapManager::SetNewGame()
{
	Initialize();
}

void MapManager::StartGame()
{
	FindExits();
}

void MapManager::Initialize()
{
	std::vector<std::string>().swap(mapData);
	std::vector<Vector2>().swap(exitPositions);
	std::vector<Vector2>().swap(playerPositions);

	mapData.assign(mapHeight, std::string(mapWidth, ' '));

	// Todo: 랜덤 플레이어 생성
	int randX = Util::Random(mapWidth / 2 - 10, mapWidth / 2 + 10);
	int randY = Util::Random(mapHeight / 2 - 10, mapHeight / 2 + 10);
	mapData[randY][randX] = 'P';
	playerPositions.emplace_back(Vector2(randX, randY));

	// 맵 경계 설정
	for (int j = 0; j < mapHeight; j++)
	{
		mapData[j][0] = '#';
		mapData[j][mapWidth - 1] = '#';
	}

	for (int i = 0; i < mapWidth; i++)
	{
		mapData[0][i] = '#';
		mapData[mapHeight - 1][i] = '#';
	}
}

void MapManager::FindExits()
{
	for (int i = 0; i < mapHeight; i++)
	{
		for (int j = 0; j < mapWidth; j++)
		{
			if (mapData[i][j] == 'X')
			{
				exitPositions.emplace_back(Vector2(j, i));
			}
		}
	}
}

MapManager& MapManager::Get()
{
	// 예외 처리
	if (!instance)
	{
		// 오류 메시지 출력.
		MessageBoxA(
			nullptr,
			"MapManager::Get() - instance is null",
			"Error",
			MB_OK
		);

		__debugbreak();
	}

	// 정적 변수 반환
	return *instance;
}