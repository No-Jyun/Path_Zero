#include "AStar.h"
#include "Util/Util.h"
#include "Util/Direction.h"
#include "Manager/MapManager.h"

#include <algorithm>

AStar::AStar()
{
}

AStar::~AStar()
{
	// 메모리 정리
	ClearSetting();
}

std::vector<Node*> AStar::FindPath(Node* startNode)
{
	ClearSetting();

	// 시작 노드 (현재 위치) 를 목적지로 설정
	this->goalNode = startNode;

	// 방문 비용 배열 초기화
	visitedGCost.assign(MapManager::Get().GetMapHeight(), std::vector<float>(MapManager::Get().GetMapWidth(), FLT_MAX));

	// 탈출구를 열린 리스트에 추가 및 탐색 시작
	const auto& exitPos = MapManager::Get().GetExitPositions();
	for (const Vector2& exit : exitPos)
	{
		openList.push(new Node(exit, nullptr));

		// 방문 배열 설정
		visitedGCost[exit.y][exit.x] = 0.0f;
	}

	int directionLength = Direction::direction8Length;

	// 목적지에 도달 못했고, 탐색 가능한 위치가 있으면 계속 진행
	while (!openList.empty())
	{
		// 현재 열린 리스트에 있는 노드 중 fCost가 가장 낮은 노드 선택
		// 우선순위 큐에 있으므로 Top에 있는 노드 선택하면 됨
		Node* currentNode = openList.top();
		openList.pop();

		// 현재 노드가 목표 노드인지 확인
		if (IsDestination(currentNode))
		{
			// 경로 반환 후 종료
			return ConstructPath(currentNode);
		}

		// 현재 노드를 방문 노드에 추가
		closedList.emplace_back(currentNode);

		// 이웃 노드 방문 (탐색)
		for (int directionIndex = 0; directionIndex < directionLength; directionIndex++)
		{
			// 다음에 이동할 위치 (이웃 노드의 위치)
			Vector2 newPosition = currentNode->position + Direction::eightDirection[directionIndex];

			// 유효성 검증 (새 위치가 이동 가능한지 확인)
			if (!IsInRange(newPosition))
			{
				continue;
			}

			// 새 위치가 이동 가능한 곳인지 확인
			// 벽 / 불 타일로 못 가게 막기
			if (!IsMovableTile(newPosition))
			{
				continue;
			}

			// 대각선 판단
			// 현재 방향이 대각선인 경우 양 옆 타일 검사 필요
			if (directionIndex % 2 == 1 &&
				!IsMovableDiagnal(currentNode->position, Direction::eightDirection[directionIndex]))
			{
				continue;
			}

			// 현재 노드를 기준으로 gCost 계산
			float newGCost = currentNode->gCost + Direction::directionCost[directionIndex];

			// 생존자 간의 충돌 판단
			// 이동하는 칸이 생존자인 경우
			// --> 다른 생존자가 있어도 경로 탐색 시에는 일단 지나갈 수 있는 길로 취급
			// 다만 일반 비용보다 훨씬 큰 페널티 부여
			if (!IsOtherSurvivorTile(newPosition))
			{
				newGCost += 5.0f;
			}

			// 갈 수는 있지만, 이미 방문한 곳인지 확인
			if (HasVisited(newPosition, newGCost))
			{
				continue;
			}

			// 방문을 위한 이웃 노드 생성
			Node* neighborNode = new Node(newPosition, currentNode);
			// 비용 계산
			neighborNode->gCost = newGCost;
			neighborNode->hCost = CalculateHeuristic(neighborNode, this->goalNode);
			neighborNode->fCost = neighborNode->gCost + neighborNode->hCost;

			// 방문 노드 저장값 갱신
			visitedGCost[newPosition.y][newPosition.x] = newGCost;

			// 열린 리스트에 추가
			openList.push(neighborNode);
		}
	}

	return std::vector<Node*>();
}

