#include <lib/os/thread/thread.hpp>
#include <lib/os/thread/thread_pool.hpp>
#include <lib/stream/stream.hpp>
#include <lib/time/time.hpp>
#include <lib/utils.hpp>

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
	using namespace lib::stream;

	using time_t = lib::time::Time;

	loop
	{
		bsp::gpio::status::toggle();
		time_t::sleep(time_t(500));
	}
}
