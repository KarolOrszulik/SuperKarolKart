#include "Kart.h"

void Kart::handleMovement(float dt)
{
	Track::Tile tile = m_track->atPos(m_position);

	m_maxSpeed = tile == Track::Tile::GRASS ? 60.f : 120.f;

	m_angle += m_steeringInput * dt * 3.14f * m_steeringMultiplier;
	m_angle = std::fmod(m_angle, 2 * 3.14f);

	if (m_acceleratorInput > 0.0f)
		m_position += sf::Vector2f(std::cos(m_angle), std::sin(m_angle)) * dt * m_maxSpeed; // to nie dzia³a na razie, naprawie po mergu

	m_acceleratorInput = 0.0f;
	m_steeringInput = 0.0f;
}