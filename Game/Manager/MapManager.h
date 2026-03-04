#pragma once
#include "Math/Vector2.h"

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
	void SetMapTile(const Vector2& position);

	// 게임을 새롭게 시작할 때 사용하는 함수
	void SetNewGame();

	// Getter
	inline const std::vector<std::string>& GetMapData() { return mapData; }
	inline const int GetMapWidth() { return mapWidth; }
	inline const int GetMapHeight() { return mapHeight; }
	inline const char GetMapPositionData(const Vector2& position) { return mapData[position.x][position.y]; }

	// 싱글톤 접근 함수
	static MapManager& Get();

private:
	// 초기 세팅 함수
	void Initialize();

private:
	// 맵을 저장할 변수
	std::vector<std::string> mapData;

	// 맵 크기 변수
	int mapWidth = 0;
	int mapHeight = 0;

	// 싱글톤 구현을 위한 정적 변수
	static MapManager* instance;
};