std::vector<Node*> AStar::FindPathToTarget(Node* startNode, const Vector2& targetPosition)
{
	ClearSetting();

	// 목적지로 설정 -> 정방향 탐색
	this->goalNode = new Node(targetPosition, nullptr);

	// 방문 비용 배열 초기화
	visitedGCost.assign(MapManager::Get().GetMapHeight(), std::vector<float>(MapManager::Get().GetMapWidth(), FLT_MAX));

	// 목표지점 하나만 열린 리스트에 추가 및 탐색 시작
	openList.push(startNode);
	visitedGCost[startNode->position.y][startNode->position.x] = 0.0f;

	// 가장 목표와 가까웠던 노드를 기억할 변수
	Node* closestNode = startNode;
	float minHCost = CalculateHeuristic(startNode, this->goalNode);

	int directionLength = Direction::direction8Length;

	// 목적지에 도달 못했고, 탐색 가능한 위치가 있으면 계속 진행
	while (!openList.empty())
	{
		// 현재 열린 리스트에 있는 노드 중 fCost가 가장 낮은 노드 선택
		// 우선순위 큐에 있으므로 Top에 있는 노드 선택하면 됨
		Node* currentNode = openList.top();
		openList.pop();

		float currentHCost = CalculateHeuristic(currentNode, this->goalNode);
		if (currentHCost < minHCost)
		{
			minHCost = currentHCost;
			closestNode = currentNode;
		}

		// 현재 노드가 목표 노드인지 확인
		if (IsDestination(currentNode))
		{
			// 경로 반환 후 종료
			return ConstructPath(currentNode, false);
		}

		// 현재 노드를 방문 노드에 추가
		closedList.emplace_back(currentNode);

		// 이웃 노드 방문 (탐색)
		for (int directionIndex = 0; directionIndex < directionLength; directionIndex++)
		{
			// 다음에 이동할 위치 (이웃 노드의 위치)
			Vector2 newPosition = currentNode->position + Direction::eightDirection[directionIndex];

			// 유효성 검증 (새 위치가 이동 가능한지 확인)
			if (!IsInRange(newPosition))
			{
				continue;
			}

			// 새 위치가 이동 가능한 곳인지 확인
			// 벽 / 불 타일로 못 가게 막기
			if (!IsMovableTile(newPosition))
			{
				continue;
			}

			// 대각선 판단
			// 현재 방향이 대각선인 경우 양 옆 타일 검사 필요
			if (directionIndex % 2 == 1 &&
				!IsMovableDiagnal(currentNode->position, Direction::eightDirection[directionIndex]))
			{
				continue;
			}

			// 현재 노드를 기준으로 gCost 계산
			float newGCost = currentNode->gCost + Direction::directionCost[directionIndex];

			// 생존자 간의 충돌 판단
			// 이동하는 칸이 생존자인 경우
			// --> 다른 생존자가 있어도 경로 탐색 시에는 일단 지나갈 수 있는 길로 취급
			// 다만 일반 비용보다 훨씬 큰 페널티 부여
			if (!IsOtherSurvivorTile(newPosition))
			{
				newGCost += 5.0f;
			}

			// 갈 수는 있지만, 이미 방문한 곳인지 확인
			if (HasVisited(newPosition, newGCost))
			{
				continue;
			}

			// 방문을 위한 이웃 노드 생성
			Node* neighborNode = new Node(newPosition, currentNode);
			// 비용 계산
			neighborNode->gCost = newGCost;
			neighborNode->hCost = CalculateHeuristic(neighborNode, this->goalNode);
			neighborNode->fCost = neighborNode->gCost + neighborNode->hCost;

			// 방문 노드 저장값 갱신
			visitedGCost[newPosition.y][newPosition.x] = newGCost;

			// 열린 리스트에 추가
			openList.push(neighborNode);
		}
	}
	
	// 경로를 찾지 못한 경우
	// 시작점이 가장 가까운 노드라면 빈 경로 반환
	if (closestNode == startNode)
	{
		return std::vector<Node*>();
	}

	// 목적지와 가장 가까운 노드로 경로 반환
	return ConstructPath(closestNode, false);
}

