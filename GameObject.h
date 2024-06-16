#pragma once

#include <SFML/Graphics.hpp>

class GameObject
{
public:
	GameObject (sf::Vector2f position = { 0.0f, 0.0f }) : m_position(position) {}

	virtual void update(float dt) = 0;
	virtual void draw(sf::RenderTarget& window) = 0;

	sf::Vector2f getPosition() const { return m_position; }

protected:
	sf::Vector2f m_position;
};

