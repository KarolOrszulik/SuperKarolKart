#pragma once

#include "GroundVehicle.h"

class Kart : public GroundVehicle
{
public:
	using GroundVehicle::GroundVehicle;

private:
	void handleMovement(float dt) override;
	int getTextureOffset() override { return 0; }

	GroundVehicle::DrivingParameters getDrivingParameters() override
	{
		return GroundVehicle::DrivingParameters{
			.wheelbase = 12.f,
			.maxFriction = 300.f,
			.lockAngle = 30.f * 3.14f / 180.f,
			.maxSpeedRoad = 120.f,
			.maxSpeedGrass = 60.f,
			.maxReverseSpeedRatio = 0.7f,
			.decelerationRoad = 90.f,
			.decelerationGrass = 120.f,
			.acceleration = 80.f,
			.braking = 150.f
			};
	}
};

