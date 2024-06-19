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

	static bool loadTextureWithFallback(sf::Texture& texture, const std::filesystem::path& path, const std::filesystem::path& fallbackPath = "assets/notexture.png");

	virtual ~GameObject() = default;

protected:
	sf::Vector2f m_position;
	virtual std::filesystem::path getTexturePath() const = 0; 

//private:
	sf::Texture m_texture;
	bool m_textureLoaded = false;
};

