#include "Box.h"
#include "Engine.h"
#include "PowerUp.h"
#include "BowlingBall.h"
#include "OilSpill.h"
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
	srand(time(NULL));
	auto x = rand();
	if (x % 2 == 0)
		vehicle.setPowerUp(std::make_unique<BowlingBall>());
	else
		vehicle.setPowerUp(std::make_unique<OilSpill>());
	auto engine = Engine::getInstance();
	engine->flagForRemoval(this);

	GroundItem::interactWithVehicle(vehicle);
}

void Box::update(float dt)
{

}


