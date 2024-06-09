#include "Engine.h"

#include "Player.h"
#include "Vehicle.h"
#include "UIElement.h"
#include "Menu.h"
#include "UIButton.h"
#include "UIToggleButton.h"
#include "UIRadioGroup.h"


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
			else if(m_menus.contains(m_state)) m_menus[m_state].handleEvent(event);
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

void Engine::setGameState(State state)
{
	m_menus.erase(m_state);
	m_state = state;
}

void Engine::stateMainMenu(float dt)
{
	if (!m_menus.contains(State::MAIN_MENU))
	{
		using UIToggle = UIToggleButton;

		UIButton::Style normStyle;
		normStyle.bgColor = { 0, 0, 0, 0};
		normStyle.fontColor = { 255, 255, 255, 255 };

		UIButton::Style hovStyle;
		hovStyle.fontColor = { 255, 200, 0, 255 };

		UIButton::Style selStyle;
		selStyle.fontColor = { 255, 0, 0, 255 };

		auto btn = std::make_unique<UIToggle>(normStyle, hovStyle, selStyle);
		btn->setPosition({ 50,50 });
		btn->setText("1 Player");
		btn->setFont(m_font);
		btn->setCharacterSize(24);
		btn->shrinkSizeToText();
		btn->setOnSelected([this]() { populatePlayers(1); });

		auto btn2 =std::make_unique<UIToggle>(normStyle, hovStyle, selStyle);
		btn2->setPosition({ 50,150 });
		btn2->setText("2 Players");
		btn2->setFont(m_font);
		btn2->setCharacterSize(24);
		btn2->shrinkSizeToText();
		btn2->setOnSelected([this]() { populatePlayers(2);});

		auto btn3 = std::make_unique<UIToggle>(normStyle, hovStyle, selStyle);
		btn3->setPosition({ 50,250 });
		btn3->setText("3 Players");
		btn3->setFont(m_font);
		btn3->setCharacterSize(24);
		btn3->shrinkSizeToText();
		btn3->setOnSelected([this]() { populatePlayers(3); });

		auto btn4 = std::make_unique<UIToggle>(normStyle, hovStyle, selStyle);
		btn4->setPosition({ 50,350 });
		btn4->setText("4 Players");
		btn4->setFont(m_font);
		btn4->setCharacterSize(24);
		btn4->shrinkSizeToText();
		btn4->setOnSelected([this]() { populatePlayers(4); });

		auto uiRadioGroup = std::make_unique<UIRadioGroup>();
		uiRadioGroup->addElement(std::move(btn));
		uiRadioGroup->addElement(std::move(btn2));
		uiRadioGroup->addElement(std::move(btn3));
		uiRadioGroup->addElement(std::move(btn4));

		auto btnGO = std::make_unique<UIButton>(normStyle, hovStyle, selStyle);
		btnGO->setPosition({ 250,50 });
		btnGO->setText("START!");
		btnGO->setFont(m_font);
		btnGO->setCharacterSize(24);
		btnGO->shrinkSizeToText();
		btnGO->onRelease = [this]() {
			if (getNumPlayers() > 0) {
				setGameState(State::SETUP);
			} 
		};

		Menu& m_mainMenu = m_menus[State::MAIN_MENU];
		m_mainMenu.setBgColor({ 20,170,150,255 });
		m_mainMenu.setSize({ (float)m_window.getSize().x, (float)m_window.getSize().y });

		m_mainMenu.addElement(std::move(uiRadioGroup));
		m_mainMenu.addElement(std::move(btnGO));
	}
	m_window.draw(m_menus[State::MAIN_MENU]);
}

void Engine::stateSetup(float dt)
{
	setGameState(State::RACE);
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
	m_players.clear();
	m_objects.clear();
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