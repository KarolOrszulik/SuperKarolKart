#include "Engine.h"

std::shared_ptr<Engine> Engine::s_instance = nullptr;

std::shared_ptr<Engine> Engine::getInstance()
{
	if (s_instance == nullptr)
		s_instance = std::make_shared<Engine>();

	return s_instance;
}

void Engine::init(uint32_t width, uint32_t height, std::string const& title)
{
	m_window.create(sf::VideoMode(width, height), title);
}

void Engine::run()
{
	onStart();
	while (m_window.isOpen())
	{
		sf::Event event;
		while (m_window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed) m_window.close();
		}
		m_window.clear();

		onUpdate();

		m_window.display();
	}
}

void Engine::onStart()
{
	m_track.loadTilemap("assets/track_tileset.png");
	m_track.loadTrack("assets/track_00.txt");
}

void Engine::onUpdate()
{
	m_track.draw(m_window);
}