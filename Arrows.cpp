#include "Arrows.h"
#include "Engine.h"
#include "SteeringAdjuster.h"

void Arrows::usePowerUp(Vehicle& owner)
{
	sf::Vector2f unitVec = owner.getUnitVector();

	const float launchSpeed = 40.f;
	const float minSpeed = 80.f;

	const float speed = PowerUp::calculateLaunchSpeed(owner.getSpeed(), launchSpeed, minSpeed);

	Engine& engine = *Engine::getInstance();
	auto arrows = std::make_unique<SteeringAdjuster>(
		engine.getTexture("arrows"), -1.f, unitVec * speed);
	arrows->setPosition(owner.getPosition() + unitVec * 20.f);
	engine.addObject(std::move(arrows));
}
