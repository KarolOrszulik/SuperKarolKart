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
#include "UIElementFactory.h"
#include "UIImage.h"

#include "Kart.h"
#include "Motorcycle.h"
#include "Hovercraft.h"

#include <iostream>
#include <future>
#include <thread>

std::shared_ptr<Engine> Engine::s_instance = nullptr;

float operator ""_vh(long double heightVh)
{
	unsigned windowHeight = Engine::getInstance()->m_window.getSize().y;
	return static_cast<float>(heightVh / 100.0 * windowHeight);
}

float operator ""_vw(long double widthVw)
{
	unsigned windowWidth = Engine::getInstance()->m_window.getSize().x;
	return static_cast<float>(widthVw / 100.0 * windowWidth);
}

unsigned operator""_vh(unsigned long long heightVh)
{
	unsigned windowHeight = Engine::getInstance()->m_window.getSize().y;
	return static_cast<unsigned>(heightVh / 100.0 * windowHeight);
}

unsigned operator""_vw(unsigned long long widthVw)
{
	unsigned windowWidth = Engine::getInstance()->m_window.getSize().x;
	return static_cast<unsigned>(widthVw / 100.0 * windowWidth);
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
			else if (event.type == sf::Event::Resized) resetWindowView();
			else if(m_menus.contains(m_state)) m_menus[m_state].handleEvent(event);
		}

		m_window.clear(m_track.getBackgroundColor());

		onUpdate(clock.restart().asSeconds()); // reset clock and pass delta time in seconds

		m_window.display();
	}
}

void Engine::resetWindowView()
{
	sf::Vector2f newSize(m_window.getSize());

	sf::View view(sf::FloatRect({ 0.0, 0.0 }, newSize));
	m_window.setView(view);
	m_menus.erase(m_state);
}

const sf::Texture& Engine::getTexture(const std::string& name)
{
	if (!m_textures.contains(name)) 
	{
		std::cout << "Wczytuje teksture: " << name << std::endl;

		static sf::Uint8 checkboard[] = {
			  0,  0,   0, 255, 128,  0, 128, 255,  0,  0,   0, 255,
			128,  0, 128, 255,   0,  0,   0, 255,128,  0, 128, 255,
			  0,  0,   0, 255, 128,  0, 128, 255,  0,  0,   0, 255,
		};
		sf::Image img;
		img.create(3, 3, checkboard);

		m_textures[name].loadFromImage(img);
	}
	return m_textures.at(name);
}

void Engine::onStart()
{
	m_state = State::MAIN_MENU;
	m_assetsPath = "assets/";
	std::filesystem::path texturesPath = m_assetsPath / "textures";
	std::filesystem::path fontsPath = m_assetsPath / "fonts";
	
	if (!std::filesystem::exists(m_assetsPath))
		throw std::runtime_error("Assets folder could not be found");

	if(!std::filesystem::exists(texturesPath))
		throw std::runtime_error("Textures folder could not be found");

	if(!std::filesystem::exists(fontsPath))
		throw std::runtime_error("Fonts folder could not be found");

	std::jthread texturesLoaderThread([this, texturesPath]() { loadTextures(texturesPath); });
	std::jthread fontsLoaderThread([this, fontsPath]() { loadFonts(fontsPath); });
}

void Engine::loadTextures(std::filesystem::path const& path)
{
	for (auto& p : std::filesystem::directory_iterator(path))
	{
		std::string path = p.path().string();
		std::string name = p.path().stem().string();
		if (!m_textures[name].loadFromFile(path))
			throw std::runtime_error("Texture " + name + " could not be loaded");
	}
}

void Engine::loadFonts(std::filesystem::path const& path)
{
	for (auto& p : std::filesystem::directory_iterator(path))
	{
		std::string path = p.path().string();
		std::string name = p.path().stem().string();
		std::cout << path << std::endl;
		if (!m_fonts[name].loadFromFile(path))
			throw std::runtime_error("Font" + name + "could not be loaded");
	}
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
	case State::VEHICLE_MENU:
		stateVehicleMenu(dt);
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
	if(m_gameSettings.drawFPS)
		drawFPS(dt);
}

