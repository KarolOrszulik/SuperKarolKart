#pragma once
#include "GroundItem.h"

class Box : public GroundItem
{
public:
	Box() = default;

	void interactWithVehicle(Vehicle&) override;
	void update(float dt) override;
	void draw(sf::RenderTarget& target) override;

	std::unique_ptr<GroundItem> copy() const override { return std::make_unique<Box>(*this); }


private:
	sf::RectangleShape box;
	sf::Texture boxTexture;
};
