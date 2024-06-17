#pragma once
#include "PowerUp.h"

class OilSpill : public PowerUp
{
private:
	void usePowerUp(Vehicle& owner) override;
};

