#include "Vehicle.h"

void Vehicle::applyAccelerator(float accelerator)
{
	m_acceleration += accelerator;
}

void Vehicle::applySteering(float steering)
{
	m_steering += steering;
}

void Vehicle::update(float dt)
{
	constexpr float SPEED = 200.0f;

	m_position.y -= m_acceleration * SPEED * dt; // tylko demo
	m_position.x += m_steering * SPEED * dt; // tylko demo

	m_acceleration = 0.0f;
	m_steering = 0.0f;
}

void Vehicle::draw(sf::RenderTarget& window)
{
	constexpr float RADIUS = 20.0f;
	sf::CircleShape shape(RADIUS);
	shape.setOrigin(RADIUS, RADIUS);
	shape.setFillColor(sf::Color::Red);
	shape.setPosition(m_position);
	window.draw(shape);
}