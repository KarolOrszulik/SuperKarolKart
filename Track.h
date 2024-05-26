#pragma once

#include <SFML/Graphics.hpp>
#include <unordered_map>

class Track
{
public:
	enum class Tile;

	void loadTrack(std::string const& path);
	void loadTilemap(std::string const& path);
	Tile at(sf::Vector2u);

	void draw(sf::RenderTarget& target) const;

private:
	void renderTexture();

	sf::RenderTexture m_texture;

	sf::Texture m_tilemap;
	sf::Vector2u m_size;
	std::vector<Tile> m_tiles;
};

enum class Track::Tile
{
	INVALID = -1,
	GRASS,
	ROAD,
	ROAD_CORNER,
};