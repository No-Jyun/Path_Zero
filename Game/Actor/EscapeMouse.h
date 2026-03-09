#pragma once
#include "Actor/MouseBase.h"

using namespace Wanted;

class EscapeMouse : public MouseBase
{
	// RTTI 선언
	RTTI_DECLARATIONS(EscapeMouse, MouseBase)

public:
	EscapeMouse(const Vector2& position, std::vector<class Survivor*>* allSurvivors);
	virtual ~EscapeMouse();

	virtual void Tick(float deltaTime) override;
	virtual void Draw() override;

private:
	void EraseUnaccessableSurvivor();

private:
	// 현재 선택한 생존자를 저장할 배열
	std::vector<class Survivor*> selectedSurvivor;

	// 전체 생존자 목록의 주소를 저장할 포인터 변수
	std::vector<class Survivor*>* allSurvivors = nullptr;
};

