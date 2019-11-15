#pragma once

#include <queue>
#include <mutex>

template<typename TData>
class SynchronizedQueue
{
public:
	SynchronizedQueue()
	{
		
	}

	void Push(TData& data)
	{
		_queueGuard.lock();

		_queue.push(data);

		_queueGuard.unlock();
	}

	
	TData Pop()
	{
		_queueGuard.lock();

		if (_queue.empty())
			throw std::exception("empty");

		TData result = _queue.front();
		_queue.pop();

		_queueGuard.unlock();

		return result;
	}

private:
	std::mutex _queueGuard;
	std::queue<TData> _queue;
};
