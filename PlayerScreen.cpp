#include "PlayerScreen.h"
#include "Engine.h"
#include "UIElementFactory.h"
#include <cmath>

PlayerScreen::PlayerScreen(int number) 
	: m_number(number), m_screenAngle(0.f), m_speedCounter(0.25f)
{
	if (number < 0 || number > 3)
		throw std::invalid_argument("Player number must be in range [0, 3]");

	m_speedStyle.bgColor = sf::Color::Transparent;
	m_speedStyle.fontColor = sf::Color::White;
	m_speedStyle.bgColor = sf::Color::Transparent;
	m_speedStyle.fontOutlineColor = sf::Color::Black;
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
	const Vehicle& vehicle = *(player.m_vehicle);
	Engine& engine = *Engine::getInstance();
	auto center = vehicle.getPosition();
	auto angle = vehicle.getAngle();
	auto speed = std::abs(std::roundf(vehicle.getSpeed()));


	float newsin = static_cast<float>(
		std::lerp(std::sin(m_screenAngle), std::sin(angle), 1 - std::exp(-dt * 5.f)));
	float newcos = static_cast<float>(
		std::lerp(std::cos(m_screenAngle), std::cos(angle), 1 - std::exp(-dt * 5.f)));
	m_screenAngle = std::atan2(newsin, newcos);

	calculateSizeAndViewport();

	sf::View view(center, m_size);
	view.setViewport(m_viewport);
	float zoom = 0.25f * (1 + (speed / vehicle.getMaxSpeed()));
	view.zoom(zoom); // to zmieniæ na zale¿ne od prêdkoœci jak ju¿ bêdzie coœ takiego jak prêdkoœæ
	//view.rotate(angle * 180.f / 3.1415f + 90.f); // sztywna rotacja
	view.rotate(m_screenAngle * 180.f / 3.1415f + 90.f); // interpolowana rotacja
	target.setView(view);
	target.draw(sf::Sprite(source.getTexture()));

	engine.resetWindowView();


	float vh = target.getSize().y / 100.0f;

	sf::Vector2f offset = (m_viewport.getSize() + m_viewport.getPosition());
	sf::Vector2f pos = { offset.x * target.getSize().x, offset.y * target.getSize().y };

	m_speedCounter.update(dt, speed);
	auto speedInt = static_cast<int>(m_speedCounter.getValue());
	std::string speedStr = std::to_string(speedInt) + "km/h";

	sf::Uint8 whiteLevel = 255 * (1.f - std::min(speed / 200.f, 1.f));
	m_speedStyle.fontColor = { 255 , whiteLevel, whiteLevel, 255 };
	m_speedStyle.fontOutlineThickness = 1 * vh;

	UIElementFactory factory(m_speedStyle, engine.getFont("SKK"), 7 * vh);
	auto speedDisp = factory.makeBtn(speedStr, pos, UIElement::Origin::BOT_RIGHT);
	speedDisp.draw(target, {});

}

sf::Vector2f PlayerScreen::calculateSpeedPos()
{
	return sf::Vector2f();
}