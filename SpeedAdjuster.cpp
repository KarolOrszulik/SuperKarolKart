#include "SpeedAdjuster.h"
#include "Vehicle.h"
#include "Engine.h"

SpeedAdjuster::SpeedAdjuster(
	const sf::Texture& texture,
	float speedMultiplier,
	sf::Vector2f velocity,
	bool singleUse)
	: m_speedMultiplier(speedMultiplier), GroundItem(velocity, singleUse)
{
	assignTexture(texture);
}

void SpeedAdjuster::interactWithVehicle(Vehicle& vehicle)
{
	vehicle.setSpeedMultiplier(m_speedMultiplier, 2.f);

	GroundItem::interactWithVehicle(vehicle);
}

void SpeedAdjuster::update(float dt)
{
	m_position += m_velocity * dt;
}

//void SpeedAdjuster::draw(sf::RenderTarget& target)
//{
//	sf::RectangleShape shape({ 10.f, 10.f });
//	shape.setOrigin(5.f, 5.f);
//	shape.setPosition(m_position);
//	shape.setFillColor(m_speedMultiplier > 1.f ? sf::Color::Green : sf::Color::Red);
//	if (!m_singleUse)
//	{
//		shape.setOutlineColor(sf::Color(0.f,128.f,0.f));
//		shape.setOutlineThickness(-1.f);
//	}
//	target.draw(shape);
//}