#pragma once
#include "Level/Level.h"

using namespace Wanted;

class EditLevel : public Level
{
	// RTTI 선언
	RTTI_DECLARATIONS(EditLevel, Level)

public:
	EditLevel();
	~EditLevel();

	virtual void Draw() override;
	virtual void LevelSetting() override;
	
private:
	// 맵을 그리는 함수
	void DrawMap();

	// 설명서를 보여주는(그리는) 함수
	void DrawInstruction();

private:

};

