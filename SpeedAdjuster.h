#pragma once

#include "GroundItem.h"

class SpeedAdjuster : public GroundItem
{
public:
	SpeedAdjuster(float speedMultiplier = 2.0, sf::Vector2f velocity = {0.f, 0.f}, bool singleUse = true)
		: m_speedMultiplier(speedMultiplier), m_velocity(velocity), m_singleUse(singleUse) {}

	void interactWithVehicle(Vehicle&) override;
	void update(float dt) override;
	void draw(sf::RenderTarget&) override;
	std::unique_ptr<GroundItem> copy() const override { return std::make_unique<SpeedAdjuster>(*this); }

private:
	float m_speedMultiplier = 2.f;
	sf::Vector2f m_velocity;
	bool m_singleUse = false;
};

