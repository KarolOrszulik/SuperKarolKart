#include "GroundVehicle.h"

void GroundVehicle::handleMovement(float dt)
{
	const float margin = 1.2f * m_track->getGridSizeF();
	if (m_position.x <= margin || m_position.x >= m_track->getSize().x - margin)
	{
		// reflect m_angle around x-axis
		m_angle = 3.14f - m_angle;
		m_position.x = std::clamp(m_position.x, margin, m_track->getSize().x - margin);
		m_speed *= 0.7f;
		m_speed = std::min(m_speed, 40.0f);
	}
	if (m_position.y <= margin || m_position.y >= m_track->getSize().y - margin)
	{
		// reflect m_angle around y-axis
		m_angle = -m_angle;
		m_position.y = std::clamp(m_position.y, margin, m_track->getSize().y - margin);
		m_speed *= 0.7f;
		m_speed = std::min(m_speed, 40.0f);
	}


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

	const bool withinSpeedLimit = m_speed < maxSpeed && m_speed > -maxSpeed * p.maxReverseSpeedRatio;
	const bool coasting = std::abs(m_input.accelerator) < 0.01f;
	const bool braking = m_speed * m_input.accelerator < 0.f;
	const bool accelerating = m_speed * m_input.accelerator > 0.f;

	if (braking)
	{
		// braking strength +50% at max road speed, and no more than +50%
		const float brakingFactor = std::min(1.5f, 1.0f + 0.5f * std::abs(m_speed / p.maxSpeedRoad));
		m_acceleration = p.braking * m_input.accelerator * brakingFactor;
	}
	else if (accelerating && withinSpeedLimit)
	{
		m_acceleration = p.acceleration * m_input.accelerator * m_speedMultiplier;
	}
	else if (coasting || !withinSpeedLimit)
	{
		const float direction = m_speed > 0.f ? 1.f : -1.f;
		m_acceleration = -direction * deceleration;
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