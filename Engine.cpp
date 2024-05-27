#include "Engine.h"

#include "Player.h"
#include "Vehicle.h"
#include "UIElement.h"

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
}

void Engine::onUpdate(float dt)
{
	switch (m_state)
	{
	case State::MAIN_MENU:
		stateMainMenu(dt);
		break;
	case State::SETUP:
		stateSetup(dt);
		break;
	case State::RACE:
		stateRace(dt);
		break;
	case State::RESULTS:
		stateResults(dt);
		break;
	}
}

void Engine::populatePlayers(int numPlayers)
{
	m_players.clear();

	for (int i = 0; i < numPlayers; i++)
	{
		std::unique_ptr<Vehicle> vehicle = std::make_unique<Vehicle>();

		m_players.emplace_back(static_cast<Player::ControlScheme>(i), i);
		m_players.back().setVehicle(vehicle.get());

		m_objects.insert(std::move(vehicle));
	}
}

void Engine::stateMainMenu(float dt)
{
	sf::RectangleShape bg;
	bg.setFillColor({ 100,150,230,255 });
	bg.setSize({ static_cast<float>(m_window.getSize().x),
				 static_cast<float>(m_window.getSize().y) });

	UIElement el;
	el.setBackground(bg);
	m_window.draw(el);

	sf::Text text("Press 1-4 to start the 'race'.", m_font, 24);
	text.setFillColor(sf::Color::White);
	m_window.draw(text);
	
	m_objects.clear();
	m_players.clear();

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num1))
		populatePlayers(1);
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num2))
		populatePlayers(2);
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num3))
		populatePlayers(3);
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Num4))
		populatePlayers(4);

	if (getNumPlayers() > 0)
		m_state = State::SETUP;
}

void Engine::stateSetup(float dt)
{
	m_state = State::RACE;
}

void Engine::stateRace(float dt)
{
	for (auto& player : m_players)
		player.controlVehicle();

	m_world.clear();
	m_track.draw(m_world);
	for (auto& obj : m_objects)
	{
		obj->update(dt);
		obj->draw(m_world);
	}
	m_world.display(); // finalize drawing the world


	for (auto& player : m_players)
		player.drawPlayerScreen(m_world, m_window);


	drawFPS(dt);


	sf::Text text("Escape to end race", m_font, 24);
	text.setFillColor(sf::Color::White);
	text.move(0, 30);
	m_window.draw(text);

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape))
		m_state = State::RESULTS;
}

void Engine::stateResults(float dt)
{
	m_state = State::MAIN_MENU;
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