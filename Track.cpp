#include "Track.h"
#include "GroundItemSpawner.h"
#include "SpeedAdjuster.h"
#include "Engine.h"
#include "Box.h"

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
	return sf::Vector2f(float(idx % m_size.x), float(idx / m_size.x)) 
		* static_cast<float>(GRID_SIZE);
}

sf::Vector2f Track::index2posCenter(size_t idx) const
{
	return index2pos(idx) + sf::Vector2f(GRID_SIZE_F / 2.f, GRID_SIZE_F / 2.f);
}

// TODO: wykorzystaæ radius, ¿eby sprawdzaæ czy gracz jest w okreœlonym checkpointcie
std::optional<int> Track::getCheckpointIndex(sf::Vector2f pos, float radius) const 
{
	float minDistance = std::numeric_limits<float>::max();
	std::optional<int> closestCheckpoint;

	auto squareCircleOverlap = [](sf::Vector2f squarePos, sf::Vector2f squareSize, sf::Vector2f circlePos, float radius) -> bool
	{
		float dx = std::abs(circlePos.x - squarePos.x);
		float dy = std::abs(circlePos.y - squarePos.y);

		if (dx > squareSize.x / 2.f + radius) return false;
		if (dy > squareSize.y / 2.f + radius) return false;

		if (dx <= squareSize.x / 2.f) return true;
		if (dy <= squareSize.y / 2.f) return true;

		float cornerDistance = std::hypot(dx - squareSize.x / 2.f, dy - squareSize.y / 2.f);
		return cornerDistance <= radius;
	};

	for (size_t i = 0; i < m_checkpoints.size(); i++)
	{
		for (size_t j = 0; j < m_checkpoints[i].size(); j++)
		{
			sf::Vector2f squareCenter = index2posCenter(m_checkpoints[i][j]);
			if (squareCircleOverlap(squareCenter, { GRID_SIZE_F, GRID_SIZE_F }, pos, radius))
			{
				float distance = std::hypot(squareCenter.x - pos.x, squareCenter.y - pos.y);
				if (distance < minDistance)
				{
					minDistance = distance;
					closestCheckpoint = i;
				}
			}
		}
	}

	if (minDistance < radius)
		return closestCheckpoint;
	else
		return {};
}

void Track::loadTrack(std::string const& path)
{
	std::shared_ptr<Engine> engine = Engine::getInstance();
	std::map<char, Checkpoint> tmpCheckpoints;

	std::ifstream file(path);

	if (!file.is_open())
	{
		throw std::runtime_error("Failed to open file: " + path);
	}

	const std::map<char, Tile> tileMap
	{
		{ '#', Tile::GRASS },
		{ '.', Tile::ROAD_CORNER },
		{ '~', Tile::FINISH },
		{ '?', Tile::VLC }
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
				if (c == '!') // speed booster
				{
					std::unique_ptr<GroundItem> pBooster = std::make_unique<SpeedAdjuster>(engine->getTexture("nitro"), 2.0f);
					std::unique_ptr<GroundItemSpawner> pSpawner =
						std::make_unique<GroundItemSpawner>(sf::Vector2f(x + 0.5f, y + 0.5f) * GRID_SIZE_F, std::move(pBooster));
					engine->addObject(std::move(pSpawner));
				}
				else if (c == '@') // speed booster - non-single use
				{
					std::unique_ptr<GroundItem> pBooster = std::make_unique<SpeedAdjuster>(engine->getTexture("plate"), 2.0f, sf::Vector2f{}, false);
					std::unique_ptr<GroundItemSpawner> pSpawner =
						std::make_unique<GroundItemSpawner>(sf::Vector2f(x + 0.5f, y + 0.5f) * GRID_SIZE_F, std::move(pBooster));
					engine->addObject(std::move(pSpawner));
				}
				else if (c == '$') // lootbox
				{
					std::unique_ptr<GroundItem> box = std::make_unique<Box>(engine->getTexture("box"));
					std::unique_ptr<GroundItemSpawner> pSpawner =
						std::make_unique<GroundItemSpawner>(sf::Vector2f(x + 0.5f, y + 0.5f) * GRID_SIZE_F, std::move(box));
					engine->addObject(std::move(pSpawner));
				}
				else if (c == '%') // oil spill
				{
					std::unique_ptr<GroundItem> oil = std::make_unique<SpeedAdjuster>(engine->getTexture("oil"), 0.5f);
					std::unique_ptr<GroundItemSpawner> pSpawner =
						std::make_unique<GroundItemSpawner>(sf::Vector2f(x + 0.5f, y + 0.5f) * GRID_SIZE_F, std::move(oil));
					engine->addObject(std::move(pSpawner));
				}
				else if (std::isalpha(c) || c == '~')
				{
					tmpCheckpoints[c].push_back(tile2index( x, y ));
				}
				else if (std::isdigit(c))
				{
					m_playerStarts.at(c - '1') = tile2index(x, y);
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
		{ Tile::FINISH,      sf::IntRect(3 * GRID_SIZE, 0, GRID_SIZE, GRID_SIZE) },
		{ Tile::VLC,         sf::IntRect(4 * GRID_SIZE, 0, GRID_SIZE, GRID_SIZE) }
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