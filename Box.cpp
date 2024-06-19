#include "Box.h"
#include "Engine.h"
#include "PowerUp.h"
#include "BowlingBall.h"
#include "OilSpill.h"
#include <cmath>


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
}

void Box::update(float dt)
{

}

//void Box::draw(sf::RenderTarget& target)
//{
//	sf::RectangleShape shape({ 10.f, 10.f });
//	shape.setOrigin(5.f, 5.f);
//	shape.setPosition(m_position);
//	shape.setFillColor(sf::Color::Cyan);
//	target.draw(shape);
//}