std::vector<Node*> AStar::ConstructPath(Node* goalNode, bool isReverse)
{
	// 경로를 저장할 배열 선언
	std::vector<Node*> path;

	// 역추적 하면서 path에 채우기
	Node* currentNode = goalNode;
	
	// 역방향 탐색시 goalNode는 생존자의 위치이므로 제외
	if (isReverse)
	{
		currentNode = goalNode->parentNode;
	}

	while (currentNode)
	{
		path.emplace_back(currentNode);
		currentNode = currentNode->parentNode;
	}

	// 정방향 탐색인 경우 reverse 
	if (!isReverse)
	{
		if (!path.empty())
		{
			path.pop_back();
		}

		std::reverse(path.begin(), path.end());
	}

	// 역방향 탐색은 탈출구에서부터 찾은 결과이므로 이미 정렬이 되어 있음	
	return path;
}

float AStar::CalculateHeuristic(Node* currentNode, Node* goalNode)
{
	// 현재 노드에서 목표 노드까지의 비용 계산
	// 유클리드 거리 (Euclidean Distance)
	Vector2 diff = currentNode->position - goalNode->position;
	return Util::Sqrt(static_cast<float>(diff.x * diff.x + diff.y * diff.y));
}

bool AStar::IsInRange(const Vector2& curPos)
{
	// x, y 범위가 벗어났는지 확인
	if (curPos.x < 0 || curPos.x >= MapManager::Get().GetMapWidth() ||
		curPos.y < 0 || curPos.y >= MapManager::Get().GetMapHeight())
	{
		return false;
	}

	// 벗어나지 않으면 true 반환
	return true;
}

bool AStar::IsMovableDiagnal(const Vector2& curPos, const Vector2& direction)
{
	// 이 방향의 대각선으로 움직일 수 있다는 것은 맵 경계 예외처리가 필요없다는 뜻

	// x 방향 검증
	Vector2 chkPos = curPos;
	chkPos.x += direction.x;

	// 해당 타일이 벽 / 불 / 생존자 라면 false 반환
	if (!IsMovableTile(chkPos) || !IsOtherSurvivorTile(chkPos))
	{
		return false;
	}

	// y 방향 검증
	chkPos = curPos;
	chkPos.y += direction.y;

	// 해당 타일이 벽 / 불 / 생존자 라면 false 반환
	if (!IsMovableTile(chkPos) || !IsOtherSurvivorTile(chkPos))
	{
		return false;
	}

	return true;
}

bool AStar::IsMovableTile(const Vector2& position)
{
	// 해당 타일이 벽 / 불 이라면 false 반환
	if (MapManager::Get().GetMapPositionData(position) == '#' ||
		MapManager::Get().GetMapPositionData(position) == 'F')
	{
		return false;
	}

	return true;
}

bool AStar::IsOtherSurvivorTile(const Vector2& position)
{
	// 해당 타일이 생존자라면
	if (MapManager::Get().GetMapPositionData(position) == 'S')
	{
		// 경로 탐색을 시도하는 생존자가 아닌 경우 스킵
		if (position != goalNode->position)
		{
			// 함수명을 봐선 true 반환이지만 일관성을 위해 false 반환
			return false;
		}
	}

	return true;
}

bool AStar::HasVisited(const Vector2& curPos, float gCost)
{
	// gCost를 비교하는 이유
	// 해당 위치의 휴리스틱 값 (hCost) 는 동일하므로 
	// 실제 이동 비용인 gCost만 비교하면 됨
	// 방문 배열에 저장된 값보다 크거나 같으면 방문 처리
	if (visitedGCost[curPos.y][curPos.x] <= gCost)
	{
		return true;
	}

	// 그외는 방문해야 하므로 true 반환
	return false;
}

bool AStar::IsDestination(const Node* const node)
{
	// 두 노드가 같은지 비교
	return *node == *goalNode;
}

void AStar::ClearSetting()
{
	// 메모리 정리
	while (!openList.empty())
	{
		Node* node = openList.top();
		openList.pop();
		SafeDelete(node);
	}

	for (Node* node : closedList)
	{
		SafeDelete(node);
	}
	closedList.clear();

	SafeDelete(goalNode);

	std::vector<std::vector<float>>().swap(visitedGCost);
}
