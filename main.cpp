#include "Engine.h"
#include <SFML/Graphics.hpp>
int main()
{
	auto engine = Engine::getInstance();

	engine->init(800, 600, "SuperKarolKart");
	engine->run();
}