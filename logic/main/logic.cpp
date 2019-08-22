#include <logic.h>
#include <time.h>
#include <uart.h>
#include <bsp.h>


void applogic::startLogic()
{
  using blinkLed = GPIO<PinDef<CSP_GPIO_PORT_NBR_C, GPIO_PIN_13>, bsp::mOutputPP>;

  blinkLed::setup();

	for (;;) {
    blinkLed::on();
    Time::sleep(Time(500));
    blinkLed::off();
    Time::sleep(Time(500));
	}
}

/*
#include <i2c.h>
#include <lcd.h>
#include <math.h>
#include <utils.h>
#include <bme280.h>
#include <ds3231.h>
#include <algorithm>
#include <mhz19.h>
*/

//using namespace gpio;

/*
uint8_t LT[8] = {0b00111,  0b01111,  0b11111,  0b11111,  0b11111,  0b11111,  0b11111,  0b11111};
uint8_t UB[8] = {0b11111,  0b11111,  0b11111,  0b00000,  0b00000,  0b00000,  0b00000,  0b00000};
uint8_t RT[8] = {0b11100,  0b11110,  0b11111,  0b11111,  0b11111,  0b11111,  0b11111,  0b11111};
uint8_t LL[8] = {0b11111,  0b11111,  0b11111,  0b11111,  0b11111,  0b11111,  0b01111,  0b00111};
uint8_t LB[8] = {0b00000,  0b00000,  0b00000,  0b00000,  0b00000,  0b11111,  0b11111,  0b11111};
uint8_t LR[8] = {0b11111,  0b11111,  0b11111,  0b11111,  0b11111,  0b11111,  0b11110,  0b11100};
uint8_t UMB[8] = {0b11111,  0b11111,  0b11111,  0b00000,  0b00000,  0b00000,  0b11111,  0b11111};
uint8_t LMB[8] = {0b11111,  0b00000,  0b00000,  0b00000,  0b00000,  0b11111,  0b11111,  0b11111};

Lcd lcd(0x27, 20, 4, 1);

void drawDig(uint8_t dig, uint8_t x, uint8_t y) {
  switch (dig) {
    case 0:
      lcd.setCursor(x, y); // set cursor to column 0, line 0 (first row)
      lcd.write(0);  // call each segment to create
      lcd.write(1);  // top half of the number
      lcd.write(2);
      lcd.setCursor(x, y + 1); // set cursor to colum 0, line 1 (second row)
      lcd.write(3);  // call each segment to create
      lcd.write(4);  // bottom half of the number
      lcd.write(5);
      break;
    case 1:
      //lcd.setCursor(x + 1, y);
      lcd.setCursor(x, y);
      lcd.sendChar(' ');
      lcd.write(1);
      lcd.write(2);
      //lcd.setCursor(x + 2, y + 1);
      lcd.setCursor(x, y + 1);
      lcd.sendChar(' ');
      lcd.sendChar(' ');
      lcd.write(5);
      break;
    case 2:
      lcd.setCursor(x, y);
      lcd.write(6);
      lcd.write(6);
      lcd.write(2);
      lcd.setCursor(x, y + 1);
      lcd.write(3);
      lcd.write(7);
      lcd.write(7);
      break;
    case 3:
      lcd.setCursor(x, y);
      lcd.write(6);
      lcd.write(6);
      lcd.write(2);
      lcd.setCursor(x, y + 1);
      lcd.write(7);
      lcd.write(7);
      lcd.write(5);
      break;
    case 4:
      lcd.setCursor(x, y);
      lcd.write(3);
      lcd.write(4);
      lcd.write(2);
      //lcd.setCursor(x + 2, y + 1);
      lcd.setCursor(x, y + 1);
      lcd.sendChar(' ');
      lcd.sendChar(' ');
      lcd.write(5);
      break;
    case 5:
      lcd.setCursor(x, y);
      lcd.write(0);
      lcd.write(6);
      lcd.write(6);
      lcd.setCursor(x, y + 1);
      lcd.write(7);
      lcd.write(7);
      lcd.write(5);
      break;
    case 6:
      lcd.setCursor(x, y);
      lcd.write(0);
      lcd.write(6);
      lcd.write(6);
      lcd.setCursor(x, y + 1);
      lcd.write(3);
      lcd.write(7);
      lcd.write(5);
      break;
    case 7:
      lcd.setCursor(x, y);
      lcd.write(1);
      lcd.write(1);
      lcd.write(2);
      //lcd.setCursor(x + 1, y + 1);
      lcd.setCursor(x, y + 1);
      lcd.sendChar(' ');
      lcd.write(0);
      lcd.sendChar(' ');
      break;
    case 8:
      lcd.setCursor(x, y);
      lcd.write(0);
      lcd.write(6);
      lcd.write(2);
      lcd.setCursor(x, y + 1);
      lcd.write(3);
      lcd.write(7);
      lcd.write(5);
      break;
    case 9:
      lcd.setCursor(x, y);
      lcd.write(0);
      lcd.write(6);
      lcd.write(2);
      //lcd.setCursor(x + 1, y + 1);
      lcd.setCursor(x, y + 1);
      lcd.sendChar(' ');
      lcd.write(4);
      lcd.write(5);
      break;
    case 10:
      lcd.setCursor(x, y);
      lcd.write(32);
      lcd.write(32);
      lcd.write(32);
      lcd.setCursor(x, y + 1);
      lcd.write(32);
      lcd.write(32);
      lcd.write(32);
      break;
  }
}

void loadClock()
{
    lcd.createChar(0, LT);
	lcd.createChar(1, UB);
	lcd.createChar(2, RT);
	lcd.createChar(3, LL);
	lcd.createChar(4, LB);
	lcd.createChar(5, LR);
	lcd.createChar(6, UMB);
	lcd.createChar(7, LMB);
}

void drawDots(bool dotState, uint8_t x, uint8_t y) 
{
    lcd.setCursor(x + 7, y);
    lcd.sendChar(dotState ? '*' : ' ');
    lcd.setCursor(x + 7, y + 1);
    lcd.sendChar(dotState ? '*' : ' ');
}

void drawClock(uint8_t hours, uint8_t minutes, uint8_t x, uint8_t y, bool dotState) {

  // чисти чисти!
  //lcd.setCursor(x, y);
  //lcd.sendString("               ");
  //lcd.setCursor(x, y + 1);
  //lcd.sendString("               ");

  //if (hours > 23 || minutes > 59) return;
  if (hours / 10 == 0)
	  drawDig(10, x, y);
  else
	  drawDig(hours / 10, x, y);
  drawDig(hours % 10, x + 4, y);
  // тут должны быть точки. Отдельной функцией

  drawDig(minutes / 10, x + 8, y);
  drawDig(minutes % 10, x + 12, y);

  lcd.home();
}

*/

