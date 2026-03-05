#pragma once
#include "Engine/Engine.h"

#include <vector>

using namespace Wanted;

class Game : public Engine
{
public:
	Game();
	~Game();

	// 메뉴/레벨을 전환하는 함수
	void ToggleMenu(int levelIndex);

	// 싱글톤 접근 함수
	static Game& Get();

private:
	// 게임에서 관리하는 레벨을 저장할 배열
	std::vector<Level*> levels;

	// 맵 매니저 객체
	class MapManager* mapManager = nullptr;

	// 싱글톤 구현을 위한 정적 변수
	static Game* instance;
};