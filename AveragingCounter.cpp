#include "AveragingCounter.h"

void AveragingCounter::update(float dt, float value)
{
	m_totalValue += value;
	m_finishTime += dt;
	m_count++;

	if (m_finishTime >= m_updateInterval)
	{
		m_averagedValue = m_totalValue / m_count;
		m_totalValue = 0.f;
		m_finishTime = 0.f;
		m_count = 0;
	}
}
