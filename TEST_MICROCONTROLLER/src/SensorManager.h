#pragma once
#include "Interfaces/ISensor.h"
#include <chrono>

template <typename T>
class SensorManager
{
public:
	SensorManager(ISensor<T>* sensor)
		: sensor(sensor)
	{
		now = std::chrono::steady_clock::now();
	}
	~SensorManager()
	{
		delete sensor;
	}

public:
	void Poll()
	{
		elapsedTime = { now - end };
		// Poll every 10 seconds
		if (elapsedTime >= 10)
		{
			sensor->Poll();
			now = std::chrono::steady_clock::now();
		}
		end = std::chrono::steady_clock::now();
	}
	T Read() const
	{
		return sensor->Read();
	}
	void Write(const T& data)
	{
		sensor->Write(data);
	}
private:
	std::chrono::steady_clock::time_point now;
	std::chrono::steady_clock::time_point end;
	std::chrono::duration<int> elapsedTime;
	ISensor<T>* sensor = nullptr;
};
