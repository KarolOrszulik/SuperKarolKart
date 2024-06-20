#pragma once

#include "GroundVehicle.h"

class Kart : public GroundVehicle
{
public:
	using GroundVehicle::GroundVehicle;

private:
	void handleMovement(float dt) override;

	GroundVehicle::DrivingParameters getDrivingParameters() const override
	{
		return GroundVehicle::DrivingParameters{
			.wheelbase = 12.f,
			.maxFriction = 350.f,
			.lockAngle = 30.f * 3.14f / 180.f,
			.maxSpeedRoad = 150.f,
			.maxSpeedGrass = 60.f,
			.maxReverseSpeedRatio = 0.7f,
			.decelerationRoad = 90.f,
			.decelerationGrass = 120.f,
			.acceleration = 75.f,
			.braking = 170.f
			};
	}
};

