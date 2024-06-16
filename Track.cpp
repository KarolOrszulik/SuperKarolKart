#include "Track.h"

#include <fstream>
#include <map>
#include <iostream>

Track::Tile Track::at(sf::Vector2u pos)
{
	if (pos.x >= m_size.x || pos.y >= m_size.y)
		return Tile::INVALID;

	return m_tiles[pos.y * m_size.x + pos.x];
}

void Track::loadTrack(std::string const& path)
{
	std::ifstream file(path);

	if (!file.is_open())
	{
		throw std::runtime_error("Failed to open file: " + path);
	}

	const std::map<char, Tile> tileMap
	{
		{ '#', Tile::GRASS },
		{ ' ', Tile::ROAD },
		{ '.', Tile::ROAD_CORNER}
	};

	std::string line;
	while (std::getline(file, line))
	{
		if (m_size.x != 0 && m_size.x != line.length())
		{
			std::cerr << "Invalid track file: all lines must have the same length" << std::endl;
			return;
		}
		else
		{
			m_size.x = line.length();
		}
		m_size.y++;

		for (char c : line)
		{
			try
			{
				m_tiles.push_back(tileMap.at(c));
			}
			catch (std::out_of_range const&)
			{
				m_tiles.push_back(Tile::INVALID);
				std::cerr << "Invalid character in track file" << std::endl;
			}
		}
	}

	renderTexture();
}

void Track::loadTilemap(std::string const& path)
{
	if (!m_tilemap.loadFromFile(path))
	{
		throw std::runtime_error("Failed to load tilemap: " + path);
	}
}

void Track::renderTexture()
{
	m_texture.create(m_size.x * GRID_SIZE, m_size.y * GRID_SIZE);
	m_texture.clear(sf::Color::Black);

	const std::map<Tile, sf::IntRect> tileRects
	{
		{ Tile::GRASS,		 sf::IntRect(0 * GRID_SIZE, 0, GRID_SIZE, GRID_SIZE) },
		{ Tile::ROAD,		 sf::IntRect(1 * GRID_SIZE, 0, GRID_SIZE, GRID_SIZE) },
		{ Tile::ROAD_CORNER, sf::IntRect(2 * GRID_SIZE, 0, GRID_SIZE, GRID_SIZE) }
	};

	for (uint32_t y = 0; y < m_size.y; ++y)
		for (uint32_t x = 0; x < m_size.x; ++x)
		{
			sf::Sprite sprite;
			sprite.setTexture(m_tilemap);

			Tile tile = at({ x, y });

			if (tile == Tile::INVALID)
				continue;

			sprite.setTextureRect(tileRects.at(tile));
			sprite.setPosition(x * GRID_SIZE, y * GRID_SIZE);

			if (tile == Tile::ROAD_CORNER) //  Rotate the corner tile
			{
				constexpr int WEST  = 0b0001;
				constexpr int EAST  = 0b0010;
				constexpr int NORTH = 0b0100;
				constexpr int SOUTH = 0b1000;

				int neighbours = 0;
				if (at({ x - 1, y }) == Tile::ROAD) neighbours |= WEST;
				if (at({ x + 1, y }) == Tile::ROAD) neighbours |= EAST;
				if (at({ x, y - 1 }) == Tile::ROAD) neighbours |= NORTH;
				if (at({ x, y + 1 }) == Tile::ROAD) neighbours |= SOUTH;

				switch (neighbours)
				{
					case NORTH | WEST: break; // No rotation or translation needed
					case NORTH | EAST: sprite.setRotation(90);  sprite.move({ GRID_SIZE, 0 });         break;
					case SOUTH | WEST: sprite.setRotation(270); sprite.move({ 0,         GRID_SIZE }); break;
					case SOUTH | EAST: sprite.setRotation(180); sprite.move({ GRID_SIZE, GRID_SIZE }); break;
					default: std::cerr << "Corner tile is not in a corner\n";
				}
			}
			m_texture.draw(sprite);
		}

	m_texture.display(); // This is important! It finalizes the texture
}

void Track::draw(sf::RenderTarget& target) const
{
	target.draw(sf::Sprite{m_texture.getTexture()});
}