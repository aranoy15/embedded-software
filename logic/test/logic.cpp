#include <logic/test/logic.hpp>
#include <drivers/bsp/F401xE_TEST/bsp.hpp>
#include <drivers/lib/time/timer.hpp>
#include <drivers/lib/uart/uart.hpp>
#include <drivers/lib/uart/log/log.hpp>
#include <drivers/lib/os/thread/thread.hpp>
#include <drivers/lib/os/thread/thread_pool.hpp>
#include <string>
#include <vector>
#include <memory>

#define loop while(true)

class BlinkThread : public lib::os::thread::Thread
{
public:
	BlinkThread() : Thread(bsp::os::priority_t::Normal, 256) {}
    virtual ~BlinkThread() {}

	std::string_view name() const override { return "blink"; }

	void func() override
	{
		using namespace lib::uart::log;
		using namespace lib::stream;

		using time_t = lib::time::Time;

		loop
		{
			bsp::gpio::status::toggle();
			Log() << "Status gpio toggle" << Endl();
			time_t::sleep(time_t(500));
		}
	}
};

class MessageThread : public lib::os::thread::Thread
{
public:
    MessageThread() : Thread(bsp::os::priority_t::High, 256) {}
    virtual ~MessageThread() {}

    std::string_view name() const override { return "message"; }

    void func() override
    {
        using namespace lib::uart::log;
        using namespace lib::stream;

        using time_t = lib::time::Time;

        loop {
            Log() << "Second thread message" << Endl();
            time_t::sleep(time_t(100));
        } 
    }
};

void main_thread(void*)
{
    using namespace lib::os::thread;

    using time_t = lib::time::Time;

    ThreadPool::instance()->start();

    loop {
        time_t::sleep(time_t(100));
    }
}

void applogic::start()
{
    using namespace lib::uart::log;
    using namespace lib::os::thread;
    using namespace bsp::os;

    using time_t = lib::time::Time;
    using timer_t = lib::time::Timer;

    bsp::gpio::init();
    LogHandler::uart_t::init(); 
    bsp::os::init();

    ThreadPool::instance()->reg<BlinkThread>();
    ThreadPool::instance()->reg<MessageThread>();

    Thread::start(main_thread, "main", 256, priority_t::Realtime);
    bsp::os::start();
}