void Engine::populatePlayers()
{
	m_players.clear();

	for (int i = 0; i < m_gameSettings.numPlayers; i++)
	{
		std::unique_ptr<Vehicle> vehicle;
		sf::Vector2f startingPos = m_track.getPlayerStartingPos(i);
		switch (m_gameSettings.vehicle[i])
		{
			case 1:
				vehicle = std::make_unique<Kart>(getTexture("kart"), &m_track, startingPos);
				break;
			case 2:
				vehicle = std::make_unique<Motorcycle>(getTexture("motorcycle"), &m_track, startingPos);
				break;
			case 3:
				vehicle = std::make_unique<Hovercraft>(getTexture("hovercraft"), &m_track, startingPos);
				break;
		}

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
		UIButton::Style normStyle, hovStyle, selStyle;
		normStyle.bgColor   =	{   0,   0,   0,   0 };
		normStyle.fontColor =	{ 255, 255, 255, 255 };
		hovStyle.fontColor  =	{ 255, 200,   0, 255 };
		selStyle.fontColor  =	{ 255,   0,   0, 255 };

		UIElementFactory textFactory(normStyle, normStyle, normStyle, getFont("SKK"), 15_vh);
		UIElementFactory btnFactory(normStyle, hovStyle, selStyle, getFont("SKK"), 10_vh);

		Menu& menu = m_menus[State::MAIN_MENU];
		menu.setBgColor({ 20,170,150,255 });
		menu.setSize({ (float)m_window.getSize().x, (float)m_window.getSize().y });

		//auto img = std::make_shared<UIImage>(normStyle, getTexture("kart"));
		auto img = textFactory.makeImgPtr(getTexture("kart"));
		menu.addElement(img);

		// <---- Title ---->
		auto title = textFactory.makeBtnPtr("Super Karol Kart", { 0, 5.0_vh });
		title->centerHorizontally(menu.getWidth());

		menu.addElement(title);

		// <---- Start Button ---->
		auto btnPlay = btnFactory.makeBtnPtr("PLAY", { 0, 35.0_vh });
		btnPlay->centerHorizontally(menu.getWidth());
		btnPlay->onRelease = [this]() { 
			m_gameSettings = Settings();
			setGameState(State::SETUP_MENU); 
		};

		menu.addElement(btnPlay);

		// <---- Exit Button ---->
		auto btnExit = btnFactory.makeBtnPtr("EXIT", { 0, 60.0_vh });
		btnExit->centerHorizontally(menu.getWidth());
		btnExit->onRelease = [this]() { m_window.close(); };

		menu.addElement(btnExit);
	}
	m_window.draw(m_menus[State::MAIN_MENU]);
}

void Engine::stateSetup(float dt)
{
	if (!m_menus.contains(State::SETUP_MENU))
	{
		UIButton::Style normStyle, hovStyle, selStyle;
		normStyle.bgColor   = {   0,   0,   0,   0 };
		normStyle.fontColor = { 255, 255, 255, 255 };
		hovStyle.fontColor  = { 255, 200,   0, 255 };
		selStyle.fontColor  = { 255,   0,   0, 255 };

		Menu& menu = m_menus[State::SETUP_MENU];
		menu.setBgColor({ 20,170,150,255 });
		menu.setSize({ (float)m_window.getSize().x, (float)m_window.getSize().y });

		// <---- Button factory ---->

		UIElementFactory textFactory(normStyle, normStyle, normStyle, getFont("SKK"), 15_vh);
		UIElementFactory btnFactory(normStyle, hovStyle, selStyle, getFont("SKK"), 5_vh);
		
		// <---- Title ---->
		auto title = textFactory.makeBtnPtr("Super Karol Kart", { 0, 5.0_vh });
		title->centerHorizontally(menu.getWidth());
		menu.addElement(title);

		//<---- Player Number Selection ---->
		auto playerNumSelect = std::make_shared<UIRadioGroup>();
		
		std::vector<std::shared_ptr<UIToggleButton>> playerNumBtns{
			btnFactory.makeTogglePtr("1 Player", { 5.0_vw, 30.0_vh }),
			btnFactory.makeTogglePtr("2 Players", { 5.0_vw, 45.0_vh }),
			btnFactory.makeTogglePtr("3 Players", { 5.0_vw, 60.0_vh }),
			btnFactory.makeTogglePtr("4 Players", { 5.0_vw, 75.0_vh })
		};
		
		for (int i = 0; i < playerNumBtns.size(); i++) {
			playerNumBtns[i]->setOnSelected(
				[this, i]() { m_gameSettings.numPlayers = i + 1; }
			);
		}
		playerNumSelect->addElements(playerNumBtns);

		if(m_gameSettings.numPlayers == -1)
			playerNumSelect->setActiveElement(0);
		else
			playerNumSelect->setActiveElement(m_gameSettings.numPlayers - 1);

		menu.addElement(playerNumSelect);


		////<---- Track selection ---->
		auto trackSelection = std::make_shared<UIRadioGroup>();

		std::vector<std::shared_ptr<UIToggleButton>> trackBtns{
			btnFactory.makeTogglePtr("Szombierki", { 50.0_vw, 30.0_vh }),
			btnFactory.makeTogglePtr("Opole", { 50.0_vw, 45.0_vh }),
			btnFactory.makeTogglePtr("Spa", { 50.0_vw, 60.0_vh })
		};

		trackBtns[0]->setOnSelected([this]() {
			m_gameSettings.trackName = "track_00.txt";
			std::cout << m_gameSettings.trackName << std::endl;
		});
		trackBtns[1]->setOnSelected([this]() {
			m_gameSettings.trackName = "track_01.txt";
			std::cout << m_gameSettings.trackName << std::endl;
		});
		trackBtns[2]->setOnSelected([this]() {
			m_gameSettings.trackName = "track_02.txt";
			std::cout << m_gameSettings.trackName << std::endl;
		});

		trackSelection->addElements(trackBtns);
		menu.addElement(trackSelection);


		// <---- Start Button ---->
		auto btnGO = btnFactory.makeBtnPtr("START!", { 0, 90.0_vh });
		btnGO->centerHorizontally(menu.getWidth());
		btnGO->onRelease = [this]() {
			if (m_gameSettings.numPlayers > 0) {
				setGameState(State::VEHICLE_MENU);
			}
		};

		menu.addElement(btnGO);
	}
	m_window.draw(m_menus[State::SETUP_MENU]);
}

