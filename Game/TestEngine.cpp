#include "TestEngine.h"
#include "TestLevel.h"

TestEngine::TestEngine()
{
	SetNewLevel(new TestLevel());
}

TestEngine::~TestEngine()
{
}
