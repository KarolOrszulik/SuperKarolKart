#pragma once
class AveragingCounter
{
public:
	AveragingCounter(float interval = 0.5f) : m_updateInterval(interval) {}

	void update(float dt, float value);

	float getValue() const { return m_averagedValue; }

private:
	const float m_updateInterval = 0.5f;

	float m_finishTime = 0.f;
	float m_totalValue = 0.f;
	size_t m_count = 0;

	float m_averagedValue = 0.f;
};

