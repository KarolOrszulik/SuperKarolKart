#include "Vehicle.h"
#include "Engine.h"
#include "SpeedAdjuster.h"

#include <iostream>

Vehicle::Vehicle(
	const sf::Texture& texture, 
	Track* track, 
	sf::Vector2f position)
	: GameObject(position), m_track(track)
{
	assignTexture(texture);
}

void Vehicle::setSpeedMultiplier(float speedMultiplier, float time)
{
	m_speedMultiplier = speedMultiplier;
	m_timeToClearMultiplier = time;
}

void Vehicle::setSteeringMultiplier(float steeringMultiplier, float time)
{
	m_steeringMultiplier = steeringMultiplier;
	m_timeToClearSteeringMultiplier = time;
}

void Vehicle::handleGroundItems()
{
	for (auto& obj : Engine::getInstance()->getObjects())
	{
		if (GroundItem* gi = dynamic_cast<GroundItem*>(obj.get()))
		{
			sf::Vector2f gipos = gi->getPosition();
			const float dx = m_position.x - gipos.x;
			const float dy = m_position.y - gipos.y;
			const float distanceSq = dx * dx + dy * dy;

			const float gridsize = Engine::getInstance()->getGridSizeF();
			if (distanceSq <= gridsize * gridsize)
			{
				gi->interactWithVehicle(*this);
			}
		}
	}
}

void Vehicle::handleCheckpoints()
{
	std::optional<int> checkpointIndex = m_track->getCheckpointIndex(m_position, Engine::getInstance()->getGridSizeF());
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

void Vehicle::handleClearMultiplier(float dt)
{
	if (m_timeToClearMultiplier > 0.0f)
	{
		m_timeToClearMultiplier -= dt;

		if (m_timeToClearMultiplier <= 0.0f)
			m_speedMultiplier = 1.0f;
	}
	if (m_timeToClearSteeringMultiplier > 0.0f)
	{
		m_timeToClearSteeringMultiplier -= dt;

		if (m_timeToClearSteeringMultiplier <= 0.0f)
			m_steeringMultiplier = 1.0f;
	}
}

void Vehicle::handleItemUse()
{
	if (m_input.use)
	{
		m_input.use = false;
		if (m_powerUp) {
			m_powerUp->use(*this);
			if(!m_powerUp->canBeUsed())
				m_powerUp = nullptr;
		}
	}
}



void Vehicle::update(float dt)
{
	handleGroundItems();
	handleClearMultiplier(dt);

	m_input.steering *= m_steeringMultiplier;
	handleMovement(dt); // this part is abstract

	handleItemUse();
	handleCheckpoints();
}

void Vehicle::draw(sf::RenderTarget& window)
{
	const unsigned gridSize  = m_track->getGridSize();
	const float    gridSizeF = m_track->getGridSizeF();

	sf::Sprite sprite(getTexture());
	sprite.setPosition(m_position);
	sprite.setOrigin(gridSizeF / 2.f, gridSizeF / 2.f);
	sprite.setRotation(m_angle * 180.f / 3.14f + 90.f);
	window.draw(sprite);
}