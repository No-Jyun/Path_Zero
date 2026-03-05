#pragma once
#include "Math/Vector2.h"
#include "Math/Color.h"

#include <vector>
#include <string>

using namespace Wanted;

// 맵 데이터를 가지고 있는 클래스
class MapManager
{
public:
	MapManager();
	~MapManager();

	// 특정 좌표의 맵 타일을 바꾸는 함수
	void SetMapTile(const Vector2& position, const char tile);

	// 게임을 새롭게 시작할 때 사용하는 함수
	void SetNewGame();

	// 맵 편집이 끝나고 게임을 시작하는 함수
	void StartGame();

	// Getter
	inline const std::vector<std::string>& GetMapData() { return mapData; }
	inline const int GetMapWidth() { return mapWidth; }
	inline const int GetMapHeight() { return mapHeight; }
	inline const char GetMapPositionData(const Vector2& position) { return mapData[position.x][position.y]; }
	inline const std::vector<Vector2> GetExitPositions() { return exitPositions; }

	// 싱글톤 접근 함수
	static MapManager& Get();

private:
	// 초기 세팅 함수
	void Initialize();

	// 탈출구를 찾아 저장해놓는 함수
	void FindExits();

private:
	// 맵을 저장할 변수
	std::vector<std::string> mapData;

	// 탈출구를 저장할 변수
	std::vector<Vector2> exitPositions;

	// 맵 크기 변수
	int mapWidth = 0;
	int mapHeight = 0;

	// 싱글톤 구현을 위한 정적 변수
	static MapManager* instance;
};

