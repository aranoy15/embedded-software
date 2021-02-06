#include <taskscheduler.h>
#include <algorithm>

void TaskScheduler::reg(TaskBase* task)
{
    mPool.push_back(task);
}

void TaskScheduler::start()
{
	std::sort(mPool.begin(), mPool.end(),
	          [](const TaskBase* a, const TaskBase* b) {
		          return a->prio() > b->prio();
	          });

	for(auto item : mPool)
        item->setup();

    for(;;) {
		for (auto item : mPool) {
            if (item->mTimer.elapsed()) {
                uint32_t start = Time::getTicks();
                item->func();
                uint32_t end = Time::getTicks();

                uint32_t offset = end - start;

                uint32_t period = item->mPeriod;

                if (offset < item->mPeriod) period -= offset;
                else period = 0;

                item->mTimer.start(period);
            }
        }
	}
}