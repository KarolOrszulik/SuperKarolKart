#pragma once

#include "Vehicle.h"

class GroundVehicle : public Vehicle
{
public:
	using Vehicle::Vehicle;

protected:
	void handleMovement(float dt) override;
	virtual int getTextureOffset() = 0;

	virtual float getSpeed() const { return m_speed; }

	struct DrivingParameters
	{
		float wheelbase				= 0.f;
		float maxFriction			= 0.f;
		float lockAngle				= 0.f;
		float maxSpeedRoad			= 0.f;
		float maxSpeedGrass			= 0.f;
		float maxReverseSpeedRatio	= 0.f; // zmieniæ na maxSpeedReverse? nwm
		float decelerationRoad		= 0.f;
		float decelerationGrass		= 0.f;
		float acceleration			= 0.f;
		float braking				= 0.f;
	};

	virtual DrivingParameters getDrivingParameters() = 0;

	float m_acceleration = 0.f;
	float m_speed = 0.f;
};

