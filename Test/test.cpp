#include <gtest/gtest.h>
#include <list>
#include "Producer.h"
#include "Consumer.h"
#include "ThreadPool.h"

TEST(ProducerConsumerPattern, ConsumeAllProducedItems) {

    typedef std::function<int()> Produce;
    typedef std::function<void(int)> Consume;
    typedef std::function<bool()> RunCond;
    typedef std::function<bool()> RunCondPro;

    auto lst = std::make_shared<std::list<int>>();
    std::atomic<int> atomic_count{ 0 };
    std::atomic<int> atomic_count_prod{ 0 };
    auto cvTask = std::make_shared<std::condition_variable>();
    auto mtx = std::make_shared<std::mutex>();

    auto produce1 = [&]() { std::this_thread::sleep_for(std::chrono::milliseconds(3)); atomic_count_prod++; return 10; };
    auto produce2 = [&]() { std::this_thread::sleep_for(std::chrono::milliseconds(2)); atomic_count_prod++; return 5; };
    auto consume = [&](int x) { atomic_count++; std::this_thread::sleep_for(std::chrono::milliseconds(4)); };
    auto runCond = [&]() {return atomic_count_prod < 1000 || lst->size() > 0; };
    auto runCondPro = [&]() {return atomic_count_prod < 1000; };

    gtppcp::ThreadPool threadPool(6, cvTask);
    auto producer1 = std::make_shared<gtppcp::Producer<int, Produce, RunCondPro>>(lst, cvTask, mtx, produce1, runCondPro);
    auto producer2 = std::make_shared<gtppcp::Producer<int, Produce, RunCondPro>>(lst, cvTask, mtx, produce2, runCondPro);
    threadPool.AddTask(producer1);
    threadPool.AddTask(producer2);

    for (int i = 0; i < 4; i++)
    {
        auto cons = std::make_shared<gtppcp::Consumer<int, Consume, RunCond>>(lst, cvTask, mtx, consume, runCond);
        threadPool.AddTask(cons);
    }

    while (atomic_count_prod < 1000 || atomic_count < 1000 || lst->size() > 0)
        std::this_thread::sleep_for(std::chrono::milliseconds(10));

    threadPool.Kill();
    threadPool.Wait();

    EXPECT_EQ(atomic_count_prod, atomic_count);
}