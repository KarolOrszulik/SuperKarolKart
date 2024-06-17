#include "GroundItem.h"

GroundItem::~GroundItem()
{
	if (m_spawner)
	{
		m_spawner->notifyObjectDeleted();
	}
}