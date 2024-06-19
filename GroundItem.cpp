#include "GroundItem.h"
#include "Engine.h"

void GroundItem::interactWithVehicle(Vehicle&)
{
	auto engine = Engine::getInstance();

	if (isSingleUse())
		engine->flagForRemoval(this);
}

GroundItem::~GroundItem()
{
	if (m_spawner)
	{
		m_spawner->notifyObjectDeleted();
	}
}