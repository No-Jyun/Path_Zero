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

	// 탈출구 / 불 위치를 찾아 저장하는 함수
	void FindImportantTiles();

	// Getter
	inline const std::vector<std::string>& GetMapData() { return mapData; }
	inline const int GetMapWidth() { return mapWidth; }
	inline const int GetMapHeight() { return mapHeight; }
	inline const char GetMapPositionData(const Vector2& position) { return mapData[position.y][position.x]; }

	inline const int GetMaxExitCount() { return maxExitCount; }
	inline const int GetExitCount() { return static_cast<int>(exitPositions.size()); }
	inline const std::vector<Vector2> GetExitPositions() { return exitPositions; }

	inline const std::vector<Vector2> GetSurvivorPositions() { return survivorPositions; }

	inline const int GetMaxFireCount() { return maxFireCount; }
	inline const int GetFireCount() { return static_cast<int>(firePositions.size()); }
	inline const std::vector<Vector2> GetFirePositions() { return firePositions; }

	// 싱글톤 접근 함수
	static MapManager& Get();

private:
	// 초기 세팅 함수
	void Initialize();

	// 맵 편집 이후 부족한 불 / 탈출구를 생성하는 함수
	void CheckFireAndExit();

private:
	// 맵을 저장할 변수
	std::vector<std::string> mapData;

	// 탈출구 위치를 저장할 변수
	std::vector<Vector2> exitPositions;

	// 생존자 위치를 저장할 변수
	std::vector<Vector2> survivorPositions;

	// 불 위치를 저장할 변수
	std::vector<Vector2> firePositions;

	// 맵 크기 변수
	int mapWidth = 0;
	int mapHeight = 0;

	// Todo: 불 타일, 탈출구 타일 개수 조정 필요
	
	// 맵에 배치될 수 있는 최대 탈출구 개수
	const int maxExitCount = 3;
	// 맵에 배치될 수 있는 최대 불 개수
	const int maxFireCount = 5;

	// 싱글톤 구현을 위한 정적 변수
	static MapManager* instance;
};

