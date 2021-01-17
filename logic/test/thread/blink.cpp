#include <drivers/lib/os/thread/thread.hpp>
#include <drivers/lib/os/thread/thread_pool.hpp>
#include <drivers/lib/stream/stream.hpp>
#include <drivers/lib/time/time.hpp>
#include <drivers/lib/uart/log/log.hpp>
#include <drivers/lib/utils.hpp>

namespace logic::thread
{
class BlinkThread : public lib::os::thread::Thread
{
public:
	BlinkThread() : Thread(bsp::os::thread::priority_t::Normal, 256) {}
	virtual ~BlinkThread() {}

	std::string_view name() const override { return "blink"; }

	void func() override;
};
}  // namespace logic::thread

namespace
{
__attribute__((constructor)) void reg()
{
	using threadpool_t = lib::os::thread::ThreadPool;

	threadpool_t::instance()->reg<logic::thread::BlinkThread>();
}
}  // namespace

void logic::thread::BlinkThread::func()
{
	using namespace lib::uart::log;
	using namespace lib::stream;

	using time_t = lib::time::Time;

	loop
	{
		bsp::gpio::status::toggle();
		//Log() << "Status gpio toggle" << Endl();
		time_t::sleep(time_t(500));
	}
}
