#pragma once

#include "Level/Level.h"
#include "Util/Timer.h"

using namespace Wanted;

class EscapeLevel : public Level
{
	// RTTI 선언
	RTTI_DECLARATIONS(EscapeLevel, Level)

public:
	EscapeLevel();
	~EscapeLevel();

	virtual void Tick(float deltaTime) override;
	virtual void Draw() override;
	virtual void LevelClear() override;

	// 레벨이 토글됐을 때 호출할 이벤트 함수
	virtual void LevelSetting() override;

private:
	// 맵을 그리는 함수
	void DrawMap();

	// 설명서를 보여주는(그리는) 함수
	void DrawInstruction();

	void Initialize();

private:
	// 로그 액터를 저장할 배열
	std::vector<class LogActor*> logVector;

	// 생존자 액터를 저장할 배열
	std::vector<class Survivor*> survivorVector;

	// 마우스 액터 저장용 변수
	class EscapeMouse* mouseActor = nullptr;

	// 불 확산 제어 타이머
	Timer fireSpreadTimer;

	// 불 타일 색상 제어용 변수
	bool colorSwap = false;

	// 생존자 이동 제어 타이머
	Timer survivorMoveTimer;

	// 일시정지 팝업 액터를 저장할 변수
	class PauseMenuPopup* pauseMenuActor = nullptr;

	// 생존자가 맵에 없고 대기시간 타이머
	Timer gameEndTimer;

	int exitSurvivorNum = 0;

	int burnSurvivorNum = 0;

	int logIndexx = 0;
};

