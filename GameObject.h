#pragma once

#include <SFML/Graphics.hpp>

#include <filesystem>

class GameObject
{
public:
	GameObject(sf::Vector2f position = { 0.0f, 0.0f });

	virtual void update(float dt) = 0;
	virtual void draw(sf::RenderTarget& window);

	sf::Vector2f getPosition() const { return m_position; }
	void setPosition(sf::Vector2f position) { m_position = position; } // kochom OOP

	const sf::Texture& getTexture();
	void assignTexture(const sf::Texture& texture);

	virtual ~GameObject() = default;

protected:
	sf::Vector2f m_position;
	float m_angle = 0.0f;

private:
	sf::Sprite m_sprite;

	static sf::Uint8 checkboard[];
};

