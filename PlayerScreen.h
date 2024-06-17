#pragma once

#include <SFML/Graphics.hpp>

class PlayerScreen
{
public:
	PlayerScreen(int number);

	void draw(sf::RenderTexture& source, sf::RenderTarget& target, sf::Vector2f center, float angle, float dt);

private:
	void calculateSizeAndViewport();

	int m_number;
	sf::Vector2f m_size;
	sf::FloatRect m_viewport {0.f, 0.f, 1.f, 1.f};
};

