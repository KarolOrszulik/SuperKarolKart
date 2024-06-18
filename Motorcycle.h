#pragma once

#include "GroundVehicle.h"

class Motorcycle :  public GroundVehicle
{
public:
	using GroundVehicle::GroundVehicle;

private:
	void handleMovement(float dt) override;
	int getTextureOffset() override { return 1; }

	GroundVehicle::DrivingParameters getDrivingParameters() override
	{
		return GroundVehicle::DrivingParameters{
			.wheelbase = 10.f,
			.maxFriction = 200.f,
			.lockAngle = 40.f * 3.14f / 180.f,
			.maxSpeedRoad = 150.f,
			.maxSpeedGrass = 60.f,
			.maxReverseSpeedRatio = 0.3f,
			.decelerationRoad = 100.f,
			.decelerationGrass = 150.f,
			.acceleration = 100.f,
			.braking = 200.f
		};
	}
};

