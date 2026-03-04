#include "TestLevel.h"
#include "TestActor.h"

TestLevel::TestLevel()
{
	AddNewActor(new TestActor());
}

TestLevel::~TestLevel()
{
}
