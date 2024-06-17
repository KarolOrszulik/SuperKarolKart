#include "Engine.h"
#include "BowlingBall.h"
#include "SpeedAdjuster.h"

void BowlingBall::usePowerUp(Vehicle& owner)
{

	sf::Vector2f unitVec = owner.getUnitVector();

	auto ball = std::make_unique<SpeedAdjuster>(0.2f, unitVec * 150.f);
	ball->setPosition(owner.getPosition() + unitVec * 20.f);
	Engine::getInstance()->addObject(std::move(ball));
}

