#pragma once

#include <memory>
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include "Track.h"

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

	sf::RenderWindow m_window;
	Track m_track;
};

