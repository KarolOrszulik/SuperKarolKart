#include "Kart.h"

#include <iostream>

void Kart::handleMovement(float dt)
{
	Track::Tile tile = m_track->atPos(m_position);

	m_maxSpeed = tile == Track::Tile::GRASS ? 80.f : 120.f;
	m_maxSpeed *= m_speedMultiplier;

	constexpr float WHEELBASE = 12.f;
	constexpr float MAX_FRICTION = 250.f;

	if (std::abs(m_acceleratorInput) < 0.01f || m_speed >= m_maxSpeed || m_speed <= -m_maxSpeed * 0.4f) // no input or above max speed, coast
	{
		if (std::abs(m_speed) > 0.01f)
			m_acceleration = std::abs(m_speed) / m_speed * -40.f;
	}
	else
	{
		if (m_acceleratorInput * m_speed < 0.f) // braking
		{
			m_acceleration = 120.f * m_acceleratorInput * m_speedMultiplier;
		}
		else // accelerating
		{
			m_acceleration = 80.f * m_acceleratorInput * m_speedMultiplier;
		}
	}

	m_speed += m_acceleration * dt;
	if (tile == Track::Tile::GRASS)
		m_speed = std::clamp(m_speed, -m_maxSpeed / m_speedMultiplier, m_maxSpeed / m_speedMultiplier);

	constexpr float LOCK_ANGLE = 30.f * 3.14f / 180.f;
	if (std::abs(m_steeringInput) < 0.01f || m_wheelsAngle >= LOCK_ANGLE || m_wheelsAngle <= -LOCK_ANGLE) // no steering input, return to center gradually
	{
		m_wheelsAngle = std::lerp(m_wheelsAngle, 0.f, 1.f - std::exp(-10.f * dt));
	}
	else // steer slowly, much like with throttle
	{
		m_wheelsAngle = std::lerp(m_wheelsAngle, m_steeringInput * LOCK_ANGLE, 1.f - std::exp(-5.f * dt));
	}

	const float max_angle = std::atan(WHEELBASE * MAX_FRICTION / (m_speed * m_speed));
	m_wheelsAngle = std::clamp(m_wheelsAngle, -max_angle, max_angle);

	sf::Vector2f rWheel = m_position - getUnitVector() * (WHEELBASE / 2.f);
	sf::Vector2f fWheel = m_position + getUnitVector() * (WHEELBASE / 2.f);

	rWheel += getUnitVector() * m_speed * dt;
	float fWheelRotation = m_angle + m_wheelsAngle;
	fWheel += sf::Vector2f(std::cos(fWheelRotation), std::sin(fWheelRotation)) * m_speed * dt;

	sf::Vector2f newDirection = fWheel - rWheel;
	m_angle = std::atan2(newDirection.y, newDirection.x);

	m_position = rWheel + newDirection / 2.f;

	//m_angle = std::fmod(m_angle, 2 * 3.14f);

	m_acceleratorInput = 0.0f;
	m_steeringInput = 0.0f;
}