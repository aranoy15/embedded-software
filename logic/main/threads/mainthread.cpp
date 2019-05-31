#include <mainthread.h>
#include <blinkthread.h>
#include <commandthread.h>
#include <uart.h>

MainThread::MainThread() : Thread(osPriorityNormal, 0, "main") {}

MainThread::~MainThread() {}

using namespace gpio;
using namespace uart;

void MainThread::threadFunc()
{
	Uart<UartPort::usart1>::instance()->init(128, 115200);
	I2C<i2c::i2cPort1>::instance()->init();

    using btnInput = GPIO<PinDef<CSP_GPIO_PORT_NBR_B, GPIO_PIN_10>, mInput>;
    btnInput::setup();
	Lcd lcd(0x27, 20, 4, 1);

	lcd.sendString("Hello!!!");
	lcd.setCursor(0, 2);
	lcd.sendString("I'm stm32f103!!!");

    BlinkThread blinkThread;
    blinkThread.start();

    CommandThread commandThread;
    commandThread.start();

    uint8_t row[8] = {0b00000,  0b11111,  0b11111,  0b11111,  0b11111,  0b11111,  0b11111,  0b11111};

	for (;;) {
		lcd.setCursor(0, 1);
		lcd.sendString("State: " + std::to_string(btnInput::state()));
        lcd.setCursor(0, 3);
        lcd.createChar(0, row);
        lcd.sendString("Test");

		Time::sleep(Time(10));
	}
}