#pragma once

#include <SFML/Graphics.hpp>
#include "UIButton.h"

class Player;

class PlayerScreen
{
public:
	PlayerScreen(int number);

	void draw(sf::RenderTexture& source, sf::RenderTarget& target, const Player& player, float dt);

private:
	void calculateSizeAndViewport();
	sf::Vector2f calculateSpeedPos();

	int m_number;
	sf::Vector2f m_size;
	sf::FloatRect m_viewport {0.f, 0.f, 1.f, 1.f};
	UIButton::Style m_speedStyle;
};