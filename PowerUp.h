#pragma once

#include <SFML/Graphics.hpp>

class Vehicle;

class PowerUp
{
public:
	PowerUp(const sf::Texture& texture) { m_sprite.setTexture(texture); };

	void use(Vehicle& owner);

	bool canBeUsed() const { return m_usageLeft > 0; }
	int getUsageLeft() const { return m_usageLeft; }

	void assignTexture(const sf::Texture& texture) { m_sprite.setTexture(texture); };
	const sf::Texture& getTexture() const { return *m_sprite.getTexture(); }
private:
	sf::Sprite m_sprite;
	int m_usageLeft = 1;
	virtual void usePowerUp(Vehicle& owner) = 0;
};
