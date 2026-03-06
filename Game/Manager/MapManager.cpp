#include "MapManager.h"
#include "Engine/Engine.h"
#include "Util/Util.h"
#include "Util/Direction.h"

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
	// 마우스로 선택된 영역은 map 영역을 벗어나지 않으므로 예외처리 생략
	mapData[position.y][position.x] = tile;
}

void MapManager::SetNewGame()
{
	Initialize();
}

void MapManager::StartGame()
{
	// 불 / 탈출구 개수를 확인하고 부족하면 생성
	CheckFireAndExit();

	// 탈출구 저장
	FindImportantTiles();	
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

		mapData[randY][randX] = 'S';
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

void MapManager::CheckFireAndExit()
{
	// 탈출구 개수가 최대 개수보다 적다면
	if (GetExitCount() < maxExitCount)
	{
		int leftCount = maxExitCount - GetExitCount();
		int count = 0;

		while (count < leftCount)
		{
			// 맵에 생성할 좌표 선언
			int randX = 0;
			int randY = 0;

			// 상하좌우 중에서 생성할 방향를 선택하고 랜덤 좌표 생성
			int dir = Util::Random(0, 3);			
			switch (dir)
			{
				// 상
			case 0:
				randX = Util::Random(0, mapWidth - 1);
				randY = 0;
				break;

				// 우
			case 1:
				randX = mapWidth - 1;
				randY = Util::Random(0, mapHeight - 1);
				break;

				// 하
			case 2:
				randX = Util::Random(0, mapWidth - 1);
				randY = mapHeight - 1;
				break;

				// 좌
			default:
				randX = 0;
				randY = Util::Random(0, mapHeight - 1);
				break;
			}

			// 좌표 타일 정보
			const char curTile = mapData[randY][randX];

			// 현재 타일이 탈출구 / 불 / 생존자라면 다시 생성
			if (curTile == 'X' || curTile == 'F' || curTile == 'S')
			{
				continue;
			}

			// 탈출구의 4방향이 막혔는지 확인
			bool isAllBlocked = true;

			int dirLength = Direction::direction4Length;
			for (int d = 0; d < dirLength; d++)
			{
				int nextX = randX + Direction::fourDirection[d].x;
				int nextY = randY + Direction::fourDirection[d].y;

				if (nextX < 0 || nextX >= mapWidth || nextY < 0 || nextY >= mapHeight)
				{
					continue;
				}

				// 4방향 중 한 곳이라도 빈 공간 존재시 생성 가능
				if (mapData[nextY][nextY] == ' ')
				{
					isAllBlocked = false;
				}
			}

			if (!isAllBlocked)
			{
				// 랜덤 좌표 공간을 탈출구 타일로 생성
				// 배열에 저장은 이후에 처리됨
				mapData[randY][randX] = 'X';
				count++;
			}
		}
	}

	// 불 개수가 최대 개수보다 적다면
	if (GetFireCount() < maxFireCount)
	{
		int leftCount = maxFireCount - GetFireCount();
		int count = 0;

		while (count < leftCount)
		{
			count++;

			// 맵 좌표 랜덤 생성
			int randX = Util::Random(0, mapWidth - 1);
			int randY = Util::Random(0, mapHeight - 1);

			// 좌표 타일 정보
			const char curTile = mapData[randY][randX];

			// 현재 타일이 탈출구 / 불 / 생존자라면 다시 생성
			if (curTile == 'X' || curTile == 'F' || curTile == 'S')
			{
				count--;
				continue;
			}

			// 랜덤 좌표 공간을 불 타일로 생성
			// 배열에 저장은 이후에 처리됨
			mapData[randY][randX] = 'F';
		}
	}
}

void MapManager::FindImportantTiles()
{
	std::vector<Vector2>().swap(exitPositions);
	std::vector<Vector2>().swap(firePositions);

	for (int i = 0; i < mapHeight; i++)
	{
		for (int j = 0; j < mapWidth; j++)
		{
			if (mapData[i][j] == 'X')
			{
				exitPositions.emplace_back(Vector2(j, i));
			}
			else if (mapData[i][j] == 'F')
			{
				firePositions.emplace_back(Vector2(j, i));
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