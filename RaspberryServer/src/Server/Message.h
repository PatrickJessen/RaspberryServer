#pragma once
#include <mutex>

static std::mutex message_mutex;

// Message class uses template to be able to expand it in the future to be able to send objects.
template <typename T>
class Message
{
public:
	void PushBack(const T& msg)
	{
		std::lock_guard<std::mutex> lock(message_mutex);
		msgQueue.push_back(msg);
	}

	void PopFront()
	{
		std::lock_guard<std::mutex> lock(message_mutex);
		msgQueue.erase(msgQueue.begin());
	}

	static void Unlock()
	{
		message_mutex.unlock();
	}

	void Clear()
	{
		msgQueue.clear();
	}
	std::vector<T> GetQueue() { return msgQueue; }
	std::vector<int> GetOwners() { return messageOwners; }
	void AddOwner(const int& owner) { messageOwners.push_back(owner); }
private:
	std::vector<T> msgQueue{};
	std::vector<int> messageOwners;
};