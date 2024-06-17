#include "Engine.h"

#include "Player.h"
#include "Vehicle.h"
#include "SpeedAdjuster.h"
#include "UIElement.h"
#include "Menu.h"
#include "UIButton.h"
#include "UIToggleButton.h"
#include "UIRadioGroup.h"
#include "UITextInput.h"

#include <iostream>


std::shared_ptr<Engine> Engine::s_instance = nullptr;

float operator ""_vh(long double heightVh)
{
	auto engine = Engine::getInstance();
	unsigned windowHeight = engine->m_window.getSize().y;
	return static_cast<float>(heightVh / 100.0 * windowHeight);
}

float operator ""_vw(long double widthVw)
{
	auto engine = Engine::getInstance();
	unsigned windowWidth = engine->m_window.getSize().x;
	return static_cast<float>(widthVw / 100.0 * windowWidth);
}

unsigned operator""_vh(unsigned long long heightVh)
{
	auto engine = Engine::getInstance();
	unsigned windowHeight = engine->m_window.getSize().y;
	return heightVh / 100.0 * windowHeight;
}

unsigned operator""_vw(unsigned long long widthVw)
{

	auto engine = Engine::getInstance();
	unsigned windowWidth = engine->m_window.getSize().x;
	return widthVw / 100.0 * windowWidth;
}


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
			else if (event.type == sf::Event::Resized)
			{
				sf::Vector2u newSize = m_window.getSize();

				sf::View view(sf::FloatRect(0, 0, newSize.x, newSize.y));
				m_window.setView(view);
				m_menus.erase(m_state);
			}
			else if(m_menus.contains(m_state)) m_menus[m_state].handleEvent(event);
		}
		m_window.clear();

		onUpdate(clock.restart().asSeconds()); // reset clock and pass delta time in seconds

		m_window.display();
	}
}

void Engine::onStart()
{
	m_track.loadTilemap("assets/track_tileset.png");

	m_track.loadTrack("assets/track_02.txt");
	m_world.create(m_track.getSize().x, m_track.getSize().y);

	if (!m_font.loadFromFile("assets/SKK.ttf"))
		throw std::runtime_error("Font could not be found");
}

void Engine::onUpdate(float dt)
{
	switch (m_state)
	{
	case State::MAIN_MENU:
		stateMainMenu(dt);
		break;
	case State::SETUP_MENU:
		stateSetup(dt);
		break;
	case State::PRE_RACE:
		statePreRace(dt);
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
		std::unique_ptr<Vehicle> vehicle = std::make_unique<Vehicle>(&m_track, m_track.getPlayerStartingPos(i));

		m_players.emplace_back(static_cast<Player::ControlScheme>(i), i);
		m_players.back().setVehicle(vehicle.get());

		m_objects.insert(std::move(vehicle));
	}
}

void Engine::setGameState(State state)
{
	m_state = state;
	m_menus.erase(m_state);
}

void Engine::stateMainMenu(float dt)
{
	if (!m_menus.contains(State::MAIN_MENU))
	{
		using UIToggle = UIToggleButton;
		using namespace std;

		UIButton::Style normStyle;
		normStyle.bgColor = { 0, 0, 0, 0 };
		normStyle.fontColor = { 255, 255, 255, 255 };

		UIButton::Style hovStyle;
		hovStyle.fontColor = { 255, 200, 0, 255 };

		UIButton::Style selStyle;
		selStyle.fontColor = { 255, 0, 0, 255 };

		Menu& m_mainMenu = m_menus[State::MAIN_MENU];
		m_mainMenu.setBgColor({ 20,170,150,255 });
		m_mainMenu.setSize(
			{ (float)m_window.getSize().x, (float)m_window.getSize().y });

		// <---- Title ---->
		UIButton title(normStyle);
		title.setPosition({ 0, 5.0_vh });
		title.setText("Super Karol Kart");
		title.setFont(m_font);
		title.setCharacterSize(15_vh);
		title.shrinkSizeToText();
		title.centerHorizontally(m_mainMenu.getWidth());

		m_mainMenu.addElement(make_unique<UIButton>(title));

		// <---- Start Button ---->
		UIButton btnPlay(normStyle, hovStyle, selStyle);
		btnPlay.setPosition({ 0, 35.0_vh });
		btnPlay.setText("PLAY");
		btnPlay.setFont(m_font);
		btnPlay.setCharacterSize(10_vh);
		btnPlay.shrinkSizeToText();
		btnPlay.centerHorizontally(m_mainMenu.getWidth());
		btnPlay.onRelease = [this]() { setGameState(State::SETUP_MENU); };

		m_mainMenu.addElement(make_unique<UIButton>(btnPlay));

		// <---- Exit Button ---->
		UIButton btnExit(btnPlay);
		btnExit.setPosition({ 0, 60.0_vh });
		btnExit.setText("EXIT");
		btnExit.shrinkSizeToText();
		btnExit.centerHorizontally(m_mainMenu.getWidth());
		btnExit.onRelease = [this]() { m_window.close(); };

		m_mainMenu.addElement(make_unique<UIButton>(btnExit));
	}
	m_window.draw(m_menus[State::MAIN_MENU]);
}

