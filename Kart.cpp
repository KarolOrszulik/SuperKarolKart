#include "Kart.h"

void Kart::handleMovement(float dt)
{
	Track::Tile tile = m_track->atPos(m_position);

	float speed = tile == Track::Tile::GRASS ? 60.f : 120.f;
	speed *= m_speedMultiplier;

	m_angle += m_steering * dt * 3.14f;
	m_angle = std::fmod(m_angle, 2 * 3.14f);

	if (m_acceleration > 0.0f)
		m_position += sf::Vector2f(std::cos(m_angle), std::sin(m_angle)) * speed * dt;

	m_acceleration = 0.0f;
	m_steering = 0.0f;
}

void Kart::draw(sf::RenderTarget& target)
{
	constexpr float LENGTH = 16.f;
	constexpr float WIDTH = 8.f;

	sf::ConvexShape triangle;
	triangle.setPointCount(3);
	triangle.setPoint(0, { LENGTH * .75f, 0.f });
	triangle.setPoint(1, { -LENGTH * .25f, -WIDTH / 2.f });
	triangle.setPoint(2, { -LENGTH * .25f,  WIDTH / 2.f });
	triangle.setFillColor(sf::Color::Red);
	triangle.setOutlineColor(sf::Color::Black);
	triangle.setOutlineThickness(1.0f);

	triangle.setPosition(m_position);
	triangle.setRotation(m_angle * 180.0f / 3.14f);

	target.draw(triangle);
}