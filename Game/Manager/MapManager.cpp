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
	mapData[position.y][position.x] = tile;
}

void MapManager::SetNewGame()
{
	Initialize();
}

void MapManager::StartGame()
{
	// 탈출구 저장
	FindExits();	
}

void MapManager::Initialize()
{
	std::vector<std::string>().swap(mapData);
	std::vector<Vector2>().swap(exitPositions);
	std::vector<Vector2>().swap(survivorPositions);

	mapData.assign(mapHeight, std::string(mapWidth, ' '));

	// 랜덤 생존자 6명 생성
	int count = 1;
	while (count <= 6)
	{
		count++;

		// 맵 중앙 랜덤 좌표 생성
		int randX = Util::Random(mapWidth / 2 - 8, mapWidth / 2 + 8);
		int randY = Util::Random(mapHeight / 2 - 3, mapHeight / 2 + 3);

		// 이미 생성된 플레이어 좌표 검사
		for (const Vector2& pos : survivorPositions)
		{
			// 좌표가 같으면 다시 랜덤좌표 스킵
			if (pos.x == randX && pos.y == randY)
			{
				count--;
				continue;
			}
		}

		mapData[randY][randX] = 'P';
		survivorPositions.emplace_back(Vector2(randX, randY));

	}

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