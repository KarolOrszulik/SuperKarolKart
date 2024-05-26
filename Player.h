#pragma once

#include <SFML/Graphics.hpp>

#include "Vehicle.h"


class Player
{
public:
	enum class ControlScheme;

	Player(ControlScheme control, int player_number, int total_players, sf::Vector2u window_size);

	void setVehicle(Vehicle* vehicle) { m_vehicle = vehicle; }
	void controlVehicle();

private:
	Vehicle* m_vehicle = nullptr;
	ControlScheme m_control;
};

enum class Player::ControlScheme
{
	WASD,
	ARROWS
};
