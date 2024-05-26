#include "Player.h"
#include "Vehicle.h"

Player::Player(ControlScheme control, int player_number)
	: m_control(control), m_screen(player_number)
{}

void Player::drawPlayerScreen(sf::RenderTexture& source, sf::RenderTarget& target)
{
	m_screen.draw(source, target, m_vehicle->getPosition());
}

void Player::controlVehicle()
{
	if (m_vehicle == nullptr)
		return;

	struct KeyMapping
	{
		sf::Keyboard::Key up;
		sf::Keyboard::Key down;
		sf::Keyboard::Key left;
		sf::Keyboard::Key right;
	};

	static const std::map<Player::ControlScheme, KeyMapping> keyMappings
	{
		{ Player::ControlScheme::WASD,   { sf::Keyboard::Key::W,  sf::Keyboard::Key::S,    sf::Keyboard::Key::A,    sf::Keyboard::Key::D } },
		{ Player::ControlScheme::ARROWS, { sf::Keyboard::Key::Up, sf::Keyboard::Key::Down, sf::Keyboard::Key::Left, sf::Keyboard::Key::Right } },
		{ Player::ControlScheme::IJKL,   { sf::Keyboard::Key::I,  sf::Keyboard::Key::K,    sf::Keyboard::Key::J,    sf::Keyboard::Key::L } }
	};
	// to siê a¿ prosi o refaktoring, ale zostawiam dla czytelnoœci

	auto mapping = keyMappings.at(m_control);

	if (sf::Keyboard::isKeyPressed(mapping.up))
		m_vehicle->applyAccelerator(1.f);

	if (sf::Keyboard::isKeyPressed(mapping.down))
		m_vehicle->applyAccelerator(-1.f);

	if (sf::Keyboard::isKeyPressed(mapping.left))
		m_vehicle->applySteering(-1.f);

	if (sf::Keyboard::isKeyPressed(mapping.right))
		m_vehicle->applySteering(1.f);
}