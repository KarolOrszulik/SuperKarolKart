#include "SteeringAdjuster.h"
#include "Engine.h"
#include "Vehicle.h"

void SteeringAdjuster::interactWithVehicle(Vehicle& vehicle)
{
	vehicle.setSteeringMultiplier(this->m_steeringMultiplier, 2.f);
	auto engine = Engine::getInstance();

	if (m_singleUse)
		engine->flagForRemoval(this);
}

void SteeringAdjuster::update(float dt)
{
	m_position += m_velocity * dt;
}

void SteeringAdjuster::draw(sf::RenderTarget& target)
{
	sf::RectangleShape shape({ 10.f, 10.f });
	shape.setOrigin(5.f, 5.f);
	shape.setPosition(m_position);
	shape.setFillColor(sf::Color::Blue);
	target.draw(shape);
}
