#include "Engine.h"

#include "Player.h"
#include "Vehicle.h"
#include "UIElement.h"
#include "Menu.h"
#include "UIButton.h"
#include "UIToggleButton.h"
#include "UIRadioGroup.h"


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
		title.setCharacterSize(48);
		title.shrinkSizeToText();
		title.centerHorizontally(m_mainMenu.getWidth());

		m_mainMenu.addElement(make_unique<UIButton>(title));

		// <---- Player Number Selection ---->
		auto playerNumSelect = std::make_unique<UIRadioGroup>();

		UIToggle onePlayerBtn(normStyle, hovStyle, selStyle);
		onePlayerBtn.setPosition({ 5.0_vw, 25.0_vh });
		onePlayerBtn.setText("1 Player");
		onePlayerBtn.setFont(m_font);
		onePlayerBtn.setCharacterSize(24);
		onePlayerBtn.shrinkSizeToText();
		onePlayerBtn.setOnSelected([this]() { populatePlayers(1); });

		UIToggle twoPlayerBtn(onePlayerBtn);
		twoPlayerBtn.setPosition({ 5.0_vw, 40.0_vh});
		twoPlayerBtn.setText("2 Players");
		twoPlayerBtn.setOnSelected([this]() { populatePlayers(2);});

		UIToggle threePlayerBtn(twoPlayerBtn);
		threePlayerBtn.setPosition({ 5.0_vw, 55.0_vh });
		threePlayerBtn.setText("3 Players");
		threePlayerBtn.setOnSelected([this]() { populatePlayers(3); });

		UIToggle fourPlayerBtn(threePlayerBtn);
		fourPlayerBtn.setPosition({ 5.0_vw, 70.0_vh });
		fourPlayerBtn.setText("4 Players");
		fourPlayerBtn.setOnSelected([this]() { populatePlayers(4); });

		playerNumSelect->addElement(make_unique<UIToggle>(onePlayerBtn));
		playerNumSelect->addElement(make_unique<UIToggle>(twoPlayerBtn));
		playerNumSelect->addElement(make_unique<UIToggle>(threePlayerBtn));
		playerNumSelect->addElement(make_unique<UIToggle>(fourPlayerBtn));

		m_mainMenu.addElement(move(playerNumSelect));

		// <---- Start Button ---->
		UIButton btnGO(onePlayerBtn);
		btnGO.setPosition({ 250,50 });
		btnGO.setText("START!");
		btnGO.setFont(m_font);
		btnGO.setCharacterSize(24);
		btnGO.shrinkSizeToText();
		btnGO.onRelease = [this]() {
			if (getNumPlayers() > 0) {
				setGameState(State::SETUP);
			} 
		};
		
		m_mainMenu.addElement(make_unique<UIButton>(btnGO));
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