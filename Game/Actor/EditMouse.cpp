#include "EditMouse.h"
#include "Core/Input.h"
#include "Manager/MapManager.h"
#include "Manager/LogManager.h"
#include "Game/Game.h"
#include "Algorithm/AStar.h"
#include "Algorithm/BFS.h"
#include "Util/Util.h"

EditMouse::EditMouse(const Vector2& position)
	: super(position)
{
}

EditMouse::~EditMouse()
{
}

void EditMouse::Tick(float deltaTime)
{
	super::Tick(deltaTime);

	// 키보드 입력 처리
	/*
	"조작법",
	"클릭/드래그를 통해 변경할 곳을 선택합니다.",
	"특정 키를 입력하면 선택한 영역을 해당 타일로 변경합니다.",
	" ",
	"W키 : 생존자가 넘어갈 수 없는 벽을 생성합니다.",
	"F키 : 점점 넓어지는 불을 생성합니다 (최대 5개)",
	"E키 : 생존자가 탈출할 수 있는 탈출구를 생성합니다. (최대 3개)",
	"D키 : 선택한 영역을 빈 공간으로 변경합니다.",
	"R키 : 선택 영역이 초기화 됩니다.",
	"Q키 : 프로그램을 종료합니다.",
	"Enter키 : 맵 편집을 종료하고 시뮬레이션을 시작합니다.",
	" ",
	"주의점",
	"불과 탈출구를 생성하지 않을 시 무작위로 생성됩니다.",
	"탈출구는 반드시 맵 경계에 생성해야 합니다.",
	"현재 상황에서 탈출 경로가 없는 경우 맵을 다시 제작해야 합니다.",
	*/
	// 게임 종료
	if (Input::Get().GetKeyDown('Q'))
	{
		QuitGame();
		return;
	}

	// 엔터 입력시 맵 토글
	if (Input::Get().GetKeyDown(VK_RETURN))
	{
		// 맵 토글 전에 맵 검사
		if (!IsExitable())
		{
			return;
		}

		Game::Get().ToggleMenu(1);
	}

	// 선택 영역 벽 생성
	if (Input::Get().GetKeyDown('W'))
	{
		MakeTile('#');
	}
	// 선택 영역 불 생성
	if (Input::Get().GetKeyDown('F'))
	{
		MakeTile('F');
	}
	// 선택 영역 탈출구 생성
	if (Input::Get().GetKeyDown('E'))
	{
		MakeTile('X');
	}
	// 선택 영역 빈 공간으로 초기화
	if (Input::Get().GetKeyDown('D'))
	{
		MakeTile(' ');
	}
	// 선택 영역 초기화
	if (Input::Get().GetKeyDown('R'))
	{
		SelectPositionClear();
	}
}

void EditMouse::MakeTile(const char tile)
{
	// 이번에 실제로 바꾼 타일 수
	int switchedCount = 0;

	// 현재 존재하는 불/탈출구 개수
	int existedSize = 0;

	// 최대 불/탈출구 개수
	int maxSize = INT_MAX;

	if (tile == 'F')
	{
		existedSize = MapManager::Get().GetFireCount();
		maxSize = MapManager::Get().GetMaxFireCount();
	}
	else if (tile == 'X')
	{
		existedSize = MapManager::Get().GetExitCount();
		maxSize = MapManager::Get().GetMaxExitCount();
	}

	for (const Vector2& selPos : selectedPositionInConsole)
	{
		// 불이나 탈출구라면 -> 개수 제한 확인 (현재 개수 + 바뀐 개수 >= 최대 크기)
		if ((tile == 'F' || tile == 'X') && (existedSize + switchedCount >= maxSize))
			break;

		if (!IsEditable(selPos, tile))
		{
			continue;
		}

		// 타일 변경
		MapManager::Get().SetMapTile(selPos, tile);
		switchedCount++;
	}

	// 불 타일 / 탈출구 타일 업데이트
	MapManager::Get().FindImportantTiles();

	// 선택한 영역 초기화
	SelectPositionClear();
}

bool EditMouse::IsEditable(const Vector2& position, const char tile)
{
	// 변경 이전 타일
	char oldTile = MapManager::Get().GetMapPositionData(position);

	// 생존자라면 무시
	if (oldTile == 'S')
	{
		return false;
	}

	// 이미 같은 타일이면 무시
	if (oldTile == tile)
	{
		return false;
	}

	// 빈 공간으로 변경시 맵 경계라면 무시
	if (tile == ' ')
	{
		if (position.x == 0 || position.x == MapManager::Get().GetMapWidth() - 1 ||
			position.y == 0 || position.y == MapManager::Get().GetMapHeight() - 1)
		{
			return false;
		}
	}

	// 탈출구는 맵 경계에만 생성 가능
	if (tile == 'X')
	{
		if (MapManager::Get().IsExitablePosition(position))
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	// 그외는 변경 가능
	return true;
}

bool EditMouse::IsExitable()
{
	// 탈출구를 만들지 않은 경우
	if (MapManager::Get().GetExitCount() == 0)
	{
		// BFS 알고리즘을 통해 가능한 맵경계를 찾자!
		BFS bfs;
		auto exitablePosition = bfs.FindExitableTile();

		// 탈출할 수 있는 경로가 없는 경우 반환 및 로그
		if (exitablePosition.empty())
		{
			// 로그 출력
			// 로그가 스택 형식으로 출력되므로 이후에 나올 로그를 먼저 작성
			LogManager::Get().PrintLog("맵을 다시 편집해주세요.");
			LogManager::Get().PrintLog("탈출할 수 있는 경로가 없습니다.");
			return false;
		}

		// 가능한 탈출경로 존재시 랜덤으로 하나 생성
		int length = static_cast<int>(exitablePosition.size());
		int exitIndex = Util::Random(0, length - 1);

		MapManager::Get().SetMapTile(exitablePosition[exitIndex], 'X');
		MapManager::Get().FindImportantTiles();

		return true;
	}	

	// 경로 탐색을 위한 AStar 객체 생성
	AStar astar;

	// 모든 플레이어에 대해 탈출 경로가 있는지 확인
	for (const Vector2& position : MapManager::Get().GetSurvivorPositions())
	{
		// 노드 생성
		Node* survivorNode = new Node(position);

		// 경로 탐색
		auto path = astar.FindPath(survivorNode);

		// 경로가 없을 경우 탈출불가
		if (path.empty())
		{
			// 로그 출력
			LogManager::Get().PrintLog("맵을 다시 편집해주세요.");
			LogManager::Get().PrintLog("탈출할 수 있는 경로가 없습니다.");
			return false;
		}

		// 재탐색을 위해 초기화
		astar.ClearSetting();
	}

	// 모두 탈출경로가 있다면 true 반환
	return true;
}
