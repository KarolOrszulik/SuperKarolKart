#include "Kart.h"

#include <iostream>

void Kart::handleMovement(float dt)
{
	Track::Tile tile = m_track->atPos(m_position);

	float maxSpeed = tile == Track::Tile::GRASS ? 60.f : 120.f;
	const float deceleration = tile == Track::Tile::GRASS ? 120.f : 90.f;
	maxSpeed *= m_speedMultiplier;

	constexpr float WHEELBASE = 12.f;
	constexpr float MAX_FRICTION = 350.f;

	if (m_speed >= maxSpeed || m_speed <= -maxSpeed * 0.4f || std::abs(m_acceleratorInput) < 0.01f)
	{
		if (std::abs(m_speed) > 1.f) // only decelerate if speed is not negligible
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
	//if (tile == Track::Tile::GRASS)
		// m_speed = std::clamp(m_speed, -m_maxSpeed / m_speedMultiplier, m_maxSpeed / m_speedMultiplier);

	constexpr float LOCK_ANGLE = 30.f * 3.14f / 180.f;
	//if (std::abs(m_steeringInput) < 0.01f || m_wheelsAngle >= LOCK_ANGLE || m_wheelsAngle <= -LOCK_ANGLE) // no steering input, return to center gradually
	//{
	//	m_wheelsAngle = std::lerp(m_wheelsAngle, 0.f, 1.f - std::exp(-10.f * dt));
	//}
	//else // steer slowly, much like with throttle
	//{
	//	m_wheelsAngle = std::lerp(m_wheelsAngle, m_steeringInput * LOCK_ANGLE, 1.f - std::exp(-5.f * dt));
	//} // olaæ to, proœciej jest mo¿e mniej realistycznie, ale lepiej siê jeŸdzi
	float wheelsAngle = m_steeringInput * LOCK_ANGLE;

	const float max_angle = std::atan(WHEELBASE * MAX_FRICTION / (m_speed * m_speed));
	wheelsAngle = std::clamp(wheelsAngle, -max_angle, max_angle);

	sf::Vector2f rWheel = m_position - getUnitVector() * (WHEELBASE / 2.f);
	sf::Vector2f fWheel = m_position + getUnitVector() * (WHEELBASE / 2.f);

	rWheel += getUnitVector() * m_speed * dt;
	float fWheelRotation = m_angle + wheelsAngle;
	fWheel += sf::Vector2f(std::cos(fWheelRotation), std::sin(fWheelRotation)) * m_speed * dt;

	sf::Vector2f newDirection = fWheel - rWheel;
	m_angle = std::atan2(newDirection.y, newDirection.x);

	m_position = rWheel + newDirection / 2.f;

	m_angle = std::fmod(m_angle, 2 * 3.14f);
}