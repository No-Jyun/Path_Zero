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
            // Todo: 임시 코드 / 일단 벽만 못가게 막기
            // Todo: 충돌 및 대각선 판단 필요
            if (MapManager::Get().GetMapPositionData(newPosition) == '#')
            {
                continue;
            }

            // 현재 노드를 기준으로 gCost 계산
            float newGCost = currentNode->gCost + Direction::directionCost[directionIndex];

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

std::vector<Node*> AStar::ConstructPath(Node* goalNode)
{
    // 경로를 저장할 배열 선언
    std::vector<Node*> path;

    // 역추적 하면서 path에 채우기
    // goalNode는 생존자의 위치이므로 제외
    Node* currentNode = goalNode->parentNode;
    while (currentNode)
    {
        path.emplace_back(currentNode);
        currentNode = currentNode->parentNode;
    }

    // 탈출구에서부터 찾은 결과이므로 이미 정렬이 되어 있음
    //std::reverse(path.begin(), path.end());
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
