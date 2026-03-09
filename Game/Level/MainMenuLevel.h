#pragma once
#include "Level/Level.h"
#include "Math/Color.h"
#include "Math/Vector2.h"

#include <vector>

using namespace Wanted;

// 메뉴 아이템 구조체
struct MenuItem
{
	// 메뉴 선택됐을 때 실행될 함수의 타입
	using OnSelected = void(*)();

	MenuItem(const char** text, int lineCount, OnSelected onSelected)
		: onSelected(onSelected), lineCount(lineCount)
	{
		this->text = new char* [lineCount];

		for (int i = 0; i < lineCount; i++)
		{
			// 텍스트 복사
			size_t length = strlen(text[i]) + 1;
			this->text[i] = new char[length];
			strcpy_s(this->text[i], length, text[i]);
		}
	}

	~MenuItem()
	{
		if (text)
		{
			for (int i = 0;i < lineCount;i++)
			{
				delete[] text[i];
			}

			delete[] text;
			text = nullptr;
		}
	}

	// 메뉴 텍스트
	char** text = nullptr;
	// 텍스트가 몇 줄인지 저장
	int lineCount = 0;

	// 화면 상 위치 정보
	Vector2 topLeft = Vector2::Zero;
	Vector2 bottomRight = Vector2::Zero;

	// 메뉴 선택됐을 때 실행될 로직(함수 - 함수 포인터)
	OnSelected onSelected = nullptr;
};

class MainMenuLevel : public Level
{
	// RTTI 선언
	RTTI_DECLARATIONS(MainMenuLevel, Level)

public:
	MainMenuLevel();
	virtual ~MainMenuLevel();

	// 이벤트 함수 오버라이드
	virtual void Tick(float deltaTime) override;
	virtual void Draw() override;

	virtual void LevelSetting() override;

private:
	// 메뉴 로고 그리는 함수
	void DrawLogo();

	// 메뉴 아이템 그리는 함수
	void DrawItem();

	void SetVectorPosition();

private:
	// 현재 활성화된 메뉴 아이템 인덱스
	int currentIndex = 0;

	// 선택된 아이템의 색상
	Color selectedColor = Color::Green;

	// 선택되지 않은 아이템의 색상
	Color unselectedColor = Color::White;

	// 메뉴 아이템 배열
	std::vector<MenuItem*> items;

	// 메뉴 로고 배열 크기
	int logoArrayLength = 0;

	// 콘솔 화면의 정중앙 값을 저장할 좌표
	Vector2 consoleCenter;

	// 메뉴 로고 그리는 좌표
	Vector2 logoPosition;

	// 메뉴 아이템을 그리는 좌표
	Vector2 itemPosition;

};