void Engine::stateVehicleMenu(float dt)
{
	if (!m_menus.contains(State::VEHICLE_MENU))
	{
		Menu& menu = m_menus[State::VEHICLE_MENU];
		menu.setBgColor({ 20,170,150,255 });
		menu.setSize(
			{ (float)m_window.getSize().x, (float)m_window.getSize().y });

		// <---- Styles ---->
		UIButton::Style normStyle, hovStyle, selStyle;
		normStyle.bgColor   = {   0,   0,   0,   0 };
		normStyle.fontColor = { 255, 255, 255, 255 };
		hovStyle.fontColor  = { 255, 200,   0, 255 };
		selStyle.fontColor  = { 255,   0,   0, 255 };

		UIButton::Style inpTxtNorm, inpTxtHov, inpTxtSel;
		inpTxtNorm.bgColor   = {   0,   0, 200, 255 };
		inpTxtNorm.fontColor = { 255, 255, 255, 255 };
		inpTxtHov.bgColor    = {   0, 200,   0, 255 };
		inpTxtHov.fontColor  = { 255, 255, 255, 255 };
		inpTxtSel.bgColor    = { 200,   0,   0, 255 };
		inpTxtSel.fontColor  = { 255, 255, 255, 255 };


		// <---- Button factory ---->
		UIElementFactory txtFactory(normStyle, normStyle, normStyle, getFont("SKK"), 15_vh);
		UIElementFactory btnFactory(normStyle, hovStyle, selStyle, getFont("SKK"), 5_vh);
		UIElementFactory inpFactory(inpTxtNorm, inpTxtHov, inpTxtSel, getFont("SKK"), 5_vh);

		// <---- Title ---->
		auto title = txtFactory.makeBtnPtr("Super Karol Kart", { 0, 5._vh });
		title->centerHorizontally(menu.getWidth());
		menu.addElement(title);

		// <---- Player Number Selection ---->

		txtFactory.setCharacterSize(5_vh);
		for (int i = 0; i < m_gameSettings.numPlayers; i++)
		{
			// preparing button info
			std::string playerName = "Player " + std::to_string(i + 1) + ":";
			float columnPosition = 3._vw + 24._vw * i;

			// creating toggle button for vehicle
			auto playerDesc = txtFactory.makeBtnPtr(playerName, { columnPosition, 30._vh });
			menu.addElement(playerDesc);

			// preparing radio group
			auto vehicleSelect = std::make_shared<UIRadioGroup>();

			std::vector<std::shared_ptr<UIToggleButton>> vehicleBtns = {
				btnFactory.makeTogglePtr("Kart", { columnPosition, 45.0_vh }),
				btnFactory.makeTogglePtr("Motorcycle", { columnPosition, 53.0_vh }),
				btnFactory.makeTogglePtr("Hovercraft", { columnPosition, 61.0_vh })
			};

			vehicleBtns[0]->setOnSelected([this, i]() { m_gameSettings.vehicle[i] = 1; });
			vehicleBtns[1]->setOnSelected([this, i]() { m_gameSettings.vehicle[i] = 2; });
			vehicleBtns[2]->setOnSelected([this, i]() { m_gameSettings.vehicle[i] = 3; });
			vehicleSelect->addElements(vehicleBtns);

			if (m_gameSettings.vehicle[i])
				vehicleSelect->setActiveElement(m_gameSettings.vehicle[i] - 1);
			else
				vehicleSelect->setActiveElement(0);


			menu.addElement(vehicleSelect);

			// adding text input for player name
			auto textInput = inpFactory.makeTxtInpPtr("",{ columnPosition, 75._vh }, "ENTER NAME", 9);
			textInput->onTextEntered = [this, i](std::string const& text) {
				m_gameSettings.playerNames[i] = text;
			};
			textInput->applyPadding(1_vh);
			menu.addElement(textInput);
		}

		// change in button factory
		btnFactory.setCharacterSize(7_vh);

		// <---- Start Button ---->
		auto btnGO = btnFactory.makeBtnPtr("START!", 
			{ 90._vw, 90._vh }, UIElement::Origin::TOP_RIGHT);
		btnGO->onRelease = [this]() {
			bool vehiclesSelected = true;
			for (int i = 0; i < m_gameSettings.numPlayers; i++)
				vehiclesSelected &= m_gameSettings.vehicle[i] != 0;

			if(vehiclesSelected)
				setGameState(State::PRE_RACE); 
		};
		menu.addElement(btnGO);

		// <---- Start Button ---->
		auto btnBack = btnFactory.makeBtnPtr("BACK", { 10.0_vw, 90.0_vh });
		btnBack->onRelease = [this]() { setGameState(State::SETUP_MENU); };
		menu.addElement(btnBack);
	}
	m_window.draw(m_menus[State::VEHICLE_MENU]);
}

