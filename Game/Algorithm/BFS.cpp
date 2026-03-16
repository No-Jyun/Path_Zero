#include "BFS.h"
#include "Manager/MapManager.h"
#include "Util/Direction.h"
#include "Util/Util.h"

#include <queue>

BFS::BFS()
{
	LoadMapData();
}

std::vector<Vector2> BFS::FindExitableTile()
{
	// BFS 용 큐 선언
	std::queue<Vector2> queue;

	// 반환용 배열
	std::vector<Vector2> canExitPosition;

	// 맵의 생존자 위치 가져오기
	auto& survivorPos = MapManager::Get().GetSurvivorPositions();

	for (const Vector2& pos : survivorPos)
	{
		// 플레이어 위치는 방문 표시
		mapVisited[pos.y][pos.x] = true;

		// 큐에 넣기
		queue.emplace(pos);
	}

	while (!queue.empty())
	{
		Vector2 nowPos = queue.front();
		queue.pop();

		for (int dir = 0; dir < Direction::direction8Length; dir++)
		{
			Vector2 nextPos = nowPos + Direction::eightDirection[dir];

			// 생존자는 항상 맵의 내부에 존재하고, 우리는 맵 경계에 있는
			// 탈출구의 경로를 찾으므로 맵을 벗어나는 예외처리 생략함

			// 다음 위치가 방문한 위치라면 생략
			if (mapVisited[nextPos.y][nextPos.x])
			{
				continue;
			}

			// 다음 위치가 탈출구를 만들 수 있는 위치라면
			if (MapManager::Get().IsExitablePosition(nextPos))
			{
				// 가능 후보에 전달
				canExitPosition.emplace_back(nextPos);
				mapVisited[nextPos.y][nextPos.x] = true;

				continue;
			}

			// 대각선 로직 검증 필요
			if (dir % 2 == 1 && !IsMovableDiagnal(nowPos, Direction::eightDirection[dir]))
			{
				continue;
			}

			// 다음 위치가 빈 공간인 경우만 이동 가능
			// 맵 편집 후 탈출경로가 존재하는지에 대한 정적 BFS 이므로 빈칸만 확인
			if (MapManager::Get().GetMapPositionData(nextPos) == ' ')
			{
				mapVisited[nextPos.y][nextPos.x] = true;
				queue.emplace(nextPos);
			}
		}
	}

	return canExitPosition;
}

std::vector<Vector2> BFS::FindSpreadableTile(std::vector<Vector2>& activeFirePositions)
{
	// BFS 용 큐 선언
	// 결국 1번만 BFS를 도므로 큐가 필요 없음
	//std::queue<Vector2> queue;

	// 이번에 새롭게 번질 타일 위치 배열
	std::vector<Vector2> newFires;

	// 다음 턴에도 확산 가능성이 있는 타일 위치 배열
	std::vector<Vector2> nextActiveFires;

	// 현재 불타일에서 1번만 bfs 하면 됨
	for (const Vector2& firePos : activeFirePositions)
	{
		bool canSpread = false;

		// 8방향 안이쁨
		for (int dir = 0; dir < Direction::directionFireLength; dir++)
		{
			Vector2 nextPos = firePos + Direction::fireDirection[dir];

			// 불타일은 맵경계를 벗어날 수 있으므로 예외 처리
			if (IsOutMap(nextPos))
			{
				continue;
			}

			if (dir == 2 || dir == 5)
			{
				if (IsBlocked(firePos, Direction::fireDirection[dir]))
				{
					continue;
				}
			}

			// Todo: 확인 필요
			char mapTile = MapManager::Get().GetMapPositionData(nextPos);
			if (mapTile == ' ' || mapTile == 'S')
			{
				canSpread = true;

				// 다음 위치가 방문한 위치라면 생략
				if (mapVisited[nextPos.y][nextPos.x])
				{
					continue;
				}

				newFires.emplace_back(nextPos);
				mapVisited[nextPos.y][nextPos.x] = true;
			}
		}

		// 최소 한 군데라도 번질 여지가 있었다면 다음에도 활성 타일로
		if (canSpread)
		{
			nextActiveFires.emplace_back(firePos);
		}
	}

	for (const Vector2& pos : newFires)
	{
		nextActiveFires.emplace_back(pos);
	}

	// 활성 불 타일 갱신
	activeFirePositions = nextActiveFires;

	return newFires;
}

