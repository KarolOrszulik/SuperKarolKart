#include "Vehicle.h"
#include "Engine.h"
#include "SpeedAdjuster.h"

#include <iostream>

void Vehicle::applyAccelerator(float accelerator)
{
	m_acceleration += accelerator;
}

void Vehicle::applySteering(float steering)
{
	m_steering += steering;
}

void Vehicle::applyUse()
{
	m_use = true;
}

void Vehicle::applySkill()
{
	m_skill = true;
}

void Vehicle::setSpeedMultiplier(float speedMultiplier, float time)
{
	m_speedMultiplier = speedMultiplier;
	m_timeToClearMultiplier = time;
}

void Vehicle::handleGroundItems()
{
	for (auto& obj : Engine::getInstance()->getObjects())
	{
		if (GroundItem* gi = dynamic_cast<GroundItem*>(obj.get()))
		{
			sf::Vector2f gipos = gi->getPosition();
			float dx = m_position.x - gipos.x;
			float dy = m_position.y - gipos.y;
			float distancesq = dx * dx + dy * dy;

			if (distancesq <= 16.f * 16.f) // pls refactor me
			{
				gi->interactWithVehicle(*this);
			}
		}
	}
}

void Vehicle::handleCheckpoints()
{
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

void Vehicle::handleClearMultiplier(float dt)
{
	if (m_timeToClearMultiplier > 0.0f)
	{
		m_timeToClearMultiplier -= dt;

		if (m_timeToClearMultiplier <= 0.0f)
			m_speedMultiplier = 1.0f;
	}
}

void Vehicle::handleItemUse()
{
	if (m_use)
	{
		m_use = false;
		// na razie na sztywno "kula do krêgli"
		std::unique_ptr<SpeedAdjuster> ball = std::make_unique<SpeedAdjuster>(0.2f, sf::Vector2f(std::cos(m_angle), std::sin(m_angle)) * 150.f);
		ball->setPosition(m_position + sf::Vector2f(std::cos(m_angle), std::sin(m_angle)) * 20.f);
		Engine::getInstance()->addObject(std::move(ball));
	}
}

void Vehicle::update(float dt)
{
	handleGroundItems();
	handleClearMultiplier(dt);

	handleMovement(dt); // this part is abstract

	handleItemUse();
	handleCheckpoints();
}