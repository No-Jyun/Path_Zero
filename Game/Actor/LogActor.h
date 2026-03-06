#pragma once
#include "Actor/Actor.h"
#include "Util/Timer.h"

using namespace Wanted;

// 화면에 로그를 출력해주는 액터
class LogActor : public Actor
{
	// RTTI 선언
	RTTI_DECLARATIONS(LogActor, Actor)

public:
	LogActor(const Vector2& position);

	virtual void Tick(float deltaTime) override;

	// 로그 출력 함수
	void PrintLog(const char* text, Color color = Color::White);

private:
	// 로그를 표시할 타이머
	Timer timer;

	// 현재 타이머가 작동 중 인지 확인하는 변수
	bool isTimerTicking = false;
};

