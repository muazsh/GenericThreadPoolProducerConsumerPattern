#include "ThreadPool.h"

ThreadPool::ThreadPool(int size, std::shared_ptr<std::condition_variable>& taskConditionVariable) :
	m_poolSize(size), m_taskConditionVariable(taskConditionVariable),
	m_tasks(std::make_shared<std::queue<std::shared_ptr<ITask>>>()),
	m_workerConditionVariable(std::make_shared<std::condition_variable>()),
	m_workerMutex(std::make_shared<std::mutex>()) {}

void ThreadPool::AddTask(std::shared_ptr<ITask> task) {
	if (m_workers.size() <= m_poolSize) {
		m_workers.emplace_back(m_tasks, m_workerConditionVariable, m_workerMutex,
			[&]() {return m_runningWorkers; });
		m_pool.emplace_back([this]() {m_workers.back().Run(); });
	}
	std::unique_lock<std::mutex> lck(*m_workerMutex);
	m_tasks->push(task);
	lck.unlock();
	m_workerConditionVariable->notify_one();
}

void ThreadPool::Kill() {
	m_runningWorkers = false;
	m_taskConditionVariable->notify_all();
	m_workerConditionVariable->notify_all();
}

void ThreadPool::Wait() {
	for (auto& thrd : m_pool) {
		thrd.join();
	}
}
