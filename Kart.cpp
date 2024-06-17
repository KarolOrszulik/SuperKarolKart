#include "Kart.h"

#include <iostream>

void Kart::handleMovement(float dt)
{
	Track::Tile tile = m_track->atPos(m_position);

	m_maxSpeed = tile == Track::Tile::GRASS ? 60.f : 120.f;
	m_maxSpeed *= m_speedMultiplier;

	// calculate speed
	if (m_acceleratorInput > 0.1f && m_speed < m_maxSpeed) // forward
	{
		m_acceleration = 60.f;

		m_speed += m_acceleration * dt;
	}
	else if (m_acceleratorInput < -0.1f && m_speed > 0.5f * m_maxSpeed) // reverse
	{
		m_acceleration = -120.f;
	} 
	else // coast
	{
		m_acceleration = 30.f;
		m_acceleration *= m_speed > 0 ? -1.f : 1.f;
	}
	m_acceleration *= m_speedMultiplier;
	m_speed += m_acceleration * dt;

	// calculate steering
	const float maxSteeringAngle = std::exp(-m_speed / 100.f);
	m_wheelsAngle += m_steeringInput * dt * 2.f;
	m_wheelsAngle = std::clamp(m_wheelsAngle, -maxSteeringAngle, maxSteeringAngle);

	if (std::abs(m_steeringInput) < 0.1f)
	{
		m_wheelsAngle -= 1.5f * dt * (m_wheelsAngle > 0 ? 1.f : -1.f);
	}

	m_angle += m_wheelsAngle * m_speed * dt * 0.06f;

	m_angle = std::fmod(m_angle, 2 * 3.14f);

	m_position += getUnitVector() * m_speed * dt;

	m_acceleratorInput = 0.0f;
	m_steeringInput = 0.0f;
}