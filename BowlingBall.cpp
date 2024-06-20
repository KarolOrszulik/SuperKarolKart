#include "Engine.h"
#include "BowlingBall.h"
#include "SpeedAdjuster.h"

void BowlingBall::usePowerUp(Vehicle& owner)
{

	sf::Vector2f unitVec = owner.getUnitVector();

	Engine& engine = *Engine::getInstance();
	auto ball = std::make_unique<SpeedAdjuster>(
		engine.getTexture("bowling"), 0.2f, unitVec * 250.f);
	ball->setPosition(owner.getPosition() + unitVec * 20.f);
	engine.addObject(std::move(ball));
}