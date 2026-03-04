#include "MouseBase.h"
#include "Render/Renderer.h"

#include <vector>

using namespace Wanted;

MouseBase::MouseBase()
{
}

MouseBase::~MouseBase()
{
}

void MouseBase::Tick(float deltaTime)
{

}

void MouseBase::Draw()
{
	super::Draw();

	for (Vector2 selPos : selectedPosition)
	{
		Renderer::Get().Submit(" ", selPos, Color::BackGoundWhite, sortingOrder + 3);
	}
}

void MouseBase::SelectPositionClear()
{
	firstSelectedPosition = Vector2::Zero;
	lastSelectedPosition = Vector2::Zero;

	selectedPosition.clear();
}

void MouseBase::DragProcess()
{
}
