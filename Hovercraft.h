#pragma once

#include "Vehicle.h"

class Hovercraft :  public Vehicle
{
public:
	using Vehicle::Vehicle;

private:
	std::filesystem::path getTexturePath() const override { return "assets/vehicles/hovercraft.png"; }

	void handleMovement(float dt) override;

	virtual float getSpeed() const override;

	float m_angularVelocity = 0.f;
	sf::Vector2f m_velocity;
};

