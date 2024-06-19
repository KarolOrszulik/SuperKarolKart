#pragma once

#include "GroundVehicle.h"

class Motorcycle : public GroundVehicle
{
public:
	using GroundVehicle::GroundVehicle;

private:
	std::filesystem::path getTexturePath() const override { return "assets/vehicles/motorcycle.png"; }

	void handleMovement(float dt) override;

	GroundVehicle::DrivingParameters getDrivingParameters() override
	{
		return GroundVehicle::DrivingParameters{
			.wheelbase = 10.f,
			.maxFriction = 250.f,
			.lockAngle = 40.f * 3.14f / 180.f,
			.maxSpeedRoad = 120.f,
			.maxSpeedGrass = 40.f,
			.maxReverseSpeedRatio = 0.3f,
			.decelerationRoad = 100.f,
			.decelerationGrass = 150.f,
			.acceleration = 90.f,
			.braking = 180.f
		};
	}
};

