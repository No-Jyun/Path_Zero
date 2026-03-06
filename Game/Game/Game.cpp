#include "Game.h"
#include "Level/EditLevel.h"
#include "Level/EscapeLevel.h"
#include "Manager/MapManager.h"
#include "Manager/LogManager.h"
#include "Util/Util.h"

#include <Windows.h>

// 정적 변수 초기화
Game* Game::instance = nullptr;

Game::Game()
{
	instance = this;

	mapManager = new MapManager();

	logManager = new LogManager();

	levels.emplace_back(new EditLevel());
	levels.emplace_back(new EscapeLevel());

	mainLevel = levels[0];
}

Game::~Game()
{
	// 중복 제거 방지
	mainLevel = nullptr;

	SafeDelete(mapManager);
	SafeDelete(logManager);

	// 모든 레벨 삭제
	for (Level*& level : levels)
	{
		delete level;
		level = nullptr;
	}

	// 배열 정리
	levels.clear();
}

void Game::ToggleMenu(int levelIndex)
{
	// 화면 지우기
	system("cls");

	// 메인 레벨 변경
	mainLevel = levels[levelIndex];
	mainLevel->LevelSetting();
}

Game& Game::Get()
{
	// 예외 처리
	if (!instance)
	{
		// 오류 메시지 출력.
		MessageBoxA(
			nullptr,
			"Game::Get() - instance is null",
			"Error",
			MB_OK
		);

		__debugbreak();
	}

	// 정적 변수 반환
	return *instance;
}