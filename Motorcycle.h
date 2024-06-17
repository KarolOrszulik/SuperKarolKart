#pragma once

#include "Vehicle.h"

class Motorcycle :  public Vehicle
{
public:
	using Vehicle::Vehicle;

private:
	void handleMovement(float dt) override;
	int getTextureOffset() override { return 1; }
};

