#include "LogActor.h"

LogActor::LogActor(const Vector2& position)
	: super(" ", position)
{
	sortingOrder = 2;
}

void LogActor::PrintLog(const char* text, Color color)
{
	ChangeImage(text);
	this->color = color;
}
