#pragma once
#ifndef THREAD_POOL
#define THREAD_POOL

#include <memory>
#include <queue>
#include <condition_variable>
#include "ITask.h"
#include <list>
#include "Worker.h"
#include <functional>

namespace gtppcp {
	class ThreadPool {
		std::size_t m_poolSize;
		std::shared_ptr<std::queue<std::shared_ptr<ITask>>> m_tasks;
		std::shared_ptr<std::condition_variable> m_workerConditionVariable;
		std::shared_ptr<std::condition_variable> m_taskConditionVariable;
		std::shared_ptr<std::mutex> m_workerMutex;
		std::list<Worker> m_workers;
		std::list<std::thread> m_pool;
		bool m_runningWorkers = true;

	public:
		ThreadPool(std::size_t size, std::shared_ptr<std::condition_variable>& taskConditionVariable);
		void AddTask(std::shared_ptr<ITask> task);
		void Kill();
		void Wait();
	};
}

#endif // !THREAD_POOL
