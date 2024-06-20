#include "PlayerScreen.h"
#include "Engine.h"
#include "UIElementFactory.h"
#include <cmath>

PlayerScreen::PlayerScreen(int number) 
	: m_number(number), m_screenAngle(0.f), m_speedCounter(0.25f)
{
	if (number < 0 || number > 3)
		throw std::invalid_argument("Player number must be in range [0, 3]");
}

void PlayerScreen::calculateSizeAndViewport()
{
	m_size = { 0.f, 0.f };
	m_viewport = { 0.f, 0.f, 1.f, 1.f };

	auto engine = Engine::getInstance();
	sf::Vector2u windowSize = engine->getWindowSize();
	m_size = { static_cast<float>(windowSize.x), static_cast<float>(windowSize.y) };

	if (engine->getNumPlayers() >= 2)
	{
		m_size.x /= 2.f;
		m_viewport.width /= 2.f;
	}

	if (engine->getNumPlayers() >= 3)
	{
		m_size.y /= 2.f;
		m_viewport.height /= 2.f;
	}

	if (m_number % 2 == 1)
		m_viewport.left += 0.5f;

	if (m_number >= 2)
		m_viewport.top += 0.5f;
}



void PlayerScreen::draw(sf::RenderTexture& source, sf::RenderTarget& target, const Player& player, float dt)
{
	constexpr float ANGLE_INTERPOLATION_SPEED = 5.f;
	constexpr float ZOOM_INTERPOLATION_SPEED = 2.f;

	const Vehicle& vehicle = *(player.m_vehicle);
	Engine& engine = *Engine::getInstance();
	auto center = vehicle.getPosition();
	auto angle = vehicle.getAngle();
	auto speed = std::abs(std::round(vehicle.getSpeed()));


	float newsin = static_cast<float>(
		std::lerp(std::sin(m_screenAngle), std::sin(angle), 1 - std::exp(-dt * ANGLE_INTERPOLATION_SPEED)));
	float newcos = static_cast<float>(
		std::lerp(std::cos(m_screenAngle), std::cos(angle), 1 - std::exp(-dt * ANGLE_INTERPOLATION_SPEED)));
	m_screenAngle = std::atan2(newsin, newcos);

	calculateSizeAndViewport();

	sf::View view(center, m_size);
	view.setViewport(m_viewport);

	const float SLOPE = (ZOOM_MAX - ZOOM_MIN) / (ZOOM_SPEED_END - ZOOM_SPEED_START);
	const float TARGET_ZOOM = std::clamp(SLOPE * (speed - ZOOM_SPEED_START) + ZOOM_MIN, ZOOM_MIN, ZOOM_MAX);

	m_zoom = std::lerp(m_zoom, TARGET_ZOOM, 1 - std::exp(-dt * ZOOM_INTERPOLATION_SPEED));

	view.zoom(m_zoom); // to zmieniæ na zale¿ne od prêdkoœci jak ju¿ bêdzie coœ takiego jak prêdkoœæ
	//view.rotate(angle * 180.f / 3.1415f + 90.f); // sztywna rotacja
	view.rotate(m_screenAngle * 180.f / 3.1415f + 90.f); // interpolowana rotacja
	target.setView(view);
	target.draw(sf::Sprite(source.getTexture()));

	engine.resetWindowView();


	sf::Vector2f windowSize(target.getSize());
	float vh = windowSize.y / 100.0f;


	UIButton speedDisplay = generateSpeedDisplay(speed, vh, dt);
	speedDisplay.setPosition(getCornerPosition(Corner::BOT_RIGHT, windowSize));
	speedDisplay.draw(target, {});

	UIImage itemImage = generateItemImage(vehicle, vh);
	itemImage.setPosition(getCornerPosition(Corner::TOP_LEFT, windowSize));
	itemImage.draw(target, {});

	UIButton lapCounter = generateLapCounter(player, vh);
	lapCounter.setPosition(getCornerPosition(Corner::BOT_LEFT, windowSize));
	lapCounter.draw(target, {});
}

UIButton PlayerScreen::generateSpeedDisplay(float speed, float vh, float dt)
{
	m_speedCounter.update(dt, speed);

	auto speedInt = static_cast<int>(m_speedCounter.getValue());
	std::string speedStr = std::to_string(speedInt) + "km/h";

	auto engine = Engine::getInstance();
	const sf::Font& font = engine->getFont("SKK");

	UIButton::Style speedStyle;
	sf::Uint8 whiteLevel = static_cast<int>(255 * (1.f - std::min(speed / 200.f, 1.f)));
	speedStyle.fontColor = { 255 , whiteLevel, whiteLevel, 255 };
	speedStyle.fontOutlineThickness = 1 * vh;

	UIElementFactory factory(speedStyle, font, static_cast<int>(5 * vh));
	return factory.makeBtn(speedStr, {}, UIElement::Origin::BOT_RIGHT);
}

UIButton PlayerScreen::generateLapCounter(const Player& player, float vh)
{
	auto engine = Engine::getInstance();
	const sf::Font& font = engine->getFont("SKK");
	size_t playerLaps = player.getCompletedLaps() + 1;
	size_t totalLaps = engine->getNumLaps();
	
	UIButton::Style lapStyle;
	lapStyle.fontOutlineThickness = 1 * vh;

	std::string lapCount = "Lap " +
		std::to_string(playerLaps) 
		+ " / "  
		+ std::to_string(totalLaps);

	UIElementFactory factory(lapStyle, font, static_cast<int>(5 * vh));
	return factory.makeBtn(lapCount, {}, UIElement::Origin::BOT_LEFT);
}

UIImage PlayerScreen::generateItemImage(const Vehicle& vehicle, float vh)
{
	auto engine = Engine::getInstance();
	constexpr float paddingPercent = 2.f;
	constexpr float sizePercent = 6.f;
	const sf::Font& font = engine->getFont("SKK");

	UIButton::Style imageStyle;
	imageStyle.bgColor = {255, 255, 255, 150};
	imageStyle.bgOutlineColor = sf::Color::Black;
	imageStyle.bgOutlineThickness = -0.75f * vh;

	UIElementFactory imageFactory(imageStyle, font, 0);
	return imageFactory.makeImg(
		vehicle.getItemTexture(),
		{}, UIElement::Origin::TOP_LEFT,
		paddingPercent * vh, 
		sizePercent * sf::Vector2f{ vh, vh });
}

sf::Vector2f PlayerScreen::getCornerPosition(
	Corner corner, 
	sf::Vector2f targetSize) const
{
	sf::Vector2f leftCorner = m_viewport.getPosition();
	
	if(corner == Corner::BOT_LEFT || corner == Corner::BOT_RIGHT)
		leftCorner += {0.f, m_viewport.getSize().y};
	
	if(corner == Corner::TOP_RIGHT || corner == Corner::BOT_RIGHT)
		leftCorner += {m_viewport.getSize().x, 0.f};
	
	return {leftCorner.x * targetSize.x, leftCorner.y * targetSize.y};
}
