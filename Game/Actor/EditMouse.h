#pragma once
#include "MouseBase.h"
#include "Math/Vector2.h"

using namespace Wanted;

class EditMouse : public MouseBase
{
	// RTTI 선언
	RTTI_DECLARATIONS(EditMouse, MouseBase)

public:
	EditMouse(const Vector2& position);
	~EditMouse();

	virtual void Tick(float deltaTime) override;
	virtual void Draw() override;

private:
	void MakeTile(const char tile);

	bool IsEditable(const Vector2& position, const char tile);

	// 맵 편집 종료 전에 생존자가 탈출할 수 있는지 확인하는 함수
	bool IsExitable();

private:


};

