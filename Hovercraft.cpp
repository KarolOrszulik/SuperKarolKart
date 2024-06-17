#include "Hovercraft.h"

void Hovercraft::handleMovement(float dt)
{
	constexpr float MAX_SPEED = 500.f; // speed at which the hovercraft stops accelerating
	constexpr float MAX_ANGULAR_VELOCITY = 150.f / 180.f * 3.1415f;

	sf::Vector2f force;

	if (std::abs(m_acceleratorInput) < 0.01f)
	{
		m_velocity.x = std::lerp(m_velocity.x, 0.f, 1 - std::exp(-3.f * dt));
		m_velocity.y = std::lerp(m_velocity.y, 0.f, 1 - std::exp(-3.f * dt));
	}
	else
	{
		float speed = std::sqrt(m_velocity.x * m_velocity.x + m_velocity.y * m_velocity.y);
		float acceleration = 180.f * m_acceleratorInput * (1.f - speed / MAX_SPEED);
		m_velocity += getUnitVector() * acceleration * dt;
	}

	m_position += m_velocity * dt;


	if (std::abs(m_steeringInput) < 0.01f)
	{
		m_angularVelocity = std::lerp(m_angularVelocity, 0.f, 1 - std::exp(-7.f * dt));
	}
	else
	{
		m_angularVelocity = m_steeringInput * MAX_ANGULAR_VELOCITY; // don't bother
		m_angularVelocity = std::clamp(m_angularVelocity, -MAX_ANGULAR_VELOCITY, MAX_ANGULAR_VELOCITY);
	}

	m_angle += m_angularVelocity * dt;
}