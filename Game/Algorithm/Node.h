#pragma once
#include "Math/Vector2.h"

using namespace Wanted;

class Node
{
	// AStar 클래스에서 Node 클래스 접근 가능하게 friend 선언
	friend class AStar;

public:
	Node(const Vector2& position, Node* parentNode = nullptr)
		: position(position), parentNode(parentNode)
	{
	}

	~Node() = default;

	// Getter
	inline const float FCost() { return fCost; }
	inline const Vector2 GetPosition() { return position; }

	// 연산자 오버로딩
	bool operator==(const Node& other) const
	{
		return position.x == other.position.x && position.y == other.position.y;
	}

	// 우선순위 큐에서 사용할 비교 연산에 대한 구조체
	struct PtrCompare
	{
		bool operator()(const Node* left, const Node* right) const
		{
			return left->fCost > right->fCost;
		}
	};

private:
	// 노드 위치
	Vector2 position;

	// 현재 노드에서 이 노드까지의 실제 이동 비용
	float gCost = 0.0f;
	// 이 노드에서 목표 노드까지의 예상 이동 비용 (휴리스틱)
	float hCost = 0.0f;
	// 최종 비용 (gCost + hCost)
	float fCost = 0.0f;

	// 부모 노드 링크로 경로 추적
	Node* parentNode = nullptr;
};