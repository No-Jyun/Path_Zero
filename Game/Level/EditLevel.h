#pragma once
#include "Level/Level.h"
#include "Math/Vector2.h"

using namespace Wanted;

class EditLevel : public Level
{
	// RTTI 선언
	RTTI_DECLARATIONS(EditLevel, Level)

public:
	EditLevel();
	~EditLevel();

	virtual void Tick(float deltaTime) override;
	virtual void Draw() override;
	virtual void LevelSetting() override;
	
private:
	// 맵을 그리는 함수
	void DrawMap();

	// 설명서를 보여주는(그리는) 함수
	void DrawInstruction();

private:
	// 로그 액터 저장하는 배열
	std::vector<class LogActor*> logVector;

	Vector2 survivorLogPosition = Vector2::Zero;
	int survivorLogIndex = 20;
	int survivorMovementSpeed = 6;

	Vector2 fireLogPosition = Vector2::Zero;
	int fireLogIndex = 22;
	int fireSpredSpeed = 4;
};

