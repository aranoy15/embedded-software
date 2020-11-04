#ifndef DRIVERS_LIB_TASK_SCHEDULER
#define DRIVERS_LIB_TASK_SCHEDULER

#include <cinttypes>
#include <lib/time/timer.hpp>
#include <lib/task/taskbase.hpp>
#include <vector>
#include <memory>

namespace lib::task
{
struct Scheduler final
{
    using item_t = TaskBase;
    using ptr_t = std::unique_ptr<item_t>;
    using data_t = std::vector<ptr_t>;

    Scheduler() = delete;

    template<class T, typename... Args>
    static void reg(Args&&... args) noexcept;

    static void start();

private:
	inline static std::vector<std::unique_ptr<TaskBase>> _data;
};

template<class T, typename... Args>
void Scheduler::reg(Args&&... args) noexcept
{
    static_assert(std::is_base_of<TaskBase, T>::value, "not base class");
    _data.emplace_back(std::make_unique<T>(std::forward<Args>(args)...));
}

}

#endif /* DRIVERS_LIB_TASK_SCHEDULER */
