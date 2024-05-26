#pragma once

#include <SFML/Graphics.hpp>

#include "Vehicle.h"
#include "PlayerScreen.h"


class Player
{
public:
	enum class ControlScheme;

	Player(ControlScheme control, int player_number);

	void setVehicle(Vehicle* vehicle) { m_vehicle = vehicle; }
	void controlVehicle();

	void drawPlayerScreen(sf::RenderTexture& source, sf::RenderTarget& target);

private:
	Vehicle* m_vehicle = nullptr;
	ControlScheme m_control;
	PlayerScreen m_screen;
};

enum class Player::ControlScheme
{
	WASD,
	ARROWS,
	IJKL,
};
