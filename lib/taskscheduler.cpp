#include <taskscheduler.h>

void TaskScheduler::reg(TaskBase* task)
{
    m_pool.push_back(task);
}

void TaskScheduler::start()
{
    for(auto item : m_pool)
        item->setup();

    for(;;) {
		for (auto item : m_pool) {
            if (item->m_timer.elapsed()) {
                item->func();
                item->m_timer.start(item->m_period);
            }
        }
	}
}