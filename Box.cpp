#include "Box.h"
#include "Engine.h"
#include "PowerUp.h"
#include "BowlingBall.h"
#include "OilSpill.h"
#include "Arrows.h"
#include <cmath>


Box::Box(const sf::Texture& texture, 
	sf::Vector2f velocity, 
	bool singleUse)
	: GroundItem(velocity, singleUse)
{
	this->assignTexture(texture);
}

void Box::interactWithVehicle(Vehicle& vehicle)
{
	auto engine = Engine::getInstance();
	srand(time(NULL));
	auto x = rand() % 3;
	switch (x)
	{
	case 0:
		vehicle.setPowerUp(std::make_unique<BowlingBall>(engine->getTexture("bowling")));
		break;
	case 1:
		vehicle.setPowerUp(std::make_unique<OilSpill>(engine->getTexture("oil")));
		break;
	case 2:
		vehicle.setPowerUp(std::make_unique<Arrows>(engine->getTexture("arrows")));
		break;
	}
	engine->flagForRemoval(this);

	GroundItem::interactWithVehicle(vehicle);
}

void Box::update(float dt)
{

}


