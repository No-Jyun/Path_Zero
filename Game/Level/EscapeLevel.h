#pragma once

#include "Level/Level.h"

using namespace Wanted;

class EscapeLevel : public Level
{
	// RTTI 선언
	RTTI_DECLARATIONS(EscapeLevel, Level)

public:
	EscapeLevel();
	~EscapeLevel();

	virtual void Draw() override;

	// 레벨이 토글됐을 때 호출할 이벤트 함수
	virtual void LevelSetting() override;

private:
	// 맵을 그리는 함수
	void DrawMap();

	void Initialize();

private:

};

