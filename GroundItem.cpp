#include "GroundItem.h"
#include "Engine.h"

void GroundItem::interactWithVehicle(Vehicle&)
{
	auto engine = Engine::getInstance();

	if (isSingleUse())
	{
		m_spawner->notifyObjectDeleted();
		engine->flagForRemoval(this);
	}
}