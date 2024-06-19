#pragma once
#include "GroundItem.h"

class Box : public GroundItem
{
public:
	Box(const sf::Texture& texture,
		sf::Vector2f velocity = {},
		bool singleUse = false);

	void interactWithVehicle(Vehicle&) override;
	void update(float dt) override;

	std::unique_ptr<GroundItem> copy() const override { return std::make_unique<Box>(*this); }
};

