#include "TestActor.h"
#include "Render/Renderer.h"
#include "Math/Vector2.h"

using namespace Wanted;

TestActor::TestActor()
{
}

TestActor::~TestActor()
{
}

void TestActor::Tick(float deltaTime)
{
	super::Tick(deltaTime);
	SayHI();
}

void TestActor::SayHI()
{
	Renderer::Get().Submit("Hi", Vector2(5, 5));
}
