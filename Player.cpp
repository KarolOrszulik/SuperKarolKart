#include "Player.h"
#include "Vehicle.h"
#include "SpeedAdjuster.h"

Player::Player(ControlScheme control, int player_number)
	: m_control(control), m_screen(player_number)
{}

void Player::drawPlayerScreen(sf::RenderTexture& source, sf::RenderTarget& target, float dt)
{
	m_screen.draw(source, target, *this, dt);
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
		sf::Keyboard::Key use;
		sf::Keyboard::Key skill;
	};

	static const std::map<Player::ControlScheme, KeyMapping> keyMappings
	{
		{ Player::ControlScheme::WASD,
			{ sf::Keyboard::Key::W, sf::Keyboard::Key::S, sf::Keyboard::Key::A, sf::Keyboard::Key::D, sf::Keyboard::Key::C, sf::Keyboard::Key::V} },
		
		{ Player::ControlScheme::ARROWS,
			{ sf::Keyboard::Key::Up, sf::Keyboard::Key::Down, sf::Keyboard::Key::Left, sf::Keyboard::Key::Right, sf::Keyboard::Key::Apostrophe, sf::Keyboard::Key::Slash}},
		
		{ Player::ControlScheme::IJKL,
			{ sf::Keyboard::Key::I, sf::Keyboard::Key::K, sf::Keyboard::Key::J, sf::Keyboard::Key::L, sf::Keyboard::Key::B, sf::Keyboard::Key::N } },
		
		{ Player::ControlScheme::NUMBERS,
			{ sf::Keyboard::Key::Numpad8, sf::Keyboard::Key::Numpad5, sf::Keyboard::Key::Numpad4, sf::Keyboard::Key::Numpad6, sf::Keyboard::PageUp, sf::Keyboard::Key::PageDown } },
	};
	// to si� a� prosi o refaktoring, ale zostawiam dla czytelno�ci

	auto mapping = keyMappings.at(m_control);

	Vehicle::Input input;

	if (sf::Keyboard::isKeyPressed(mapping.up))
		input.accelerator += 1.f;

	if (sf::Keyboard::isKeyPressed(mapping.down))
		input.accelerator -= 1.f;

	if (sf::Keyboard::isKeyPressed(mapping.left))
		input.steering -= 1.f;

	if (sf::Keyboard::isKeyPressed(mapping.right))
		input.steering += 1.f;

	if (sf::Keyboard::isKeyPressed(mapping.use))
		input.use = true;

	if (sf::Keyboard::isKeyPressed(mapping.skill))
		input.skill = true;

	m_vehicle->applyInput(input);
}