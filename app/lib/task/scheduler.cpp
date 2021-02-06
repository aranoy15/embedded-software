#include <lib/task/scheduler.hpp>
#include <algorithm>

using namespace lib::task;

void Scheduler::start()
{
    auto compare = [](const ptr_t& a, const ptr_t& b) {
        return a->prio() > b->prio();
    };

	std::sort(_data.begin(), _data.end(), compare);

	for(ptr_t& item : _data)
        item->setup();

    for(;;) {
		for (ptr_t& item : _data) {
            using namespace lib::time;

            if (item->timer().elapsed()) {
                Time start = Time::current();
                item->func();
                Time end = Time::current();

                Time offset = end - start;

                Time period = item->period();

                if (offset < period) period -= offset;
                else period = Time(0);

                item->timer().start(period);
            }
        }
	}
}