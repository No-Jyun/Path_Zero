#include "Survivor.h"
#include "Util/Util.h"
#include "Algorithm/AStar.h"
#include "Algorithm/Node.h"
#include "Algorithm/BFS.h"
#include "Render/Renderer.h"
#include "Manager/MapManager.h"
#include "Manager/LogManager.h"

#include <string>

Survivor::Survivor(const char* text, const Vector2& position, Color color)
	: super(text, position, color)
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

bool Survivor::FindPathToTarget(const Vector2& targetPosition)
{
	// 목적지까지 A* 알고리즘 적용 및 경로 저장
	path = astar->FindPathToTarget(new Node(position), targetPosition);

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
		bool isFindPathToTarget = FindPathToTarget(commandTargetPosition);

		// 빈 경로라면 - 도달 했다면
		if (!isFindPathToTarget)
		{
			// 멈춰서 대기
			hasCommandMove = false;
			isReachTarget = true;
			return;
		}

		Vector2 movedPosition = path[0]->GetPosition();

		// 막힌 타일이 다른 생존자인 경우 특별 처리
		if (MapManager::Get().GetMapPositionData(movedPosition) == 'S')
		{
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
		// 상태 변수 초기화
		isNoExitAndRunAway = false;

		// 다음으로 이동할 노드에서 위치 가져오기
		Vector2 movedPosition = path[0]->GetPosition();

		// 다음 위치 확인
		// 이동하는 곳이 탈출구인지 확인
		if (MapManager::Get().GetMapPositionData(movedPosition) == 'X')
		{
			// 로그 표시
			std::string logMsg = std::string(image) + "번 생존자가 탈출했습니다!!";
			LogManager::Get().PrintLog(logMsg.c_str(), GetPathColor());

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

	// 경로가 없다면 BFS 알고리즘을 통해 불 타일과 가장 먼 타일 탐색
	BFS bfs;
	Vector2 safestPosition = bfs.FindSafestTileFromFire(position);

	// 현재 위치가 불과 가장 먼 위치라면 대기
	if (safestPosition == position)
	{
		return;
	}

	// 이전에 도망치고 있지 않았는데, 이번에 처음 도망치게 되는 경우
	if (!isNoExitAndRunAway)
	{
		// 상태 저장
		isNoExitAndRunAway = true;

		// 로그 출력
		char buffer[128];
		sprintf_s(buffer, 128, "[%s] 탈출 경로가 차단되어 가장 안전한 곳으로 도망칩니다!", image);
		LogManager::Get().PrintLog(buffer, GetPathColor());
	}

	// 해당 타일을 향한 A* 알고리즘으로 경로 업데이트
	isFindPath = FindPathToTarget(safestPosition);

	if (!isFindPath)
	{
		return;
	}

	Vector2 movedPosition = path[0]->GetPosition();

	if (MapManager::Get().GetMapPositionData(movedPosition) == 'S')
	{
		// 누군가가 길을 막으면 리턴
		return;
	}

	// 경로에서 삭제
	path.erase(path.begin());

	// 이동 처리
	MapManager::Get().SetMapTile(position, ' ');
	position = movedPosition;
	MapManager::Get().SetMapTile(position, 'S');
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
