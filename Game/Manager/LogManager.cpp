#include "LogManager.h"
#include "Actor/LogActor.h"

// 정적 변수 초기화
LogManager* LogManager::instance = nullptr;

LogManager::LogManager()
{
	instance = this;
}

LogManager::~LogManager()
{
	// LogActor는 실제론 레벨 소유이므로 메모리 관리 X
	std::vector<LogActor*>().swap(logStack);
}

void LogManager::Initialize(const std::vector<class LogActor*>& logActors)
{
	// 저장되어 있던 배열 Clear
	std::vector<LogActor*>().swap(logStack);

	logStack = logActors;
}

void LogManager::PrintLog(const char* logText, Color color)
{
	// 로그 액터의 위치를 바꿔서 스택처럼 보이도록
	Vector2 firstLogPos = logStack[0]->GetPosition();
	for (int i = 0; i < logCount; i++)
	{
		// 0->1, 1->2 ... 4->0
		int nextIndex = (i + 1) % logCount;
		logStack[i]->SetPosition(logStack[nextIndex]->GetPosition());
	}
	logStack[logCount - 1]->SetPosition(firstLogPos);

	// 인덱스 돌리기
	logStackTop -= 1;
	if (logStackTop < 0)
	{
		logStackTop = logCount - 1;
	}
	// 맨 위의 로그 액터에 출력
	logStack[logStackTop]->PrintLog(logText, color);
}

LogManager& LogManager::Get()
{
	// 예외 처리
	if (!instance)
	{
		// 오류 메시지 출력.
		MessageBoxA(
			nullptr,
			"LogManager::Get() - instance is null",
			"Error",
			MB_OK
		);

		__debugbreak();
	}

	// 정적 변수 반환
	return *instance;
}
