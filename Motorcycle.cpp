#include "Motorcycle.h"

void Motorcycle::handleMovement(float dt)
{
	if (m_input.skill && m_speedMultiplier >= 0.99f)
	{
		setSpeedMultiplier(SKILL_SPEED_MULTIPLIER, SKILL_STEERING_MULTIPLIER_TIME);
		setSteeringMultiplier(0.f, 0.1f);
	}

	GroundVehicle::handleMovement(dt);
}