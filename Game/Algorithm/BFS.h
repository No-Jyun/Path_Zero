#pragma once
#include "Math/Vector2.h"

#include <string>
#include <vector>

using namespace Wanted;

// 너비 우선 탐색 알고리즘 클래스
// 노드는 사용하지 않고, 맵 타일만 확인
class BFS
{
public:
	BFS();
		
	// 탈출구 생성용 BFS
	std::vector<Vector2> findExitableTile();

	// 불타일의 BFS

private:
	// 맵 매니저로부터 맵 정보를 가져와 저장하는 함수
	void LoadMapData();

	// 대각선 방향 검증 함수
	bool IsMovableDiagnal(const Vector2& curPos, const Vector2& direction);

private:
	// 맵 정보를 저장할 배열
	std::vector<std::string> mapData;

	// 맵 방문 정보를 저장할 배열
	std::vector<std::vector<bool>> mapVisited;
};

