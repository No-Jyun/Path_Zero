#include "BFS.h"
#include "Manager/MapManager.h"
#include "Util/Direction.h"

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

std::vector<Vector2> BFS::FindSpreadableTile()
{
    // BFS 용 큐 선언
    std::queue<Vector2> queue;

    // 반환용 배열
    std::vector<Vector2> canSpreadPosition;

    // 맵의 불 위치 가져오기
    auto& firePos = MapManager::Get().GetFirePositions();

    for (const Vector2& pos : firePos)
    {
        queue.emplace(pos);
        mapVisited[pos.y][pos.x] = true;
    }

    // 현재 불타일에서 1번만 bfs 하면 됨
    while (!queue.empty())
    {
        Vector2 nowPos = queue.front();
        queue.pop();

        // 8방향 안이쁨
        for (int dir = 0; dir < Direction::direction4Length; dir++)
        {
            Vector2 nextPos = nowPos + Direction::fourDirection[dir];

            // 불타일은 맵경계를 벗어날 수 있으므로 예외 처리
            if (nextPos.x < 0 || nextPos.x >= MapManager::Get().GetMapWidth() ||
                nextPos.y < 0 || nextPos.y >= MapManager::Get().GetMapHeight())
            {
                continue;
            }

            // 다음 위치가 방문한 위치라면 생략
            if (mapVisited[nextPos.y][nextPos.x])
            {
                continue;
            }

            // 불타일은 모든 타일을 덮을 수 있으므로 바로 배열에 저장
            canSpreadPosition.emplace_back(nextPos);
            mapVisited[nextPos.y][nextPos.x] = true;
        }
    }

    return canSpreadPosition;
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