void Engine::statePreRace(float dt)
{
	m_objects.clear();
	std::string trackPath = (m_assetsPath / "track").string();
	m_track.loadTilemap(trackPath + "/track_tileset.png");
	//m_track.loadTrack(trackPath + "/" + m_gameSettings.trackName);
	m_track.loadTrack(trackPath + "/small.txt");


	sf::Vector2u worldSize(m_track.getSize());
	m_world.create(worldSize.x, worldSize.y);

	updateAllObjects(dt); // 
	updateAllObjects(dt); // niestety musi byæ 2 razy

	populatePlayers();
	for(int i = 0; i < getNumPlayers(); i++)
		m_players[i].setName(m_gameSettings.playerNames[i]);

	m_raceTime = -3.f;
	setGameState(State::RACE);
}

void Engine::stateRace(float dt)
{
	m_raceTime += dt;

	// collect player input in parallel
	std::vector<std::future<void>> futures;
	for (auto& player : m_players)
		futures.push_back(std::async(std::launch::async, &Player::controlVehicle, &player));

	for (auto& f : futures)
		f.get();

	// update all objects if the race has begun
	if (raceHasStarted())
		updateAllObjects(dt);

	// draw the track and all objects to the world
	m_world.clear(m_track.getBackgroundColor());
	m_track.draw(m_world);
	drawAllObjects();

	// draw the world from each player's perspective
	for (auto& player : m_players)
		player.drawPlayerScreen(m_world, m_window, dt);

	// draw the UI elements for initial countdown
	displayCountdown();

	// update race completion times
	for (auto& player : m_players)
		if (player.getCompletedLaps() >= m_gameSettings.laps && player.getFinishTime() < 0.1f)
			player.setFinishTime(m_raceTime);

	// early exit
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape))
		m_state = State::MAIN_MENU;

	// proper exit
	if (allPlayersFinished())
		setGameState(State::RESULTS);
}

void Engine::displayCountdown()
{
	UIButton::Style normStyle;
	normStyle.fontOutlineThickness = 2._vh;
	UIElementFactory factory(normStyle, getFont("SKK"), 20_vh);
	UIButton txt;
	if (m_raceTime < 0.f)
	{
		auto time = std::to_string(int(-m_raceTime + 1));
		txt = factory.makeBtn(time, { 50._vw, 50._vh}, UIElement::Origin::MID_CENTER);
	}
	else if (m_raceTime < 1.f )
	{
		txt = factory.makeBtn("GO!", { 50._vw, 50._vh }, UIElement::Origin::MID_CENTER);
	}
	if (getNumPlayers() == 1)
		txt.setPosition({ 50._vw, 25._vh });
	txt.draw(m_window, {});
}


