#pragma once
#ifndef CONSUMER
#define CONSUMER

#include <memory>
#include <mutex>
#include <condition_variable>
#include <list>
#include "ITask.h"

template <typename TItem, typename TConsume, typename TRunCond>
class Consumer : public ITask {
	std::shared_ptr<std::list<TItem>> m_buffer;
	std::shared_ptr<std::condition_variable> m_consumerConditionVariable;
	std::shared_ptr<std::condition_variable> m_producerConditionVariable;
	std::shared_ptr<std::mutex> m_mutex;
	TConsume f_consume;
	TRunCond f_runCond;

public:
	Consumer(std::shared_ptr<std::list<TItem>>& buffer,
		std::shared_ptr<std::condition_variable>& consumerConditionVariable,
		std::shared_ptr<std::mutex> mtx,
		TConsume consume,
		TRunCond runCond) :
		m_buffer(buffer), m_consumerConditionVariable(consumerConditionVariable),
		m_mutex(mtx), f_consume(consume), f_runCond(runCond) {}

	void Start() override {
		while (f_runCond()) {
			TItem item;

			{   // critical section
				std::unique_lock<std::mutex> lck(*m_mutex);
				m_consumerConditionVariable->wait(lck, [&]() {return !m_buffer->empty() || !f_runCond(); });
				if (m_buffer->empty()) // Kill is called.
					return;
				item = m_buffer->back();
				m_buffer->pop_back();
			}	// lck gets released when destroyed.

			f_consume(item);
		}
	}
};


#endif // !CONSUMER
