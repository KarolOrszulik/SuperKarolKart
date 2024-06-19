#pragma once

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include <memory>
#include <unordered_set>
#include <filesystem>

#include "Track.h"
#include "GameObject.h"
#include "Player.h"
#include "Menu.h"

#include "AveragingCounter.h"

#include <iostream>

class Engine
{
public:
	static std::shared_ptr<Engine> getInstance();

	int getNumPlayers() const { return static_cast<int>(m_players.size()); }
	sf::Vector2u getWindowSize() const { return m_window.getSize(); }

	void addObject(std::unique_ptr<GameObject> object) { m_objectsToAdd.push_back(std::move(object)); }
	void flagForRemoval(GameObject* object) { m_objectsToRemove.push_back(object); }

	auto& getObjects() { return m_objects; }

	unsigned int getGridSize() const { return m_track.getGridSize(); }
	float getGridSizeF() const { return m_track.getGridSizeF(); }

	void init(uint32_t width, uint32_t height, std::string const& title);
	void run();

	void resetWindowView();
	const sf::Font& getFont() const { return m_font; };

private:
	enum class State
	{
		MAIN_MENU,
		SETUP_MENU,
		VEHICLE_MENU,
		PRE_RACE,
		RACE,
		RESULTS
	};

	static std::shared_ptr<Engine> s_instance;

	void onStart();
	void onUpdate(float dt);

	AveragingCounter m_fpsCounter;
	void drawFPS(float dt);

	void setGameState(State state);

	void stateMainMenu(float dt);
	void stateSetup(float dt);
	void stateVehicleMenu(float dt);
	void statePreRace(float dt);
	void stateRace(float dt);
	void stateResults(float dt);

	void populatePlayers();

	State m_state = State::MAIN_MENU;

	std::unordered_set<std::unique_ptr<GameObject>> m_objects;
	std::vector<GameObject*> m_objectsToRemove;
	std::vector<std::unique_ptr<GameObject>> m_objectsToAdd;
	std::vector<Player> m_players;

	sf::RenderTexture m_world;

	sf::RenderWindow m_window;
	Track m_track;

	sf::Font m_font;

	std::unordered_map<State, Menu> m_menus;
	std::filesystem::path m_assetsPath;
	struct Settings
	{
		int numPlayers = -1;
		std::string trackName;
		std::string playerNames[4]{};
		int vehicle[4]{};
	} gameSettings;

	friend float operator ""_vh(long double);
	friend float operator ""_vw(long double);
	friend unsigned operator ""_vh(unsigned long long);
	friend unsigned operator ""_vw(unsigned long long);
};

