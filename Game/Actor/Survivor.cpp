#include "Survivor.h"
#include "Util/Util.h"
#include "Algorithm/AStar.h"
#include "Algorithm/Node.h"
#include "Render/Renderer.h"

Survivor::Survivor(const Vector2& position, Color color)
	: super("P", position, color)
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

void Survivor::Tick(float deltaTime)
{
	super::Tick(deltaTime);
	timer.Tick(deltaTime);

	bool isFindPath = FindPath();

	// Todo: 경로 없으면 로그 출력 및 애니메이션
	if (!isFindPath)
	{
		return;
	}
	
	Move();
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
	Renderer::Get().Submit("X", nodePos, GetPathColor(), sortingOrder);
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

void Survivor::Move()
{
	if (!timer.IsTimeOut())
	{
		return;
	}

	timer.Reset();

	// 1칸 이동할 노드
	Node* nextNode = path[0];

	// 경로에서 삭제
	path.erase(path.begin());

	position = nextNode->GetPosition();
}

Color Survivor::GetPathColor()
{
	return static_cast<Color>(static_cast<unsigned short>(color) | FOREGROUND_INTENSITY);
}
