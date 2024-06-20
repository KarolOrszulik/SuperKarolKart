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

	void drawPlayerScreen(sf::RenderTexture& source, sf::RenderTarget& target, float dt);

	size_t getCompletedLaps() const { return m_vehicle->getCompletedLaps(); }

	void setFinishTime(float time) { m_finishTime = time; }
	float getFinishTime() const { return m_finishTime; }

private:
	Vehicle* m_vehicle = nullptr;
	ControlScheme m_control;
	PlayerScreen m_screen;

	float m_finishTime = 0.f;

	friend class PlayerScreen;
};

enum class Player::ControlScheme
{
	WASD,
	ARROWS,
	IJKL,
	NUMBERS
};
