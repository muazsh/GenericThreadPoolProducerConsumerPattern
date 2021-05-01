#include "Worker.h"

Worker::Worker(std::shared_ptr<std::queue<std::shared_ptr<ITask>>>& tasks,
	std::shared_ptr<std::condition_variable>& workerConditionVariable,
	std::shared_ptr<std::mutex>& workerMutex,
	std::function<bool()> runCond) :
	m_tasks(tasks), m_workerConditionVariable(workerConditionVariable),
	m_workerMutex(workerMutex), f_runCond(runCond) {}

void Worker::Run() {
	while (f_runCond()) {

		// critical section
		std::unique_lock<std::mutex> lck(*m_workerMutex);
		m_workerConditionVariable->wait(lck, [&] { return !m_tasks->empty() || !f_runCond(); });
		if (!f_runCond()) // Kill is called.
			return;
		auto task = m_tasks->front();
		m_tasks->pop();
		lck.unlock();
		// !critical section

		task->Start();
	}
}
