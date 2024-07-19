#include "pch.hpp"
#include "WorkerPool.hpp"

#include <iostream>

namespace tur
{
	WorkerPool::WorkerPool(size_t threadAmount)
	{
		for (size_t i = 0; i < threadAmount; ++i)
			m_Workers.emplace_back(&WorkerPool::WorkerThread, this);
	}

	WorkerPool::~WorkerPool()
	{
		{
			std::unique_lock<std::mutex> lock(m_TaskQueueMutex);
			m_StopExecution = true;
		}

		m_TaskCondition.notify_all();

		for (std::thread& worker : m_Workers)
			worker.join();
	}

	void WorkerPool::PollTasks()
	{
		std::queue<std::function<void()>> tasksToExecute;

		{
			std::unique_lock<std::mutex> lock(m_CompletedTasksMutex);
			std::swap(tasksToExecute, m_CompletedTasks);
		}

		while (!tasksToExecute.empty())
		{
			tasksToExecute.front()();
			tasksToExecute.pop();
		}
	}

	void WorkerPool::WorkerThread()
	{
		while (true)
		{
			std::function<void()> task;

			{
				std::unique_lock<std::mutex> lock(m_TaskQueueMutex);
				m_TaskCondition.wait(lock, [this] { return m_StopExecution || !m_Tasks.empty(); });

				if (m_StopExecution && m_Tasks.empty())
					return;

				task = std::move(m_Tasks.front());
				m_Tasks.pop();
			}

			task();
		}
	}
}