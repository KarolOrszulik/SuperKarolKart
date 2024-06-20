#include "Arrows.h"
#include "Engine.h"
#include "SteeringAdjuster.h"

void Arrows::usePowerUp(Vehicle& owner)
{
	sf::Vector2f unitVec = owner.getUnitVector();

	Engine& engine = *Engine::getInstance();
	auto arrows = std::make_unique<SteeringAdjuster>(
		engine.getTexture("arrows"), -1.f, unitVec * (owner.getSpeed() + 40.f));
	arrows->setPosition(owner.getPosition() + unitVec * 20.f);
	engine.addObject(std::move(arrows));
}