void Engine::stateSetup(float dt)
{
	if (!m_menus.contains(State::SETUP_MENU))
	{
		using UIToggle = UIToggleButton;
		using namespace std;

		// <---- Preparing settings ---->
		gameSettings.numPlayers = -1;

		UIButton::Style normStyle;
		normStyle.bgColor = { 0, 0, 0, 0 };
		normStyle.fontColor = { 255, 255, 255, 255 };

		UIButton::Style hovStyle;
		hovStyle.fontColor = { 255, 200, 0, 255 };

		UIButton::Style selStyle;
		selStyle.fontColor = { 255, 0, 0, 255 };

		Menu& menu = m_menus[State::SETUP_MENU];
		menu.setBgColor({ 20,170,150,255 });
		menu.setSize(
			{ (float)m_window.getSize().x, (float)m_window.getSize().y });

		// <---- Title ---->
		UIButton title(normStyle);
		title.setPosition({ 0, 5.0_vh });
		title.setText("Super Karol Kart");
		title.setFont(m_font);
		title.setCharacterSize(15_vh);
		title.shrinkSizeToText();
		title.centerHorizontally(menu.getWidth());

		menu.addElement(make_unique<UIButton>(title));

		// <---- Player Number Selection ---->
		auto playerNumSelect = std::make_unique<UIRadioGroup>();

		UIToggle onePlayerBtn(normStyle, hovStyle, selStyle);
		onePlayerBtn.setPosition({ 5.0_vw, 30.0_vh });
		onePlayerBtn.setText("1 Player");
		onePlayerBtn.setFont(m_font);
		onePlayerBtn.setCharacterSize(5_vh);
		onePlayerBtn.shrinkSizeToText();
		onePlayerBtn.setOnSelected([this]() { gameSettings.numPlayers = 1; });

		UIToggle twoPlayerBtn(onePlayerBtn);
		twoPlayerBtn.setPosition({ 5.0_vw, 45.0_vh });
		twoPlayerBtn.setText("2 Players");
		twoPlayerBtn.setOnSelected([this]() { gameSettings.numPlayers = 2; });

		UIToggle threePlayerBtn(twoPlayerBtn);
		threePlayerBtn.setPosition({ 5.0_vw, 60.0_vh });
		threePlayerBtn.setText("3 Players");
		threePlayerBtn.setOnSelected([this]() { gameSettings.numPlayers = 3; });

		UIToggle fourPlayerBtn(threePlayerBtn);
		fourPlayerBtn.setPosition({ 5.0_vw, 75.0_vh });
		fourPlayerBtn.setText("4 Players");
		fourPlayerBtn.setOnSelected([this]() { gameSettings.numPlayers = 4; });

		playerNumSelect->addElement(make_unique<UIToggle>(onePlayerBtn));
		playerNumSelect->addElement(make_unique<UIToggle>(twoPlayerBtn));
		playerNumSelect->addElement(make_unique<UIToggle>(threePlayerBtn));
		playerNumSelect->addElement(make_unique<UIToggle>(fourPlayerBtn));

		menu.addElement(move(playerNumSelect));

		// <---- Text input ---->
		normStyle.bgColor = { 0, 0, 200, 255 };
		normStyle.fontColor = { 255, 255, 255, 255 };
		hovStyle.bgColor = { 0, 200, 0, 255 };
		hovStyle.fontColor = { 255, 255, 255, 255 };
		selStyle.bgColor = { 200, 0, 0, 255 };
		selStyle.fontColor = { 255, 255, 255, 255 };

		UITextInput textInput(normStyle, hovStyle, selStyle);
		textInput.setPosition({ 5.0_vw, 80.0_vh });
		textInput.setSize({ 20.0_vw, 5.0_vh });
		textInput.setFont(m_font);
		textInput.setCharacterSize(5_vh);
		textInput.setPlaceHolder("Enter your name");
		textInput.onTextEntered = [this](const std::string& text) {
			gameSettings.playerName[0] = text;
		};
		textInput.shrinkSizeToText();

		menu.addElement(make_unique<UITextInput>(textInput));

		// <---- Start Button ---->
		UIButton btnGO(onePlayerBtn);
		btnGO.setPosition({ 0, 90.0_vh });
		btnGO.setText("START!");
		btnGO.setFont(m_font);
		btnGO.shrinkSizeToText();
		btnGO.centerHorizontally(menu.getWidth());
		btnGO.onRelease = [this]() {
			// ZAIMPLEMENTOWAC REGEXA, ZEBY BYLY TYLKO LITERY W IMIONACH
			if (gameSettings.numPlayers > 0) {
				populatePlayers(gameSettings.numPlayers);
				setGameState(State::PRE_RACE);
			}
		};

		menu.addElement(make_unique<UIButton>(btnGO));
	}
	m_window.draw(m_menus[State::SETUP_MENU]);
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape))
		setGameState(State::MAIN_MENU);
}

void Engine::statePreRace(float dt)
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


	// remove objects from m_objects which are in m_objectsToRemove
	for (auto& obj : m_objectsToRemove)
	{
		auto it = std::ranges::find_if(m_objects, [obj](auto& p) { return p.get() == obj; });
		if (it != m_objects.end())
			m_objects.erase(it);
	}
	m_objectsToRemove.clear();


	for (auto& player : m_players)
		player.drawPlayerScreen(m_world, m_window);


	drawFPS(dt);


	sf::Text text("Escape to end race", m_font, 24);
	text.setFillColor(sf::Color::White);
	text.move(0, 30);
	m_window.draw(text);

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape))
		m_state = State::RESULTS;

	constexpr size_t LAPS = 3;
	if (std::ranges::all_of(m_players, [](Player const& p) {return p.getCompletedLaps() >= LAPS; }))
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