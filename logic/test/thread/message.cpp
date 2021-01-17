#include <bsp.hpp>
#include <drivers/lib/os/thread/thread_pool.hpp>
#include <drivers/lib/uart/log/log.hpp>
#include <drivers/lib/time/time.hpp>
#include <drivers/lib/utils.hpp>

namespace logic::thread
{

class MessageThread : public lib::os::thread::Thread
{
public:
	MessageThread() : Thread(bsp::os::thread::priority_t::High, 256) {}
	virtual ~MessageThread() {}

	std::string_view name() const override { return "message"; }

	void func() override;
};

}  // namespace logic::thread

namespace 
{
__attribute__((constructor))
void reg()
{
    using threadpool_t = lib::os::thread::ThreadPool;

    threadpool_t::instance()->reg<logic::thread::MessageThread>();
}
}

void logic::thread::MessageThread::func()
{
	using namespace lib::uart::log;
	using namespace lib::stream;

	using time_t = lib::time::Time;
    using result_t = Log::ResultRead;

	loop
	{
        std::string receive;
        result_t result;

        Log() >> receive >> result;

        if (result == result_t::OK) {
            Log() << "Receive: " << receive << Endl();
        }

		time_t::sleep(time_t(10));
	}
}

