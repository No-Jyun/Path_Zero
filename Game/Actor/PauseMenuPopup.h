#pragma once
#include "Actor/Actor.h"
#include "Math/Color.h"
#include "Math/Vector2.h"

#include <vector>
#include <string>
#include <functional>

using namespace Wanted;

struct PauseMenuItem
{
	// 메뉴 선택됐을 때 실행될 함수의 타입
	using OnSelected = std::function<void()>;

	PauseMenuItem(const char** text, int lineCount, const Vector2& topLeft, OnSelected onSelected)
		: onSelected(onSelected), lineCount(lineCount), topLeft(topLeft)
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

	virtual ~PauseMenuItem()
	{
		if (text)
		{
			for (int i = 0; i < lineCount; i++)
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

class PauseMenuPopup : public Actor
{
	// RTTI 선언
	RTTI_DECLARATIONS(PauseMenuPopup, Actor)

public:
	PauseMenuPopup();
	~PauseMenuPopup();

	virtual void Tick(float deltaTime) override;
	virtual void Draw() override;

	void SetActive(bool active);

private:
	void Initialize();

	void DrawLogo();

	void DrawItem();

private:
	std::vector<PauseMenuItem*> menuItems;

	int currentIndex = -1;
	
	// 배경 로고 박스 크기 및 위치
	Vector2 boxPos;
	Vector2 boxSize;
};

