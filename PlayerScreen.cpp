#include "PlayerScreen.h"
#include "Engine.h"
#include "UIElementFactory.h"
#include <cmath>

PlayerScreen::PlayerScreen(int number) : m_number(number)
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
	static float a = 0.f;
	auto center = player.m_vehicle->getPosition();
	auto angle = player.m_vehicle->getAngle();

	a = std::lerp(a, angle, 1 - std::exp(-dt * 5.f));
	float newsin = std::lerp(std::sin(0), std::sin(angle), 1 - std::exp(-dt * 5.f));
	float newcos = std::lerp(std::cos(0), std::cos(angle), 1 - std::exp(-dt * 5.f));
	a = std::atan2(newsin, newcos);

	calculateSizeAndViewport();

	sf::View view(center, m_size);
	view.setViewport(m_viewport);
	view.zoom(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Z) ? 0.25f : 0.5f); // to zmieni� na zale�ne od pr�dko�ci jak ju� b�dzie co� takiego jak pr�dko��
	//view.rotate(angle * 180.f / 3.1415f + 90.f); // sztywna rotacja
	view.rotate(a * 180.f / 3.1415f + 90.f); // interpolowana rotacja
	target.setView(view);
	sf::Sprite s(source.getTexture());
	target.draw(s);

	Engine::getInstance()->resetWindowView();


	float vh = target.getSize().y / 100.0f;
	m_speedStyle.fontOutlineThickness = 1 * vh;
	float x = (target.getSize().x * (m_viewport.width + m_viewport.left)) * 0.98f;
	float y = (target.getSize().y * (m_viewport.height + m_viewport.top)) * 0.98f;
	UIElementFactory factory(m_speedStyle, m_speedStyle, m_speedStyle, 
		Engine::getInstance()->m_font, 7 * vh);
	auto speed = std::abs(std::roundf(player.m_vehicle->getSpeed()));
	auto speedInt = static_cast<int>(speed);
	auto speedDisp = factory.makeBtn(std::to_string(speedInt) + "km/h", {x,y}, UIElement::Origin::BOT_RIGHT);
	speedDisp.draw(target, {});


}

sf::Vector2f PlayerScreen::calculateSpeedPos()
{
	return sf::Vector2f();
}