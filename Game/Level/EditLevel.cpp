#include "EditLevel.h"
#include "Manager/MapManager.h"
#include "Engine/Engine.h"
#include "Render/Renderer.h"

const char* instructionString[] =
{
	"Hi",
	"I'm Instruction"
};

EditLevel::EditLevel()
{
	MapManager::Get().SetNewGame();
}

EditLevel::~EditLevel()
{
}

void EditLevel::Draw()
{
	super::Draw();

	DrawMap();
	DrawInstruction();
}

void EditLevel::DrawMap()
{
	const std::vector<std::string>& map = MapManager::Get().GetMapData();

	int rows = MapManager::Get().GetMapHeight();
	int cols = MapManager::Get().GetMapWidth();

	for (int i = 0; i < rows; i++)
	{
		Vector2 drawPosition = Vector2::Zero;
		drawPosition.y += i;

		Renderer::Get().Submit(map[i].c_str(), drawPosition);
	}
}

void EditLevel::DrawInstruction()
{
	size_t length = sizeof(instructionString) / sizeof(instructionString[0]);

	int offsetX = MapManager::Get().GetMapWidth() + 5;

	for (int i = 0; i < length; i++)
	{
		Vector2 drawPosition = Vector2(offsetX, 1);
		drawPosition.y += i;

		Renderer::Get().Submit(instructionString[i], drawPosition);
	}
}
