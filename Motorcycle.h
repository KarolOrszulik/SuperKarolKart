#pragma once

#include "GroundVehicle.h"

class Motorcycle : public GroundVehicle
{
public:
	using GroundVehicle::GroundVehicle;

private:
	void handleMovement(float dt) override;

	static constexpr float SKILL_SPEED_MULTIPLIER = 1.3f;
	static constexpr float SKILL_STEERING_MULTIPLIER_TIME = 0.5f;

	GroundVehicle::DrivingParameters getDrivingParameters() const override
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

