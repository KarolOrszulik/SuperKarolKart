#include "OilSpill.h"
#include "SpeedAdjuster.h"
#include "SteeringAdjuster.h"
#include "Engine.h"

void OilSpill::usePowerUp(Vehicle& owner)
{
	std::shared_ptr<Engine> engine = Engine::getInstance();
	sf::Vector2f unitVec = -owner.getUnitVector();

	auto oilSpill = std::make_unique<SpeedAdjuster>(engine->getTexture("oil"), 0.2f);
	oilSpill->setPosition(owner.getPosition() + unitVec * engine->getGridSizeF() * 1.2f);
	engine->addObject(std::move(oilSpill));
}