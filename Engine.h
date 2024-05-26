#pragma once

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include <memory>
#include <unordered_set>

#include "Track.h"
#include "GameObject.h"
#include "Player.h"

class Engine
{
public:
	static std::shared_ptr<Engine> getInstance();

	void init(uint32_t width, uint32_t height, std::string const& title);
	void run();

private:
	static std::shared_ptr<Engine> s_instance;

	void onStart();
	void onUpdate(float dt);

	void drawFPS(float dt);

	std::unordered_set<std::unique_ptr<GameObject>> m_objects;
	std::vector<Player> m_players;

	sf::RenderTexture m_world;

	sf::RenderWindow m_window;
	Track m_track;

	sf::Font m_font;
};

