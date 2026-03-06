#pragma once
#include "MouseBase.h"
#include "Math/Vector2.h"

using namespace Wanted;

class EditMouse : public MouseBase
{
	// RTTI ¼±¾ð
	RTTI_DECLARATIONS(EditMouse, MouseBase)

public:
	EditMouse(const Vector2& position);
	~EditMouse();

	virtual void Tick(float deltaTime) override;

private:
	void MakeTile(const char tile);

	bool IsEditable(const Vector2& position, const char tile);

private:


};

