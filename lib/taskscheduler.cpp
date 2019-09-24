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
                uint32_t start = Time::getTicks();
                item->func();
                uint32_t end = Time::getTicks();

                uint32_t offset = end - start;

                uint32_t period = item->m_period;

                if (offset < item->m_period) period -= offset;
                else period = 0;

                item->m_timer.start(period);
            }
        }
	}
}