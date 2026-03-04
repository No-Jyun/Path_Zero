#pragma once
#include "Actor/Actor.h"

class TestActor : public Wanted::Actor
{
	// RTTI ¼±¾ð
	RTTI_DECLARATIONS(TestActor, Actor)

public:
	TestActor();
	~TestActor();

	virtual void Tick(float deltaTime) override;

private:
	void SayHI();
};

