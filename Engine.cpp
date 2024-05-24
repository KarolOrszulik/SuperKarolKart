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
	static sf::Clock clock;

	onStart();
	while (m_window.isOpen())
	{
		sf::Event event;
		while (m_window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed) m_window.close();
		}
		m_window.clear();

		onUpdate(clock.restart().asSeconds()); // reset clock and pass delta time in seconds

		m_window.display();
	}
}

void Engine::onStart()
{
	m_track.loadTilemap("assets/track_tileset.png");
	m_track.loadTrack("assets/track_00.txt");

	if (!m_font.loadFromFile("assets/Minecraft.ttf"))
		throw std::runtime_error("Font could not be found");
}

void Engine::onUpdate(float dt)
{
	m_track.draw(m_window);
	drawFPS(dt);
}

void Engine::drawFPS(float dt)
{
	constexpr float UPDATE_INTERVAL = 0.5f;

	static float totalTime = 0.f;
	static int   frames   = 0;
	static sf::Text text("...", m_font, 24);
	// Tyle statycznych zmiennych, ¿e to w zasadzie powinna byæ klasa. Ale w specyfikacji nie ma takiej klasy, wiêc trudno.

	text.setFillColor(sf::Color::White);
	text.setOutlineColor(sf::Color::Black);
	text.setOutlineThickness(2);

	m_window.draw(text);

	totalTime += dt;
	frames++;

	if (totalTime < UPDATE_INTERVAL) return;

	text.setString("FPS: " + std::to_string(static_cast<int>(frames / totalTime)));

	totalTime = 0.f;
	frames = 0;
}