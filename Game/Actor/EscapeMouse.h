#pragma once
#include "Actor/MouseBase.h"

using namespace Wanted;

class EscapeMouse : public MouseBase
{
	// RTTI ¼±¾ð
	RTTI_DECLARATIONS(EscapeMouse, MouseBase)

public:
	EscapeMouse(const Vector2& position);
	~EscapeMouse();

	virtual void Tick(float deltaTime) override;

private:
	std::vector<class Survivor*> selectedSurvivor;
};

