#pragma once

#include <SFML/Graphics.hpp>
class Vehicle;

class PowerUp
{
public:
	void use(Vehicle& owner);

	bool canBeUsed() const { return m_usageLeft > 0; }
	int getUsageLeft() const { return m_usageLeft; }


private:
	sf::Sprite m_sprite;
	int m_usageLeft = 1;
	virtual void usePowerUp(Vehicle& owner) = 0;
};
