#pragma once

#include "Vehicle.h"

class Kart : public Vehicle
{
public:
	using Vehicle::Vehicle;

	void draw(sf::RenderTarget&) override;

private:
	void handleMovement(float dt) override;
};

