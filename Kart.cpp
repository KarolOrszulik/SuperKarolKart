#include "Kart.h"

#include <iostream>

void Kart::handleMovement(float dt)
{
	constexpr float WHEELBASE = 12.f;
	constexpr float MAX_FRICTION = 350.f;
	constexpr float LOCK_ANGLE = 30.f * 3.14f / 180.f; // max steering angle in radians

	Track::Tile tile = m_track->atPos(m_position);

	const float deceleration = tile == Track::Tile::GRASS ? 120.f : 90.f;
	float maxSpeed = tile == Track::Tile::GRASS ? 60.f : 120.f;
	maxSpeed *= m_speedMultiplier;


	if (m_speed >= maxSpeed || m_speed <= -maxSpeed * 0.4f || std::abs(m_acceleratorInput) < 0.01f)
	{
		if (std::abs(m_speed) > 1.f)
		{
			float dir = m_speed > 0.f ? 1.f : -1.f;
			m_acceleration = -dir * deceleration;
		}
		else
		{
			m_speed = 0.f;
		}
	}
	else
	{
		if (m_acceleratorInput * m_speed < 0.f) // braking
		{
			m_acceleration = 150.f * m_acceleratorInput * m_speedMultiplier;
		}
		else // accelerating
		{
			m_acceleration = 80.f * m_acceleratorInput * m_speedMultiplier;
		}
	}

	m_speed += m_acceleration * dt;
	

	const float max_angle = std::atan(WHEELBASE * MAX_FRICTION / (m_speed * m_speed));

	float wheelsAngle = std::clamp(m_steeringInput * LOCK_ANGLE, -max_angle, max_angle);

	sf::Vector2f frontWheelPos = m_position + getUnitVector() * (WHEELBASE / 2.f);
	sf::Vector2f rearWheelPos  = m_position - getUnitVector() * (WHEELBASE / 2.f);

	rearWheelPos += getUnitVector() * m_speed * dt;
	const float fWheelRotation = m_angle + wheelsAngle;
	frontWheelPos += sf::Vector2f(std::cos(fWheelRotation), std::sin(fWheelRotation)) * m_speed * dt;

	const sf::Vector2f newDirection = frontWheelPos - rearWheelPos;
	m_angle = std::atan2(newDirection.y, newDirection.x);

	m_position = rearWheelPos + newDirection / 2.f;
	m_angle = std::fmod(m_angle, 2 * 3.14f);
}