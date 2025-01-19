#include "pch.hpp"
#include "worker_pool.hpp"

namespace tur
{
	WorkerPool::~WorkerPool()
	{
		{
			std::unique_lock<std::mutex> lock(m_QueueMutex);
			m_StopExecution = true;
		}

		m_ConditionalVar.notify_all();

		for (auto& thread : m_Threads)
			thread.join();
	}

	void WorkerPool::initialize(u64 threadAmount)
	{
		for (size_t i = 0; i < threadAmount; ++i)
			m_Threads.emplace_back(std::bind(&WorkerPool::worker_function, this));
	}

	void WorkerPool::poll_tasks()
	{
		std::unique_lock<std::mutex> lock(m_QueueMutex);

		for (const auto& task : m_Callbacks)
			task();

		m_Callbacks.clear();
	}

	void WorkerPool::worker_function()
	{
		while (true)
		{
			task_t task;

			{
				std::unique_lock<std::mutex> lock(m_QueueMutex);

				m_ConditionalVar.wait(lock, [this] {
					return !m_Tasks.empty() || m_StopExecution;
				});

				if (m_StopExecution && m_Tasks.empty())
					return;

				task = std::move(m_Tasks.front());
				m_Tasks.pop_front();
			}

			task();
		}
	}
}