/*
    using LogUart = Uart<uart::UartPort::usart1>;
    LogUart& log = *LogUart::instance();

    using MainI2cType = I2C<i2c::i2cPort1>;
    MainI2cType& mainI2C = *MainI2cType::instance();

    log.init(128, 115200);
    Uart<uart::UartPort::usart2>::instance()->init(16, 9600);
    Mhz19 mhz19;
    mhz19.setAutoCalibration(false);

    mainI2C.init();
    log.send("Start application");
    */

    /*
    std::vector<uint8_t> addresses = mainI2C.searchAddresses();

    if (addresses.empty())
        log.send("No i2c devices");
    else {
        for (const uint8_t address : addresses) {
            char hex[10];
            sprintf(hex, "0x%02X", address);
            log.send(std::string("Find device number: ") + std::string(hex));
        }
    }

    log.send(" ");
    */

   /*
    Ds3231 ds3231(0x68);
    Bme280 bme(0x76);
    bme.init();
    lcd.init();

    loadClock();
    lcd.home();

    {
        DateTime dtNow;
        dtNow = ds3231.now();
        drawClock(dtNow.hour(), dtNow.minute(), 0, 0, true);
    }

    bool dotState = false;

	for (;;) {
            std::string temperature  = utils::ftostring(bme.readTemperature());
            std::string humidity = utils::ftostring(bme.readHumidity());
            float pressureValue = bme.readPressure();
            std::string pressure = std::to_string(static_cast<uint16_t>(pressureValue / 100.0f));
            std::string pressureInMM = std::to_string(static_cast<uint16_t>(pressureValue / 133.3f));

            log.send(std::string("Temperature = ") + temperature + " *C");
            log.send(std::string("Pressure = ") + pressure + " hPa");
            log.send(std::string("Humidity = ") + humidity + " %");
            log.send(" ");

            lcd.setCursor(0, 2);
            auto tempString = std::string(" ") + temperature + std::string(" *C");
            if (tempString.capacity() < 10) {
                for (uint8_t i = 0; i < (10 - tempString.capacity()); i++)
                    tempString.push_back(' ');
            }
            lcd.sendString(tempString);

            lcd.setCursor(10, 2);
            auto humString = std::string(" ") + humidity + std::string(" %");
            if (humString.capacity() < 10) {
                for (uint8_t i = 0; i < (10 - humString.capacity()); i++)
                    humString.push_back(' ');
            }
            lcd.sendString(humString);

            lcd.setCursor(0, 3);
            auto pressInMMString = std::string(" ") + pressureInMM + std::string(" mm");
            if (pressInMMString.capacity() < 10) {
                for (uint8_t i = 0; i < (10 - pressInMMString.capacity()); i++)
                    pressInMMString.push_back(' ');
            }
            lcd.sendString(pressInMMString);

            lcd.setCursor(10, 3);
            auto pressString = std::string(" ") + pressure + std::string(" hPa");
            if (pressString.capacity() < 10) {
                for (uint8_t i = 0; i < (10 - pressString.capacity()); i++)
                    pressString.push_back(' ');
            }
            lcd.sendString(pressString);

            DateTime dtNow;
            dtNow = ds3231.now();
            log.send(dtNow.toString());
            log.send(" ");

            drawClock(dtNow.hour(), dtNow.minute(), 0, 0, true);

            dotState = not dotState;
            drawDots(dotState, 0, 0);

        uint16_t co2 = mhz19.getCO2();

        lcd.setCursor(16, 0);
        lcd.sendString(std::to_string(co2));

        Time::sleep(Time::seconds(1));
    }
    */