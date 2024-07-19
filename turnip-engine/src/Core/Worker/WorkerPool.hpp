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

#include "Common.h"

namespace tur
{
	class WorkerPool
	{
	public:
		explicit WorkerPool(size_t threadAmount = std::thread::hardware_concurrency());

		~WorkerPool();

	public:
		template<typename Function, typename... Args, typename Callback>
		void SubmitTask(Function&& function, std::tuple<Args&...>&& args, Callback&& callback)
		{
			using returnType = typename std::invoke_result<Function, Args...>::type;

			auto task = std::make_shared<std::packaged_task<returnType()>>(
				[function, args = std::move(args)]() mutable {
					return std::apply(function, args);
				}
			);

			{
				std::unique_lock<std::mutex> lock(m_TaskQueueMutex);
				if (m_StopExecution)
					TUR_LOG_CRITICAL("[WorkerPool]: The worker pool has stopped suddenly.");

				m_Tasks.emplace([task, callback, this]() 
				{
					(*task)();
					returnType taskResult = task->get_future().get();

					{
						std::unique_lock<std::mutex> lock(m_CompletedTasksMutex);
						m_CompletedTasks.emplace([callback, taskResult]() { callback(taskResult); });
					}
				});
			}

			m_TaskCondition.notify_one();
		}

		void PollTasks();

	private:
		void WorkerThread();

	private:
		std::vector<std::thread> m_Workers;
		std::queue<std::function<void()>> m_Tasks;
		bool m_StopExecution = false;

		std::mutex m_TaskQueueMutex;
		std::condition_variable m_TaskCondition;

		std::queue<std::function<void()>> m_CompletedTasks;
		std::mutex m_CompletedTasksMutex;
	};
}