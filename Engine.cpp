#include "Engine.h"

#include "Player.h"
#include "Vehicle.h"

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
	m_world.create(m_window.getSize().x, m_window.getSize().y);

	m_track.loadTilemap("assets/track_tileset.png");
	m_track.loadTrack("assets/track_00.txt");

	if (!m_font.loadFromFile("assets/Minecraft.ttf"))
		throw std::runtime_error("Font could not be found");

	// === SEKCJA DEMO
	Vehicle* v1 = new Vehicle();
	Vehicle* v2 = new Vehicle();

	m_objects.insert(std::unique_ptr<GameObject>(v1));
	m_objects.insert(std::unique_ptr<GameObject>(v2));

	m_players.emplace_back(Player::ControlScheme::WASD,   0);
	m_players.emplace_back(Player::ControlScheme::ARROWS, 1);

	m_players[0].setVehicle(v1);
	m_players[1].setVehicle(v2);
	// === KONIEC SEKCJI DEMO
}

void Engine::onUpdate(float dt)
{
	static int i = 0;

	m_world.clear();

	m_track.draw(m_world);
	// drawFPS(dt);

	for (auto& player : m_players)
		player.controlVehicle();

	for (auto& obj : m_objects)
	{
		obj->update(dt);
		obj->draw(m_world);
	}

	m_world.display();

	sf::Sprite worldsprite = sf::Sprite(m_world.getTexture());

	for (auto& player : m_players)
		player.drawPlayerScreen(m_world, m_window);
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