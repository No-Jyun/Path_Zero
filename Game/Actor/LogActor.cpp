#include "LogActor.h"

LogActor::LogActor(const Vector2& position)
	: super(" ", position)
{
	sortingOrder = 2;

	// 로그는 5초간 표시
	timer.SetTargetTime(5.0f);
}

void LogActor::Tick(float deltaTime)
{
	super::Tick(deltaTime);

	// 현재 타이머가 켜져있으면
	if (isTimerTicking)
	{
		timer.Tick(deltaTime);

		// 타이머 종료시
		if (timer.IsTimeOut())
		{
			// 타이머 리셋 및 변수 초기화
			timer.Reset();
			isTimerTicking = false;

			// 로그 삭제
			PrintLog(" ");
		}
	}
}

void LogActor::PrintLog(const char* text, Color color)
{
	// 이미 타이머가 진행중이었다면
	if (isTimerTicking)
	{
		timer.Reset();
	}

	// 타이머 시작
	isTimerTicking = true;

	// 문자열 변경
 	ChangeImage(text);
	this->color = color;
}
