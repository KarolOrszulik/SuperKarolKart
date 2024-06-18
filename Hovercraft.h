#pragma once

#include "Vehicle.h"

class Hovercraft :  public Vehicle
{
public:
	using Vehicle::Vehicle;

private:
	void handleMovement(float dt) override;
	int getTextureOffset() override { return 2; }

	virtual float getSpeed() const override;

	float m_angularVelocity = 0.f;
	sf::Vector2f m_velocity;
};

