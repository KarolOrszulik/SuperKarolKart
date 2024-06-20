#pragma once

#include <SFML/Graphics.hpp>
#include "UIButton.h"
#include "UIImage.h"
#include "Vehicle.h"
#include "AveragingCounter.h"

class Player;

class PlayerScreen
{
public:
	PlayerScreen(int number);

	void draw(sf::RenderTexture& source, sf::RenderTarget& target, const Player& player, float dt);

private:
	static constexpr float ZOOM_MIN = 0.3f;
	static constexpr float ZOOM_MAX = 0.5f;
	static constexpr float ZOOM_SPEED_START = 50.f;
	static constexpr float ZOOM_SPEED_END = 250.f;
	// zoom stays at ZOOM_MIN when speed is below ZOOM_SPEED_START
	// zoom grows linearly from ZOOM_MIN to ZOOM_MAX when speed is between ZOOM_SPEED_START and ZOOM_SPEED_END
	// zoom stays at ZOOM_MAX when speed is above ZOOM_SPEED_END

	enum class Corner
	{
		TOP_LEFT,
		TOP_RIGHT,
		BOT_LEFT,
		BOT_RIGHT
	};

	void calculateSizeAndViewport();

	UIButton generateSpeedDisplay(float speed, float vh, float dt);
	UIButton generateLapCounter(const Player& player, float vh);
	UIImage generateItemImage(const Vehicle& vehicle, float vh);

	sf::Vector2f getCornerPosition(Corner corner, sf::Vector2f targetSize) const;

	int m_number;
	sf::Vector2f m_size;
	sf::FloatRect m_viewport {0.f, 0.f, 1.f, 1.f};
	float m_screenAngle = 0.f;
	AveragingCounter m_speedCounter;
	float m_zoom = ZOOM_MIN;
};