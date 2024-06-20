#include "Engine.h"
#include "BowlingBall.h"
#include "SpeedAdjuster.h"

void BowlingBall::usePowerUp(Vehicle& owner)
{

	sf::Vector2f unitVec = owner.getUnitVector();

	Engine& engine = *Engine::getInstance();
	
	const float launchSpeed = 80.f;
	const float minSpeed = 100.f;

	const float speed = PowerUp::calculateLaunchSpeed(owner.getSpeed(), launchSpeed, minSpeed);

	auto ball = std::make_unique<SpeedAdjuster>(
		engine.getTexture("bowling"), 0.2f, unitVec * speed);
	ball->setPosition(owner.getPosition() + unitVec * 20.f);
	engine.addObject(std::move(ball));
}