void Engine::updateAllObjects(float dt)
{
	for (auto& obj : m_objects)
		obj->update(dt);

	addObjectsToAdd();
	removeObjectsForRemoval();
}

void Engine::drawAllObjects()
{
	for (auto& obj : m_objects)
		obj->draw(m_world);
	m_world.display();
}

void Engine::addObjectsToAdd()
{
	for (auto& obj : m_objectsToAdd)
		m_objects.emplace(std::move(obj));
	m_objectsToAdd.clear();
}

void Engine::removeObjectsForRemoval()
{
	for (auto& obj : m_objectsToRemove)
	{
		auto it = std::ranges::find_if(m_objects, [obj](auto& p) { return p.get() == obj; });
		if (it != m_objects.end())
			m_objects.erase(it);
	}
	m_objectsToRemove.clear();
}

bool Engine::allPlayersFinished() const
{
	return std::ranges::all_of(m_players, 
		[this](Player const& p) { 
			return p.getCompletedLaps() >= m_gameSettings.laps; 
		}
	);
}

void Engine::stateResults(float dt)
{
	if (!m_menus.contains(State::RESULTS))
	{
		struct Result
		{
			float time;
			Player* player;
		};

		std::vector<Result> finishTimes;

		for (int i = 0; i < m_players.size(); i++)
		{
			finishTimes.push_back({ m_players[i].getFinishTime(), &m_players[i] });
		}

		std::ranges::sort(finishTimes, {}, &Result::time);
		UIButton::Style normStyle, hovStyle, selStyle;
		normStyle.bgColor   = {   0,   0,   0,   0 };
		normStyle.fontColor = { 255, 255, 255, 255 };
		hovStyle.fontColor  = { 255, 200,   0, 255 };
		selStyle.fontColor  = { 255,   0,   0, 255 };

		UIElementFactory factory(normStyle, getFont("SKK"), 15_vh);
		UIElementFactory factoryBtn(normStyle, hovStyle, selStyle, getFont("SKK"), 10_vh);


		Menu& menu = m_menus[State::RESULTS];
		menu.setBgColor({ 20,170,150,255 });
		menu.setSize({ (float)m_window.getSize().x, (float)m_window.getSize().y });

		// <---- Title ---->
		auto title = factory.makeBtnPtr("Super Karol Kart", { 0, 5.0_vh });
		title->centerHorizontally(menu.getWidth());
		menu.addElement(title);

		// <---- Results ---->

		factory.setCharacterSize(8_vh);
		for (int i = 0; i < finishTimes.size(); i++)
		{
			auto& [time, player] = finishTimes[i];

			const int minutes = std::floor(time / 60.f);
			const int seconds = std::fmod(time, 60.f);
			const int hundredths = std::fmod(time, 1.f) * 100;
			const char* leadingZero = seconds < 10 ? "0" : "";

			std::string timeStr = std::to_string(minutes) + ":" + leadingZero + std::to_string(seconds) + "." + std::to_string(hundredths);
			std::string place = "#" + std::to_string(i + 1) + ": ";
			std::string result = place + player->getName() + " finished in " + timeStr;

			auto resultText = factory.makeBtnPtr(result, { 0, 30.0_vh + 10.0_vh * i });
			resultText->centerHorizontally(menu.getWidth());
			menu.addElement(resultText);
		}

		// <---- Start Button ---->
		auto btnGO = factoryBtn.makeBtnPtr("MAIN MENU", { 0, 90.0_vh });
		btnGO->centerHorizontally(menu.getWidth());
		btnGO->onRelease = [this]() { setGameState(State::MAIN_MENU); };
		menu.addElement(btnGO);
	}
	m_window.draw(m_menus[State::RESULTS]);
}


void Engine::drawFPS(float dt)
{
	m_fpsCounter.update(dt, 1.f / dt);

	UIButton::Style normStyle;
	normStyle.fontOutlineThickness = 2.f;

	UIElementFactory factory(normStyle, getFont("SKK"), 3_vh);
	
	auto fps = std::to_string(static_cast<int>(m_fpsCounter.getValue()));
	UIButton btn = factory.makeBtn(fps + " FPS");
	btn.draw(m_window, {});
}