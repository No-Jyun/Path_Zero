#pragma once

#include "Math/Vector2.h"
#include "Math/Color.h"

using namespace Wanted;

// 헬퍼 (Helper) 기능 제공
namespace Util
{
	// 콘솔 커서 위치 이동(설정)하는 함수
	inline void SetConsolePosition(const Vector2& position)
	{
		// coord의 x와 y 값은 short 자료형이므로 형변환을 해야함
		// 기본형끼리의 형변환이므로 static cast 사용
		//coord.X = static_cast<short>(position.x);
		//coord.Y = static_cast<short>(position.y);

		// 윈도우에선 기본적으로 출력용, 입력용, 에러용 콘솔 3개를 가짐
		SetConsoleCursorPosition(
			GetStdHandle(STD_OUTPUT_HANDLE),
			static_cast<COORD>(position)
		);
	}

	// 콘솔 텍스트 설정 함수
	inline void SetConsoleTextColor(Color color)
	{
		SetConsoleTextAttribute(
			GetStdHandle(STD_OUTPUT_HANDLE),
			static_cast<unsigned short>(color)
		);
	}

	// 커서 끄기
	inline void TurnOffCursor()
	{
		CONSOLE_CURSOR_INFO info = {};
		GetConsoleCursorInfo(
			GetStdHandle(STD_OUTPUT_HANDLE),
			&info
		);

		info.bVisible = false;
		SetConsoleCursorInfo(
			GetStdHandle(STD_OUTPUT_HANDLE),
			&info
		);
	}

	// 커서 켜기
	inline void TurnOnCursor()
	{
		CONSOLE_CURSOR_INFO info = {};
		GetConsoleCursorInfo(
			GetStdHandle(STD_OUTPUT_HANDLE),
			&info
		);

		info.bVisible = true;
		SetConsoleCursorInfo(
			GetStdHandle(STD_OUTPUT_HANDLE),
			&info
		);
	}
}

// 메모리 정리 함수
template<typename T>
void SafeDelete(T*& t)
{
	if (t)
	{
		delete t;
		t = nullptr;
	}
}

template<typename T>
void SafeDeleteArray(T*& t)
{
	if (t)
	{
		delete[] t;
		t = nullptr;
	}
}