#pragma once
#include <mutex>

// Message class uses template to be able to expand it in the future so we can recieve and send different objects over socket
template <typename T>
class Message
{
public:
	void PushBack(const T& msg)
	{
		std::lock_guard<std::mutex> lock(Message<T>::message_mutex);
		msgQueue.push_back(msg);
	}

	void PopFront()
	{
		std::lock_guard<std::mutex> lock(Message<T>::message_mutex);
		msgQueue.erase(msgQueue.begin());
	}

	static void Unlock()
	{
		Message<T>::message_mutex.unlock();
	}

	void Clear()
	{
		msgQueue.clear();
	}
	const std::vector<T>& GetQueue() { return msgQueue; }
	const std::vector<int>& GetOwners() { return messageOwners; }
	void AddOwner(const int& owner) { messageOwners.push_back(owner); }
private:
	std::vector<T> msgQueue{};
	std::vector<int> messageOwners;
	static std::mutex message_mutex;
};