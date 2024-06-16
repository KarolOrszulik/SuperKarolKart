#pragma once

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include <memory>
#include <unordered_set>

#include "Track.h"
#include "GameObject.h"
#include "Player.h"
#include "Menu.h"

class Engine
{
public:
	static std::shared_ptr<Engine> getInstance();

	int getNumPlayers() const { return static_cast<int>(m_players.size()); }
	sf::Vector2u getWindowSize() const { return m_window.getSize(); }

	void init(uint32_t width, uint32_t height, std::string const& title);
	void run();

private:
	enum class State
	{
		MAIN_MENU,
		SETUP_MENU,
		RACE,
		RESULTS
	};

	static std::shared_ptr<Engine> s_instance;

	void onStart();
	void onUpdate(float dt);

	void drawFPS(float dt);

	void setGameState(State state);

	void stateMainMenu(float dt);
	void stateSetup(float dt);
	void stateRace(float dt);
	void stateResults(float dt);

	void populatePlayers(int numPlayers);

	State m_state = State::MAIN_MENU;

	std::unordered_set<std::unique_ptr<GameObject>> m_objects;
	std::vector<Player> m_players;

	sf::RenderTexture m_world;

	sf::RenderWindow m_window;
	Track m_track;

	sf::Font m_font;
	std::unordered_map<State, Menu> m_menus;

	struct Settings
	{
		int numPlayers = -1;
		std::string playerName[4]{};
	} gameSettings;

	friend float operator ""_vh(long double);
	friend float operator ""_vw(long double);
	friend unsigned operator ""_vh(unsigned long long);
	friend unsigned operator ""_vw(unsigned long long);
};

