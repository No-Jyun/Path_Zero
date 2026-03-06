#pragma once
#include "Actor/Actor.h"

using namespace Wanted;

// 화면에 로그를 출력해주는 액터
class LogActor : public Actor
{
	// RTTI 선언
	RTTI_DECLARATIONS(LogActor, Actor)

public:
	LogActor(const Vector2& position);

	void PrintLog(const char* text, Color color = Color::White);

private:

};

