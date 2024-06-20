#pragma once
#include "PowerUp.h"
class Arrows : public PowerUp
{
public:
	using PowerUp::PowerUp;
private:
	void usePowerUp(Vehicle& owner) override;
};

