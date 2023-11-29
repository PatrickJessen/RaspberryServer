#include "TemperatureSensor.h"
#include <stdlib.h>
#include <time.h>

TemperatureSensor::TemperatureSensor()
{
	// init random seed
	srand(time(NULL));
	temperature = -7.0f;
}

float TemperatureSensor::Read() const
{
	return temperature;
}

void TemperatureSensor::Write(const float& data)
{
	// Code for writing to the sensor goes here
}

void TemperatureSensor::Poll()
{
	bool isNegative = rand() / (RAND_MAX)+1;
	float margin = 0.1f;
	float currentTemp = 0.0f;
	if (isNegative) {
		currentTemp = temperature - margin;
	}
	else {
		currentTemp = temperature + margin;
	}
	temperature = currentTemp;
}
