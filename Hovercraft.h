#pragma once

#include "Vehicle.h"

class Hovercraft :  public Vehicle
{
public:
	using Vehicle::Vehicle;

	float getMaxSpeed() const override { return m_MAX_SPEED; }

private:
	constexpr static float m_MAX_SPEED = 500.f; // speed at which the hovercraft stops accelerating
	constexpr static float m_MAX_ANGULAR_VELOCITY = 150.f / 180.f * 3.1415f;
	void handleMovement(float dt) override;

	virtual float getSpeed() const override;

	float m_angularVelocity = 0.f;
	sf::Vector2f m_velocity;
};

