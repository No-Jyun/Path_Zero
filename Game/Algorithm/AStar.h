#pragma once

#include "Node.h"

#include <vector>
#include <Queue>

class AStar
{	
public:
	AStar();
	~AStar();

	// 경로 검색 (탐색) 함수
	// startNode : 시작 지점
	std::vector<Node*> FindPath(Node* startNode);

	// 재탐색을 위해 클리어 함수
	void ClearSetting();

private:
	// 탐색을 완료한 후에 최적 경로를 반환하는 함수
	// 부모 링크를 따라가면서 역추적
	std::vector<Node*> ConstructPath(Node* goalNode);

	// 휴리스틱 (hCost) 계산 함수
	float CalculateHeuristic(Node* currentNode, Node* goalNode);

	// 탐색하려는 위치 (노드) 가 그리드 범위 안에 있는지 확인
	// 유효성 검증
	bool IsInRange(const Vector2& curPos);

	// 이미 방문했는지 확인하는 함수
	bool HasVisited(const Vector2& curPos, float gCost);

	// 탐색하려는 노드가 목표 노드인지 확인하는 함수
	bool IsDestination(const Node* const node);

private:
	// 열린 리스트 (우선순위 큐를 사용하여 fCost가 가장 낮은 노드가 Top에 오도록 정렬됨)
	std::priority_queue<Node*, std::vector<Node*>, Node::PtrCompare> openList;

	// 닫힌 리스트
	std::vector<Node*> closedList;

	// 방문 확인 및 비용 저장용 배열
	std::vector<std::vector<float>> visitedGCost;

	// 목표 노드
	Node* goalNode = nullptr;
};

