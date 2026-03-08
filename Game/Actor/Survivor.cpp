#include "Survivor.h"
#include "Util/Util.h"
#include "Algorithm/AStar.h"
#include "Algorithm/Node.h"
#include "Algorithm/BFS.h"
#include "Render/Renderer.h"
#include "Manager/MapManager.h"

Survivor::Survivor(const Vector2& position, Color color)
	: super("S", position, color)
{
	astar = new AStar();
	sortingOrder = 5;

	timer.SetTargetTime(1.0f);
}

Survivor::~Survivor()
{
	SafeDelete(astar);

	// path에 저장된 노드는 AStar가 정리될 때 모두 처리됨
	std::vector<Node*>().swap(path);
}

void Survivor::BeginPlay()
{
	super::BeginPlay();

	// 액터 첫 시작시 경로 검색 및 저장
	path = astar->FindPath(new Node(position));
}

void Survivor::Tick(float deltaTime)
{
	super::Tick(deltaTime);

	// 플레이어가 생존자에게 명령했다면
	//if (hasCommandMove)
	//{
	//	FindPathToTarget();
	//	//path = astar->FindPathToTarget(new Node(position), commandTargetPosition);
	//	return;
	//}

	// 명령내린 목적지 도착 시
	if (isReachTarget)
	{
		// 생존자 대기
		timer.Tick(deltaTime);

		if (timer.IsTimeOut())
		{
			timer.Reset();

			isReachTarget = false;
		}
		return;
	}

	//bool isFindPath = FindPath();
	//
	//// Todo: 경로 없으면 로그 출력 및 애니메이션
	//if (!isFindPath)
	//{
	//	return;
	//}

}

void Survivor::Draw()
{
	super::Draw();

	if (path.empty())
		return;

	Vector2 nodePos = Vector2::Zero;
	for (Node* const node : path)
	{
		nodePos = node->GetPosition();
		Renderer::Get().Submit(".", nodePos, GetPathColor(), sortingOrder - 1);
	}

	// 마지막 노드의 위치 (탈출구의 위치) 는 탈출구에 색 입히기
	Renderer::Get().Submit("X", nodePos, GetPathColor(), sortingOrder - 1);
}

bool Survivor::FindPath()
{
	// 탈출구까지의 A* 알고리즘 적용 및 경로 저장
	path = astar->FindPath(new Node(position));

	// 경로를 못찾은 경우
	if (path.empty())
	{
		return false;
	}

	// 경로를 찾은 경우
	return true;
}

bool Survivor::FindPathToTarget()
{
	// 목적지까지 A* 알고리즘 적용 및 경로 저장
	path = astar->FindPathToTarget(new Node(position), commandTargetPosition);

	// 경로를 못 찾은 경우
	if (path.empty())
	{
		return false;
	}

	// 경로를 찾은 경우
	return true;
}

void Survivor::MoveToExitOrTarget()
{
	// 목적지에 도착 후 타이머가 지나지 않았다면 대기
	if (isReachTarget)
	{
		return;
	}

	if (hasCommandMove)
	{
		bool isFindPathToTarget = FindPathToTarget();

		// 빈 경로라면 - 도달 했다면
		if (!isFindPathToTarget)
		{
			// 멈춰서 대기
			hasCommandMove = false;
			isReachTarget = true;
			return;
		}

		Vector2 movedPosition = path[0]->GetPosition();

		char targetTile = MapManager::Get().GetMapPositionData(movedPosition);

		if (targetTile == '#' || targetTile == 'F')
		{
			// 벽이나 불이면 멈추기
			return;
		}

		if (targetTile == 'S')
		{
			// 막힌 타일이 다른 생존자인 경우 특별 처리
			// 목적지 근처인데 다른 생존자가 길막 중이라면, 무리 지어 도달했다고 판단
			if (path.size() <= 3)
			{
				hasCommandMove = false;
				isReachTarget = true;
			}

			// 도착 판정이든 아니든 움직일 수는 없음
			return;
		}

		// 경로에서 삭제
		path.erase(path.begin());

		// 맵에서도 이동
		// 이전 좌표는 빈 공간으로
		MapManager::Get().SetMapTile(position, ' ');

		// 생존자 이동
		position = movedPosition;

		// 이동한 좌표를 플레이어 표시
		MapManager::Get().SetMapTile(position, 'S');
		return;
	}

	// 생존자들의 이동 반영하여 다시 경로 찾기
	bool isFindPath = FindPath();

	// 경로 존재시 이동
	if (isFindPath)
	{
		// 다음으로 이동할 노드에서 위치 가져오기
		Vector2 movedPosition = path[0]->GetPosition();

		// 다음 위치 확인
		// 이동하는 곳이 탈출구인지 확인
		if (MapManager::Get().GetMapPositionData(movedPosition) == 'X')
		{
			// Todo: 로그 표시
			// 플레이어 제거
			Destroy();

			// 이전 좌표는 빈 공간으로
			MapManager::Get().SetMapTile(position, ' ');
			return;
		}

		// 이동하는 곳이 다른 생존자인지 확인
		else if (MapManager::Get().GetMapPositionData(movedPosition) == 'S')
		{
			// 이동하지 못하고 대기
			return;
		}

		// 경로에서 삭제
		path.erase(path.begin());

		// 맵에서도 이동
		// 이전 좌표는 빈 공간으로
		MapManager::Get().SetMapTile(position, ' ');

		// 생존자 이동
		position = movedPosition;

		// 이동한 좌표를 플레이어 표시
		MapManager::Get().SetMapTile(position, 'S');
		return;
	}

	// Todo: 경로 없으면 로그 출력 및 애니메이션
	// 경로가 없다면 8방향 중에서 탈출구와 가장 가까운 빈 공간으로 1칸 이동
	Vector2 nearestExit = Vector2(-1, -1);
	float minExitDist = FLT_MAX;

	// 가장 가까운 탈출구 탐색
	for (const Vector2& exitPos : MapManager::Get().GetExitPositions())
	{
		// 탈출구와 생존자의 거리 계산
		float dist = Util::Sqrt(
			static_cast<float>(
				(exitPos.x - position.x) * (exitPos.x - position.x) +
				(exitPos.y - position.y) * (exitPos.y - position.y))
		);

		// 최소 거리 갱신
		if (dist < minExitDist)
		{
			minExitDist = dist;
			nearestExit = exitPos;
		}
	}

	// 만약 맵에 탈출구가 아예 없다면 가만히 있는다.
	if (nearestExit.x == -1)
	{
		// Todo: 로그 및 애니메이션 표시
		return;
	}

	// 8방향 탐색을 위한 BFS 객체
	BFS bfs;
	Vector2 bestMovePosition = bfs.FindOneStepToGoal(position, nearestExit);

	// 현재 위치보다 더 가까워지는 타일이 있다면 그곳으로 1칸 이동
	if (bestMovePosition != position)
	{
		MapManager::Get().SetMapTile(position, ' ');
		position = bestMovePosition;
		MapManager::Get().SetMapTile(position, 'S');
	}
}

void Survivor::CommandMoveTo(const Vector2& targetPosition)
{
	hasCommandMove = true;
	isReachTarget = false;
	commandTargetPosition = targetPosition;

	// 이전에 가던 길 (출구로 향하던) 삭제
	path.clear();

	// 명령내린 목적지로 경로 업데이트
	path = astar->FindPathToTarget(new Node(position), commandTargetPosition);
}

Color Survivor::GetPathColor()
{
	return static_cast<Color>(static_cast<unsigned short>(color) | FOREGROUND_INTENSITY);
}