Vector2 BFS::FindSafestTileFromFire(const Vector2& curPosition)
{
	Vector2 bestMovePos = curPosition;

	std::vector<Vector2> fires = MapManager::Get().GetFirePositions();

	int width = MapManager::Get().GetMapWidth();
	int height = MapManager::Get().GetMapHeight();

	// 거리 맵 초기화
	// 가장 가까운 불의 최초 위치를 전파하여, Sqrt 없는 정확한 제곱 거리를 계산
	std::vector<std::vector<int>> fireDistMap(height, std::vector<int>(width, INT_MAX));
	std::vector<std::vector<Vector2>> fireOriginMap(height, std::vector<Vector2>(width, Vector2(-1, -1))); // 불의 출처 추적	
	std::queue<Vector2> fireQueue;

	for (const Vector2& fire : fires)
	{
		fireDistMap[fire.y][fire.x] = 0;
		fireOriginMap[fire.y][fire.x] = fire;
		fireQueue.emplace(fire);
	}

	while (!fireQueue.empty())
	{
		Vector2 nowPos = fireQueue.front();
		fireQueue.pop();

		// 현재 위치에 도달한 가장 가까운 불의 좌표
		Vector2 origin = fireOriginMap[nowPos.y][nowPos.x];

		for (int i = 0; i < Direction::direction8Length; ++i)
		{
			Vector2 nextPos = nowPos + Direction::eightDirection[i];

			// 맵 밖으로 나가면 무시
			if (IsOutMap(nextPos))
			{
				continue;
			}

			// 최초 불 위치와 nextPos 사이의 제곱 거리 계산
			int dx = nextPos.x - origin.x;
			int dy = nextPos.y - origin.y;
			int nextDistSq = (dx * dx) + (dy * dy);

			// 기존에 기록된 제곱 거리보다 더 가깝게 도달할 수 있다면 갱신
			if (fireDistMap[nextPos.y][nextPos.x] > nextDistSq)
			{
				fireDistMap[nextPos.y][nextPos.x] = nextDistSq;
				fireOriginMap[nextPos.y][nextPos.x] = origin; // 기원지 정보도 같이 전파
				fireQueue.emplace(nextPos);
			}
		}
	}

	// 생존자 도달 가능 영역 일반 BFS
	std::queue<Vector2> survivorQueue;
	survivorQueue.emplace(curPosition);
	mapVisited[curPosition.y][curPosition.x] = true;

	// 최고 안전 거리 비교를 위한 초기값
	int bestMinDistToFire = -1;

	while (!survivorQueue.empty())
	{
		Vector2 nowPos = survivorQueue.front();
		survivorQueue.pop();

		// 미리 계산된 다중 출발점의 제곱 거리 참조
		int distToFireSq = fireDistMap[nowPos.y][nowPos.x];

		// 가장 안전한(거리가 먼) 타일 갱신
		if (distToFireSq > bestMinDistToFire)
		{
			bestMinDistToFire = distToFireSq;
			bestMovePos = nowPos;
		}

		for (int i = 0; i < Direction::direction8Length; ++i)
		{
			Vector2 nextPos = nowPos + Direction::eightDirection[i];

			// 맵 밖, 이미 방문한 곳, 빈 공간(' ')이 아닌 곳은 모두 스킵
			if (IsOutMap(nextPos)) 
			{
				continue;
			}

			if (mapVisited[nextPos.y][nextPos.x])
			{
				continue;
			}

			if (MapManager::Get().GetMapPositionData(nextPos) != ' ')
			{
				continue;
			}

			// 대각선 검증
			if (i % 2 == 1 && !IsMovableDiagnal(nowPos, Direction::eightDirection[i]))
			{
				continue;
			}

			mapVisited[nextPos.y][nextPos.x] = true;
			survivorQueue.emplace(nextPos);
		}
	}

	return bestMovePos;
}

bool BFS::IsMovableDiagnal(const Vector2& curPos, const Vector2& direction)
{
	// x 방향 검증
	Vector2 chkPos = curPos;
	chkPos.x += direction.x;

	// 해당 타일이 빈 칸이 아니면 이동 불가
	if (MapManager::Get().GetMapPositionData(chkPos) != ' ')
	{
		return false;
	}

	// y 방향 검증
	chkPos = curPos;
	chkPos.y += direction.y;

	// 해당 타일이 빈 칸이 아니면 이동 불가
	if (MapManager::Get().GetMapPositionData(chkPos) != ' ')
	{
		return false;
	}

	return true;
}

bool BFS::IsOutMap(const Vector2& position)
{
	return position.x < 0 || position.x >= MapManager::Get().GetMapWidth() ||
		position.y < 0 || position.y >= MapManager::Get().GetMapHeight();
}

bool BFS::IsBlocked(const Vector2& curPos, const Vector2& direction)
{
	Vector2 chkPos = curPos;
	if (direction.x > 0)
	{
		chkPos.x += 1;
	}
	else
	{
		chkPos.x -= 1;
	}

	if (MapManager::Get().GetMapPositionData(chkPos) == '#')
	{
		return true;
	}

	return false;
}

void BFS::LoadMapData()
{
	// 맵 정보 배열 Clear
	std::vector<std::string>().swap(mapData);

	// 맵 매니저로부터 맵 정보 로드
	mapData = MapManager::Get().GetMapData();

	int mapWidth = MapManager::Get().GetMapWidth();
	int mapHeight = MapManager::Get().GetMapHeight();

	// 방문 정보 배열 Clear
	std::vector<std::vector<bool>>().swap(mapVisited);

	// 방문 정보 초기화
	mapVisited.assign(mapHeight, std::vector<bool>(mapWidth, false));
}
