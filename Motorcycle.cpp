#include "Motorcycle.h"

void Motorcycle::handleMovement(float dt)
{
	if (m_input.skill)
	{
		setSpeedMultiplier(2.f, 0.1f);
		setSteeringMultiplier(0.f, 0.1f);
	}

	GroundVehicle::handleMovement(dt);
}