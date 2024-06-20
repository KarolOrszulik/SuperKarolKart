#pragma once
#include "PowerUp.h"

class BowlingBall : public PowerUp
{
public: 
	using PowerUp::PowerUp;
private:
	void usePowerUp(Vehicle& owner) override;
};

