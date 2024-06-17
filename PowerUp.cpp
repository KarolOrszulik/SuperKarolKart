#include "PowerUp.h"

void PowerUp::use(Vehicle& veh)
{
	m_usageLeft--;
	usePowerUp(veh);
}