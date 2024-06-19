#pragma once
#include "GroundItem.h"

class SteeringAdjuster : public GroundItem
{
public:
	SteeringAdjuster(
		const sf::Texture& texture,
		float steeringMultiplier = -1.f, 
		sf::Vector2f velocity = {},
		bool singleUse = true);

	void interactWithVehicle(Vehicle&) override;
	void update(float dt) override;
	//void draw(sf::RenderTarget&) override;
	std::unique_ptr<GroundItem> copy() const override { return std::make_unique<SteeringAdjuster>(*this); }

	float getSteeringMultiplier() const { return m_steeringMultiplier; }
private:
	float m_steeringMultiplier = -1.f;
};

