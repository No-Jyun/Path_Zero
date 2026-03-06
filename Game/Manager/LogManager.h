#pragma once

#include "Math/Color.h"
#include <vector>

using namespace Wanted;

class LogManager
{
public:
	LogManager();
	~LogManager();

	// 레벨에서 로그 매니저를 사용하기 전에 LogActor를 전달해주는 함수
	void Initialize(const std::vector<class LogActor*>& logActors);

	void PrintLog(const char* logText, Color color = Color::White);

	// Getter
	inline const int GetLogCount() { return logCount; }

	// 싱글톤 접근 함수
	static LogManager& Get();

private:
	const int logCount = 10;

	// 스택처럼 사용할 배열의 Top을 가리킬 인덱스 변수
	int logStackTop = 0;

	// 로그를 스택처럼 관리할 배열
	std::vector<class LogActor*> logStack;

	// 싱글톤 구현을 위한 정적 변수
	static LogManager* instance;
};

