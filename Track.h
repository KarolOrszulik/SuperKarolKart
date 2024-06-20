#pragma once

#include <SFML/Graphics.hpp>
#include <unordered_map>
#include <array>
#include <optional>

class Track
{
public:
	enum class Tile;

	void loadTrack(std::string const& path);
	void loadTilemap(std::string const& path);

	Tile at(sf::Vector2u) const;
	Tile atPos(sf::Vector2f pos) const;

	size_t tile2index(int x, int y) const;
	size_t pos2index(sf::Vector2f) const;

	sf::Vector2f index2pos(size_t idx) const;
	sf::Vector2f index2posCenter(size_t idx) const;

	std::optional<int> getCheckpointIndex(sf::Vector2f pos, float radius = 0.f) const;
	size_t getCheckpointCount() const { return m_checkpoints.size(); }

	float getGridSizeF() const { return GRID_SIZE_F; }
	unsigned int getGridSize() const { return GRID_SIZE; }

	void draw(sf::RenderTarget& target) const;

	sf::Vector2f getSize() const { return sf::Vector2f(m_size * GRID_SIZE); }
	sf::Vector2f getPlayerStartingPos(size_t idx) const { return index2posCenter(m_playerStarts[idx]); }

private:
	static constexpr unsigned int GRID_SIZE = 16;
	static constexpr float GRID_SIZE_F = static_cast<float>(GRID_SIZE);

	typedef std::vector<size_t> Checkpoint;

	void renderTexture();

	sf::RenderTexture m_texture;

	sf::Texture m_tilemap;
	sf::Vector2u m_size;
	std::vector<Tile> m_tiles;
	std::vector<Checkpoint> m_checkpoints;
	std::array<size_t, 4> m_playerStarts;
};

enum class Track::Tile
{
	INVALID = -1,
	GRASS,
	ROAD,
	ROAD_CORNER,
	FINISH,
	VLC,
	TIRES,
};