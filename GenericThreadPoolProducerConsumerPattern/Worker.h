#pragma once
#ifndef WORKER
#define WORKER

#include <memory>
#include <queue>
#include <condition_variable>
#include "ITask.h"
#include <functional>

namespace gtppcp {
	class Worker {
		std::shared_ptr<std::queue<std::shared_ptr<ITask>>> m_tasks;
		std::shared_ptr<std::condition_variable> m_workerConditionVariable;
		std::shared_ptr<std::mutex> m_workerMutex;
		std::function<bool()> f_runCond;

	public:
		Worker(std::shared_ptr<std::queue<std::shared_ptr<ITask>>>& tasks,
			std::shared_ptr<std::condition_variable>& workerConditionVariable,
			std::shared_ptr<std::mutex>& workerMutex,
			std::function<bool()> runCond);
		void Run();
	};
}

#endif // !WORKER
