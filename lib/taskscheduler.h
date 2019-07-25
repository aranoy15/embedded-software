#pragma once

#include <inttypes.h>
#include <timer.h>
#include <singleton.h>
#include <vector>
#include <memory>
#include <utils.h>

class TaskScheduler;

class TaskBase
{
    friend class TaskScheduler;

protected:
    uint32_t m_period; 
    Timer m_timer;

public:
	TaskBase(uint16_t period) : m_period(period), m_timer() { m_timer.start(m_period); }
	virtual ~TaskBase() = default;

    virtual void setup() = 0;
    virtual void func() = 0;
};

class TaskScheduler : public Singleton<TaskScheduler>
{
private:
	std::vector<TaskBase*> m_pool;

public:
    TaskScheduler(const TaskScheduler&) = delete;
    TaskScheduler operator=(const TaskScheduler&) = delete;

	TaskScheduler() : m_pool() {}
	virtual ~TaskScheduler() = default;

    void reg(TaskBase*);
    void start();
};