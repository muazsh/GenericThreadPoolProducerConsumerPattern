#pragma once
#ifndef PRODUCER
#define PRODUCER

#include<memory>
#include<mutex>
#include<condition_variable>
#include<list>
#include"ITask.h"

template <typename TItem, typename TProduce, typename TRunCond>
class Producer : public ITask
{
	std::shared_ptr<std::list<TItem>> m_buffer;
	std::shared_ptr<std::condition_variable> m_consumerConditionVariable;
	std::shared_ptr<std::mutex> m_mutex;
	TProduce f_produce;
	TRunCond f_runCond;

public:
	Producer(std::shared_ptr<std::list<TItem>>& buffer,
		std::shared_ptr<std::condition_variable>& consumerConditionVariable,
		std::shared_ptr<std::mutex> mtx, TProduce produce, TRunCond runCond) :
		m_buffer(buffer),
		m_consumerConditionVariable(consumerConditionVariable),
		m_mutex(mtx),
		f_produce(produce), f_runCond(runCond) {}

	void Start() override
	{
		while (f_runCond()) {
			TItem item = f_produce();

			{   // critical section
				std::unique_lock<std::mutex> lck(*m_mutex);
				m_buffer->push_back(item);
				m_consumerConditionVariable->notify_one();
			}	// lck gets unlocked when destoryed.
		}
	}
};

#endif // !PRODUCER
