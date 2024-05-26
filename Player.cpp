#include "Player.h"
#include "Vehicle.h"

Player::Player(ControlScheme control, int player_number, int total_players, sf::Vector2u window_size)
	: m_control(control)
{

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

	std::map<Player::ControlScheme, KeyMapping> keyMappings
	{
		{ Player::ControlScheme::WASD,   { sf::Keyboard::Key::W,  sf::Keyboard::Key::S,    sf::Keyboard::Key::A,    sf::Keyboard::Key::D } },
		{ Player::ControlScheme::ARROWS, { sf::Keyboard::Key::Up, sf::Keyboard::Key::Down, sf::Keyboard::Key::Left, sf::Keyboard::Key::Right } }
	};
	// to siê a¿ prosi o refaktoring, ale zostawiam dla czytelnoœci

	auto mapping = keyMappings[m_control];

	if (sf::Keyboard::isKeyPressed(mapping.up))
		m_vehicle->applyAccelerator(1.f);

	if (sf::Keyboard::isKeyPressed(mapping.down))
		m_vehicle->applyAccelerator(-1.f);

	if (sf::Keyboard::isKeyPressed(mapping.left))
		m_vehicle->applySteering(-1.f);

	if (sf::Keyboard::isKeyPressed(mapping.right))
		m_vehicle->applySteering(1.f);
}