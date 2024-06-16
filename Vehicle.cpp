#include "Vehicle.h"
#include "Engine.h"
#include "GroundItem.h"

#include <iostream>

void Vehicle::applyAccelerator(float accelerator)
{
	m_acceleration += accelerator;
}

void Vehicle::applySteering(float steering)
{
	m_steering += steering;
}

void Vehicle::setSpeedMultiplier(float speedMultiplier, float time)
{
	m_speedMultiplier = speedMultiplier;
	m_timeToClearMultiplier = time;
}

void Vehicle::update(float dt)
{
	for (auto& obj : Engine::getInstance()->getObjects())
	{
		if (GroundItem* gi = dynamic_cast<GroundItem*>(obj.get())) // :(
		{
			sf::Vector2f gipos = gi->getPosition();
			float dx = m_position.x - gipos.x;
			float dy = m_position.y - gipos.y;
			float distancesq = dx * dx + dy * dy;

			if (distancesq <= 16.f*16.f)
			{
				gi->interactWithVehicle(*this);
			}
		}
	}

	if (m_timeToClearMultiplier > 0.0f)
	{
		m_timeToClearMultiplier -= dt;
		if (m_timeToClearMultiplier <= 0.0f)
		{
			m_speedMultiplier = 1.0f;
		}
	}

	Track::Tile tile = m_track->atPos(m_position);
	
	float SPEED = tile == Track::Tile::GRASS ? 60.f : 120.f;
	SPEED *= m_speedMultiplier;

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
				++m_completedLaps;
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