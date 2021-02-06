#pragma once

#include <inttypes.h>
#include <timer.h>
#include <singleton.h>
#include <vector>
#include <memory>
#include <utils.h>

class TaskScheduler;

enum class TaskPriority
{
    Idle          = -3,          ///< priority: idle (lowest)
    Low           = -2,          ///< priority: low
    BelowNormal   = -1,          ///< priority: below normal
    Normal        =  0,          ///< priority: normal (default)
    AboveNormal   = +1,          ///< priority: above normal
    High          = +2,          ///< priority: high
    Realtime      = +3,          ///< priority: realtime (highest)
};

class TaskBase
{
    friend class TaskScheduler;

protected:
    uint32_t mPeriod; 
    TaskPriority mPrio;
    Timer mTimer;

public:
	TaskBase(uint16_t period, TaskPriority prio) : mPeriod(period), mPrio(prio), mTimer() { mTimer.start(mPeriod); }
	virtual ~TaskBase() = default;

    virtual void setup() = 0;
    virtual void func() = 0;

	TaskPriority prio() const { return mPrio; }
};

class TaskScheduler : public Singleton<TaskScheduler>
{
private:
	std::vector<TaskBase*> mPool;

public:
    TaskScheduler(const TaskScheduler&) = delete;
    TaskScheduler operator=(const TaskScheduler&) = delete;

	TaskScheduler() : mPool() {}
	virtual ~TaskScheduler() = default;

    void reg(TaskBase*);
    void start();

};