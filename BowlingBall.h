#pragma once
#include "PowerUp.h"

class BowlingBall : public PowerUp
{
private:
	void usePowerUp(Vehicle& owner) override;
};

