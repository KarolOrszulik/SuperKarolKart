#include "Motorcycle.h"

void Motorcycle::handleMovement(float dt)
{
	if (m_input.skill && m_speedMultiplier >= 0.99f)
	{
		setSpeedMultiplier(SKILL_SPEED_MULTIPLIER, 0.1f);
		setSteeringMultiplier(0.f, 0.1f);
	}

	GroundVehicle::handleMovement(dt);
}