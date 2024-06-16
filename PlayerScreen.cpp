#include "PlayerScreen.h"
#include "Engine.h"

PlayerScreen::PlayerScreen(int number) : m_number(number)
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

void PlayerScreen::draw(sf::RenderTexture& source, sf::RenderTarget& target, sf::Vector2f center)
{
	calculateSizeAndViewport();

	sf::View view(center, m_size);
	view.setViewport(m_viewport);
	view.zoom(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Z) ? 0.25f : 1.f); // to zmieniæ na zale¿ne od prêdkoœci jak ju¿ bêdzie coœ takiego jak prêdkoœæ
	target.setView(view);
	target.draw(sf::Sprite(source.getTexture()));
	target.setView(target.getDefaultView());
}