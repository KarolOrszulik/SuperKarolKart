#pragma once
#include "PowerUp.h"

class OilSpill : public PowerUp
{
public:
	using PowerUp::PowerUp;
private:
	void usePowerUp(Vehicle& owner) override;
};

