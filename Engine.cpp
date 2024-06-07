#include "Engine.h"

#include "Player.h"
#include "Vehicle.h"
#include "UIElement.h"
#include "Menu.h"
#include "UIButton.h"
#include <iostream>

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
			else if(m_menus.contains(m_state)) m_menus[m_state].handleEvents(event);
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
	if (!m_menus.contains(State::MAIN_MENU))
	{
		UIButton::Style normalStyle;
		normalStyle.bgColor = { 0, 0, 0, 0};
		normalStyle.fontColor = { 255, 255, 255, 255 };

		UIButton::Style hoveredStyle;
		hoveredStyle.fontColor = { 255, 200, 0, 255 };

		UIButton::Style selectedStyle;
		selectedStyle.fontColor = { 255, 0, 0, 255 };

		std::unique_ptr<UIButton> btn = 
			std::make_unique<UIButton>(normalStyle, hoveredStyle, selectedStyle);
		btn->setPosition({ 50,50 });
		btn->onClick = [this]() { populatePlayers(1); std::cout << "Klik"; };
		btn->onRelease = [this]() { std::cout << "Puszczone"; };
		//btn->onMouseEnter = mouseEnterEvent;
		//btn->onMouseLeave = mouseLeaveEvent;
		btn->setText("1 Player");
		btn->setFont(m_font);
		btn->setCharacterSize(24);
		btn->shrinkSizeToText();
		btn->onMouseEnter = []() { std::cout << "Weszlo\n"; };
		btn->onMouseLeave = []() { std::cout << "Wyszlo\n"; };


		//std::unique_ptr<UIButton> btn2 = std::make_unique<UIButton>(*btn);
		//btn2->setPosition({ 50, 175 });
		//btn2->onClick = [this](UIButton&) { populatePlayers(2); };
		//btn2->setText("2 Players");
		//btn2->shrinkSizeToText();

		//std::unique_ptr<UIButton> btn3 = std::make_unique<UIButton>(*btn);
		//btn3->setPosition({ 50, 300 });
		//btn3->onClick = [this](UIButton&) { populatePlayers(3); };
		//btn3->setText("3 Players");
		//btn3->shrinkSizeToText();

		//std::unique_ptr<UIButton> btn4 = std::make_unique<UIButton>(*btn);
		//btn4->setPosition({ 50, 425 });
		//btn4->onClick = [this](UIButton&) { populatePlayers(4); };
		//btn4->setText("4 Players");
		//btn4->shrinkSizeToText();

		std::unique_ptr<UIButton> btnGO = std::make_unique<UIButton>(*btn);
		btnGO->setPosition({ 250, 425 });
		btnGO->onClick = [this]() {
			if (getNumPlayers() > 0) {
				m_state = State::SETUP;
				m_menus.erase(State::MAIN_MENU);
			}
		};
		btnGO->setText("START!");
		btnGO->shrinkSizeToText();

		Menu& m_mainMenu = m_menus[State::MAIN_MENU];
		m_mainMenu.setBgColor({ 20,170,150,255 });
		m_mainMenu.setSize({ (float)m_window.getSize().x, (float)m_window.getSize().y });

		//std::unique_ptr<UIRadioGroup> radioGroup = std::make_unique<UIRadioGroup>();
		//radioGroup->addButton(std::move(btn));
		//radioGroup->addButton(std::move(btn2));
		//radioGroup->addButton(std::move(btn3));
		//radioGroup->addButton(std::move(btn4));

		m_mainMenu.addElement(std::move(btn));
		/*m_mainMenu.addElement(std::move(btn));
		m_mainMenu.addElement(std::move(btn2));
		m_mainMenu.addElement(std::move(btn3));
		m_mainMenu.addElement(std::move(btn4));*/
		m_mainMenu.addElement(std::move(btnGO));
	}
	m_window.draw(m_menus[State::MAIN_MENU]);
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