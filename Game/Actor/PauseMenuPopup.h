#pragma once
#include "Actor/Actor.h"
#include "Math/Color.h"
#include "Math/Vector2.h"

#include <vector>
#include <string>

using namespace Wanted;

struct PauseMenuItem
{
	std::string text;
	Vector2 position;
	Vector2 size;
	
	// 클릭시 실행될 동작을 구분하기 위한 ID
	int id;
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

private:
	std::vector<PauseMenuItem> menuItems;

	int currentIndex = -1;
	
	// 배경 로고 박스 크기 및 위치
	Vector2 boxPos;
	Vector2 boxSize;
};

