#include "OilSpill.h"
#include "SpeedAdjuster.h"
#include "SteeringAdjuster.h"
#include "Engine.h"

void OilSpill::usePowerUp(Vehicle& owner)
{
	std::shared_ptr<Engine> engine = Engine::getInstance();
	sf::Vector2f unitVec = -owner.getUnitVector();

	auto oilSpill = std::make_unique<SteeringAdjuster>(engine->getTexture("oil"));
	oilSpill->setPosition(owner.getPosition() + unitVec * 30.f);
	engine->addObject(std::move(oilSpill));
}