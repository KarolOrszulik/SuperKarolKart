#include "Vehicle.h"
#include "Engine.h"
#include "SpeedAdjuster.h"

#include <iostream>

Vehicle::Vehicle(Track* track, sf::Vector2f position)
	: GameObject(position) , m_track(track)
{
	m_texture.loadFromFile("assets/vehicle_tileset.png");
}

// ZAST¥PIONE APPLYINPUT W PLIKU H

//void Vehicle::applyAccelerator(float accelerator)
//{
//	m_acceleratorInput += accelerator;
//}
//
//void Vehicle::applySteering(float steering)
//{
//	m_steeringInput += steering;
//}
//
//void Vehicle::applyUse()
//{
//	m_use = true;
//}
//
//void Vehicle::applySkill()
//{
//	m_skill = true;
//}


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
	sf::Sprite sprite(m_texture);
	sprite.setPosition(m_position);
	sprite.setOrigin(8.f, 8.f);
	sprite.setRotation(m_angle * 180.f / 3.14f + 90.f);
	sprite.setTextureRect(sf::IntRect(getTextureOffset() * 16, 0, 16, 16));
	window.draw(sprite);
}