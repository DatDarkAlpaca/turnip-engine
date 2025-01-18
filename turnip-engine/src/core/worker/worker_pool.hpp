#pragma once
#include <condition_variable>
#include <functional>
#include <future>
#include <thread>
#include <vector>
#include <atomic>
#include <mutex>
#include <tuple>
#include <queue>

#include "Common.hpp"

namespace tur
{
	class WorkerPool
	{
		using task_t = std::function<void()>;

	public:
		~WorkerPool();

	public:
		void initialize(uint64_t threadAmount = std::thread::hardware_concurrency());

	public:
		template<typename ReturnType>
		void submit(std::function<ReturnType()> task, std::function<void(ReturnType)> callback = {})
		{
			{
				std::unique_lock<std::mutex> lock(m_QueueMutex);

				m_Tasks.emplace_back([task, callback]() {
					ReturnType returnValue = task();
					callback(returnValue);
				});
			}

			m_ConditionalVar.notify_one();
		}

	private:
		void worker_function();

	private:
		std::vector<std::thread> m_Threads;
		std::deque<task_t> m_Tasks;

		std::condition_variable m_ConditionalVar;
		std::mutex m_QueueMutex;

		bool m_StopExecution = false;
	};
}