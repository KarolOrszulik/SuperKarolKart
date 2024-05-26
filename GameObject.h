#pragma once

#include <SFML/Graphics.hpp>

class GameObject
{
public:
	virtual void update(float dt) = 0;
	virtual void draw(sf::RenderTarget& window) = 0;

protected:
	sf::Vector2f m_position;
};

