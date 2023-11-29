#pragma once
#include "Interfaces/ISensor.h"

class TemperatureSensor : public ISensor<float>
{
public:
	TemperatureSensor();
	~TemperatureSensor() = default;

	virtual float Read() const override;
	virtual void Write(const float& data) override;

private:
	virtual void Poll() override;
private:
	float temperature = 0.0f;
};