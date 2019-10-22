#include <taskscheduler.h>
#include <bsp.h>

class BlinkTask : public TaskBase
{
private:
	using blink_pin = GPIO<PinDef<CSP_GPIO_PORT_NBR_C, GPIO_PIN_13>,
	                       bsp::mOutputPP, bsp::sHi, bsp::pUp>;
	bool m_state;

public:
    BlinkTask() : TaskBase(5000, TaskPriority::Low), m_state(false) {}
    virtual ~BlinkTask() {}

	void setup() override { blink_pin::setup(); }
    void func() override
    {
        if (m_state) blink_pin::on();
        else blink_pin::off();

        m_state = not m_state;
    }
};

namespace
{
__attribute__((constructor)) void reg()
{
	TaskScheduler::instance()->reg(new BlinkTask());
}
}  // namespace