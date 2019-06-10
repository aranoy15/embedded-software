#include <mainthread.h>
#include <blinkthread.h>
#include <spi.h>
#include <timer.h>

MainThread::MainThread() : Thread(osPriorityNormal, 0, "main") {}

MainThread::~MainThread() {}

void MainThread::threadFunc()
{
	using logUart = Uart<uart::UartPort::usart1>;
	logUart& log = *logUart::instance();

	using AdcSpi = Spi<spi::spiPort1>;
	AdcSpi& adc = *AdcSpi::instance();

	adc.init();
	log.init(64, 115200);

	log.send("Start mainthread");

	BlinkThread blinkThread;
	blinkThread.start();

	Timer timer;

	for (;;) {
		log.send(std::string("Busy state 1: ") + std::to_string(spi::adcBusy::state()));

		spi::adcCs::on();
		//Time::sleep(Time(100));
		log.send(std::string("Busy state 2: ") + std::to_string(spi::adcBusy::state()));

		uint8_t sendData = ((1 << 7) | (0 << 4) | (1 << 2) | (1 << 1) | (1 << 0));

		log.send(std::string("Send data: ") + std::to_string(sendData));

		adc.sendByte(sendData);

		log.send(std::string("Busy state 3: ") + std::to_string(spi::adcBusy::state()));

		timer.start(2000);
		bool result = true;
		while (not spi::adcBusy::state()) {
			if (timer.elapsed()) {
				result = false;
				break;
			}

			Time::sleep(Time(5));
		}

		log.send(std::string("Busy state 4: ") + std::to_string(spi::adcBusy::state()));

		if (result) {
			uint8_t resultH = adc.receiveByte();
			uint8_t resultL = adc.receiveByte();
	
			log.send(std::string("resultH: ") + std::to_string(resultH));
			log.send(std::string("resultL: ") + std::to_string(resultL));
		}

		spi::adcCs::off();

		Time::sleep(Time::seconds(1));
	}
}