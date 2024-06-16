#include "Track.h"

#include <fstream>
#include <map>
#include <iostream>

Track::Tile Track::at(sf::Vector2u tile) const
{
	if (tile.x >= m_size.x || tile.y >= m_size.y)
		return Tile::INVALID;

	return m_tiles[tile2index(tile.x, tile.y)];
}

Track::Tile Track::atPos(sf::Vector2f pos) const
{
	return at({ static_cast<unsigned>(pos.x / GRID_SIZE), static_cast<unsigned>(pos.y / GRID_SIZE) });
}

size_t Track::tile2index(int x, int y) const
{
	return y * m_size.x + x;
}

size_t Track::pos2index(sf::Vector2f pos) const
{
	return tile2index(static_cast<int>(pos.x / GRID_SIZE), static_cast<int>(pos.y / GRID_SIZE));
}

sf::Vector2f Track::index2pos(size_t idx) const
{
	return sf::Vector2f(idx % m_size.x, idx / m_size.x) * static_cast<float>(GRID_SIZE);
}

sf::Vector2f Track::index2posCenter(size_t idx) const
{
	return index2pos(idx) + sf::Vector2f(GRID_SIZE_F / 2.f, GRID_SIZE_F / 2.f);
}

std::optional<int> Track::getCheckpointIndex(sf::Vector2f pos) const
{
	for (size_t i = 0; i < m_checkpoints.size(); i++)
	{
		if (std::ranges::find(m_checkpoints[i], pos2index(pos)) != m_checkpoints[i].end())
			return i;
	}
	return {};
}

void Track::loadTrack(std::string const& path)
{
	std::map<char, Checkpoint> tmpCheckpoints;

	std::ifstream file(path);

	if (!file.is_open())
	{
		throw std::runtime_error("Failed to open file: " + path);
	}

	const std::map<char, Tile> tileMap
	{
		{ '#', Tile::GRASS },
		{ '.', Tile::ROAD_CORNER},
		{ '~', Tile::FINISH},
	};

	std::string line;
	for (int y = 0; std::getline(file, line); y++)
	{
		if (m_size.x != 0 && m_size.x != line.length())
		{
			std::cerr << "Invalid track file: all lines must have the same length" << std::endl;
			return;
		}
		else
		{
			m_size.x = static_cast<unsigned int>(line.length());
		}
		m_size.y = y+1;


		for (int x = 0; x < line.length(); x++)
		{
			char c = line[x];
			try
			{
				if (std::isalpha(c) || c == '~')
				{
					tmpCheckpoints[c].push_back(tile2index( x, y ));
				}
				if (std::isdigit(c))
				{
					m_playerStarts[c - '1'] = tile2index(x, y);
				}
				m_tiles.push_back(tileMap.at(c));
			}
			catch (std::out_of_range const&)
			{
				m_tiles.push_back(Tile::ROAD);
			}
		}
	}

	renderTexture();

	for (auto& [c, cp] : tmpCheckpoints)
	{
		m_checkpoints.push_back(cp);
	}
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
		{ Tile::ROAD_CORNER, sf::IntRect(2 * GRID_SIZE, 0, GRID_SIZE, GRID_SIZE) },
		{ Tile::FINISH,      sf::IntRect(3 * GRID_SIZE, 0, GRID_SIZE, GRID_SIZE) }
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
			sprite.setPosition(x * GRID_SIZE_F, y * GRID_SIZE_F);

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