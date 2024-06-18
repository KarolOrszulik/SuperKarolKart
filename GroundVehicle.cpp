#include "GroundVehicle.h"

void GroundVehicle::handleMovement(float dt)
{
	const DrivingParameters p = getDrivingParameters();

	Track::Tile tile = m_track->atPos(m_position);

	const float deceleration =
		tile == Track::Tile::GRASS
		? p.decelerationGrass
		: p.decelerationRoad;

	const float maxSpeed = m_speedMultiplier * (
		tile == Track::Tile::GRASS
		? p.maxSpeedGrass
		: p.maxSpeedRoad);


	if (std::abs(m_input.accelerator) < 0.01f // no input or
		|| m_speed >= maxSpeed				 // max speed in either direction
		|| m_speed <= -maxSpeed * p.maxReverseSpeedRatio)
	{
		if (std::abs(m_speed) > 0.1f)
		{
			float dir = m_speed > 0.f ? 1.f : -1.f;
			m_acceleration = -dir * deceleration;
		}
		else
		{
			m_speed = 0.f;
		}
	}
	else // valid input
	{
		if (m_input.accelerator * m_speed > 0.f) // accelerating
		{
			m_acceleration =
				p.acceleration * m_input.accelerator * m_speedMultiplier;
		}
		else // braking
		{
			m_acceleration =
				p.braking * m_input.accelerator * m_speedMultiplier;
		}
	}

	m_speed += m_acceleration * dt;


	const float max_angle = std::atan(p.wheelbase * p.maxFriction / (m_speed * m_speed));

	const float wheelAngle = std::clamp(m_input.steering * p.lockAngle, -max_angle, max_angle);

	sf::Vector2f frontWheelPos = m_position + getUnitVector() * (p.wheelbase / 2.f);
	sf::Vector2f rearWheelPos =  m_position - getUnitVector() * (p.wheelbase / 2.f);

	const float frontWheelAngle = m_angle + wheelAngle;
	frontWheelPos += sf::Vector2f(std::cos(frontWheelAngle), std::sin(frontWheelAngle)) * m_speed * dt;
	rearWheelPos += getUnitVector() * m_speed * dt;

	const sf::Vector2f newDirection = frontWheelPos - rearWheelPos;
	m_angle = std::atan2(newDirection.y, newDirection.x);
	m_angle = std::fmod(m_angle, 2 * 3.14f);

	m_position = rearWheelPos + newDirection / 2.f;
}