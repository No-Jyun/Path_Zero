#include "Input.h"
#include <Windows.h>
#include <iostream>

namespace Wanted
{
	// 전역 변수 초기화
	Input* Input::instance = nullptr;

	Input::Input()
	{
		// 객체가 초기화 되면 자기 자신의 주소를 저장
		instance = this;
	}

	Input::~Input()
	{
		// 메모리 관리는 엔진 쪽에서 해줄 것임
	}

	bool Input::GetKeyDown(int keyCode)
	{
		return keyStates[keyCode].isKeyDown
			&& !keyStates[keyCode].wasKeyDown;
	}

	bool Input::GetKeyUp(int keyCode)
	{
		return !keyStates[keyCode].isKeyDown
			&& keyStates[keyCode].wasKeyDown;
	}

	bool Input::GetKey(int keyCode)
	{
		return keyStates[keyCode].isKeyDown;
	}

	Input& Input::Get()
	{
		// 싱글턴 (Singleton) : 객체를 1개만 유지하는 패턴
		// 이 함수는 콘텐츠 프로젝트에서 접근함
		// 따라서 엔진은 이미 초기화 완료 상태
		if (!instance)
		{
			// return *nullptr;
			std::cout << "Error : Input::Get(). instance is null\n";

			// 디버그 모드에서만 동작함
			// 자동으로 중단점 걸림
			__debugbreak();
		}

		// Lazy-Pattern : 생성 직전에 함
		// 이펙티브 C++에 나옴
		//static Input instance;
		//return instance;

		return *instance;
	}


	void Input::ProcessInput()
	{
		// 키 마다의 입력 읽기
		// !!! 운영체제가 제공하는 기능을 사용할 수 밖에 없음
		// 함수가 호출될 때 키가 눌렸는지, 이전 호출 이후 키를 눌렀는지 알려줌
		for (int ix = 0; ix < 255; ix++)
		{
			keyStates[ix].isKeyDown
				= (GetAsyncKeyState(ix) & 0x8000) > 0 ? true : false;
		}
	}

	void Input::SavePreviousInputStates()
	{
		// 현재 입력 값을 이전 입력 값으로 저장
		for (int ix = 0; ix < 255; ix++)
		{
			keyStates[ix].wasKeyDown
				= keyStates[ix].isKeyDown;
		}
	}
}
