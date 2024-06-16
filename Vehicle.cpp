#include "Vehicle.h"

#include <iostream>

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
	Track::Tile tile = m_track->atPos(m_position);
	
	float SPEED = tile == Track::Tile::GRASS ? 100.f : 200.f;

	m_position.y -= m_acceleration * SPEED * dt; // tylko demo
	m_position.x += m_steering * SPEED * dt; // tylko demo

	m_acceleration = 0.0f;
	m_steering = 0.0f;


	// sprawdzamy czy pojazd przekroczy³ checkpoint
	std::optional<int> checkpointIndex = m_track->getCheckpointIndex(m_position);
	if (checkpointIndex.has_value())
	{
		if (checkpointIndex == m_nextCheckpoint)
		{
			m_nextCheckpoint = (m_nextCheckpoint + 1) % m_track->getCheckpointCount();
			std::cout << "Gracz " << this << " przeszedl checkpoint " << checkpointIndex.value() << std::endl;
			if (m_nextCheckpoint == 0)
			{
				std::cout << "Gracz " << this << " rozpoczal nowe okrazenie" << std::endl;
			}
		}
	}
}

void Vehicle::draw(sf::RenderTarget& window)
{
	constexpr float RADIUS = 8.0f;

	sf::CircleShape shape(RADIUS);
	shape.setOrigin(RADIUS, RADIUS);
	shape.setFillColor(sf::Color::Red);
	shape.setPosition(m_position);
	window.draw(shape);
}