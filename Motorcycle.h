#pragma once

#include "Vehicle.h"

class Motorcycle :  public Vehicle
{
public:
	using Vehicle::Vehicle;

	void draw(sf::RenderTarget&) override;

private:
	void handleMovement(float dt) override;
};

