#include "Hovercraft.h"

float Hovercraft::getSpeed() const
{
	return std::sqrt(m_velocity.x * m_velocity.x + m_velocity.y * m_velocity.y);
}

void Hovercraft::handleMovement(float dt)
{
	// handle bouncing off edges of track
	const float margin = 1.2f * m_track->getGridSizeF();
	if (m_position.x <= margin || m_position.x >= m_track->getSize().x - margin)
	{
		m_position -= m_velocity * dt;
		m_velocity.x = -m_velocity.x;
		m_velocity *= 0.7f;
	}
	if (m_position.y <= margin || m_position.y >= m_track->getSize().y - margin)
	{
		m_position -= m_velocity * dt;
		m_velocity.y = -m_velocity.y;
		m_velocity *= 0.7f;
	}

	sf::Vector2f force;

	if (std::abs(m_input.accelerator) < 0.01f)
	{
		m_velocity.x = std::lerp(m_velocity.x, 0.f, 1 - std::exp(-3.f * dt));
		m_velocity.y = std::lerp(m_velocity.y, 0.f, 1 - std::exp(-3.f * dt));
	}
	else
	{
		float speed = std::sqrt(m_velocity.x * m_velocity.x + m_velocity.y * m_velocity.y);
		float acceleration = 180.f * m_input.accelerator * (1.f - speed / m_MAX_SPEED);
		m_velocity += getUnitVector() * acceleration * dt;
	}

	m_position += m_velocity * dt;


	if (std::abs(m_input.steering) < 0.01f)
	{
		m_angularVelocity = std::lerp(m_angularVelocity, 0.f, 1 - std::exp(-7.f * dt));
	}
	else
	{
		m_angularVelocity = m_input.steering * m_MAX_ANGULAR_VELOCITY; // don't bother with lerping or smoothing
		m_angularVelocity = std::clamp(m_angularVelocity, -m_MAX_ANGULAR_VELOCITY, m_MAX_ANGULAR_VELOCITY);
	}

	m_angle += m_angularVelocity * dt;
}