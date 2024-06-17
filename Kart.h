#pragma once

#include "Vehicle.h"

class Kart : public Vehicle
{
public:
	using Vehicle::Vehicle;

private:
	void handleMovement(float dt) override;
	int getTextureOffset() override { return 0; }

	float m_acceleration = 0.f;
	float m_speed = 0.f;
};

