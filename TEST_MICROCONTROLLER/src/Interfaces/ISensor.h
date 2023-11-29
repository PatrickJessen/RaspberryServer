#pragma once

template <typename T>
class ISensor
{
public:
	virtual void Poll() = 0;
	virtual T Read() const = 0;
	virtual void Write(const T& data) = 0;
private:
	T data;
};