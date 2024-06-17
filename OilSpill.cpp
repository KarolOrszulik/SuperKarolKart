#include "OilSpill.h"
#include "SpeedAdjuster.h"
#include "SteeringAdjuster.h"
#include "Engine.h"

void OilSpill::usePowerUp(Vehicle& owner)
{
	sf::Vector2f unitVec = -owner.getUnitVector();

	auto oilSpill = std::make_unique<SteeringAdjuster>(-1.f);
	oilSpill->setPosition(owner.getPosition() + unitVec * 30.f);
	Engine::getInstance()->addObject(std::move(oilSpill));
}