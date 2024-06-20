#include "PowerUp.h"

float PowerUp::calculateLaunchSpeed(float vehicleSpeed, float launchSpeed, float minSpeed)
{
	return std::max(vehicleSpeed + launchSpeed, minSpeed);
}

void PowerUp::use(Vehicle& veh)
{
	m_usageLeft--;
	usePowerUp(veh);
}