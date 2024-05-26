#pragma once

#include "GameObject.h"

class Vehicle : public GameObject
{
public:
	void applyAccelerator(float accelerator);
	void applySteering(float steering);

	void update(float dt) override;
	void draw(sf::RenderTarget& window) override;

	sf::Vector2f getPosition() const { return m_position; } // przenieœæ do GameObject (?)

private:
	float m_acceleration = 0.0f;
	float m_steering = 0.0f;
};

