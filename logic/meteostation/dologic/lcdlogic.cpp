#include <lcdlogic.h>
#include <clocklogic.h>
#include <sensorlogic.h>
#include <logicstate.h>

uint8_t LcdLogic::LT[8] = {0b00111, 0b01111, 0b11111, 0b11111,
                           0b11111, 0b11111, 0b11111, 0b11111};
uint8_t LcdLogic::UB[8] = {0b11111, 0b11111, 0b11111, 0b00000,
                           0b00000, 0b00000, 0b00000, 0b00000};
uint8_t LcdLogic::RT[8] = {0b11100, 0b11110, 0b11111, 0b11111,
                           0b11111, 0b11111, 0b11111, 0b11111};
uint8_t LcdLogic::LL[8] = {0b11111, 0b11111, 0b11111, 0b11111,
                           0b11111, 0b11111, 0b01111, 0b00111};
uint8_t LcdLogic::LB[8] = {0b00000, 0b00000, 0b00000, 0b00000,
                           0b00000, 0b11111, 0b11111, 0b11111};
uint8_t LcdLogic::LR[8] = {0b11111, 0b11111, 0b11111, 0b11111,
                           0b11111, 0b11111, 0b11110, 0b11100};
uint8_t LcdLogic::UMB[8] = {0b11111, 0b11111, 0b11111, 0b00000,
                            0b00000, 0b00000, 0b11111, 0b11111};
uint8_t LcdLogic::LMB[8] = {0b11111, 0b00000, 0b00000, 0b00000,
                            0b00000, 0b11111, 0b11111, 0b11111};


std::vector<std::string> LcdLogic::weekDay = {
	"Sun",
	"Mon",
	"Tue",
	"Wed",
	"Thu",
	"Fri",
	"Sat"
};

LcdLogic::LcdLogic()
    : lcd(new Lcd(0x27, 20, 4, 1)),
      mTimer(new Timer(1000)),
      mChartTimer(new Timer(10 * Time::second())),
      mDotState(false)
{
    lcd->init();
}

LcdLogic::~LcdLogic()
{
}

void LcdLogic::drawDig(uint8_t dig, uint8_t x, uint8_t y)
{
	switch (dig) {
		case 0:
			lcd->setCursor(x, y);
			lcd->write(0); 
			lcd->write(1);
			lcd->write(2);
			lcd->setCursor(x, y + 1);
			lcd->write(3);
			lcd->write(4);
			lcd->write(5);
			break;
		case 1:
			lcd->setCursor(x, y);
			lcd->sendChar(' ');
			lcd->write(1);
			lcd->write(2);
			lcd->setCursor(x, y + 1);
			lcd->sendChar(' ');
			lcd->sendChar(' ');
			lcd->write(5);
			break;
		case 2:
			lcd->setCursor(x, y);
			lcd->write(6);
			lcd->write(6);
			lcd->write(2);
			lcd->setCursor(x, y + 1);
			lcd->write(3);
			lcd->write(7);
			lcd->write(7);
			break;
		case 3:
			lcd->setCursor(x, y);
			lcd->write(6);
			lcd->write(6);
			lcd->write(2);
			lcd->setCursor(x, y + 1);
			lcd->write(7);
			lcd->write(7);
			lcd->write(5);
			break;
		case 4:
			lcd->setCursor(x, y);
			lcd->write(3);
			lcd->write(4);
			lcd->write(2);
			lcd->setCursor(x, y + 1);
			lcd->sendChar(' ');
			lcd->sendChar(' ');
			lcd->write(5);
			break;
		case 5:
			lcd->setCursor(x, y);
			lcd->write(0);
			lcd->write(6);
			lcd->write(6);
			lcd->setCursor(x, y + 1);
			lcd->write(7);
			lcd->write(7);
			lcd->write(5);
			break;
		case 6:
			lcd->setCursor(x, y);
			lcd->write(0);
			lcd->write(6);
			lcd->write(6);
			lcd->setCursor(x, y + 1);
			lcd->write(3);
			lcd->write(7);
			lcd->write(5);
			break;
		case 7:
			lcd->setCursor(x, y);
			lcd->write(1);
			lcd->write(1);
			lcd->write(2);
			lcd->setCursor(x, y + 1);
			lcd->sendChar(' ');
			lcd->write(0);
			lcd->sendChar(' ');
			break;
		case 8:
			lcd->setCursor(x, y);
			lcd->write(0);
			lcd->write(6);
			lcd->write(2);
			lcd->setCursor(x, y + 1);
			lcd->write(3);
			lcd->write(7);
			lcd->write(5);
			break;
		case 9:
			lcd->setCursor(x, y);
			lcd->write(0);
			lcd->write(6);
			lcd->write(2);
			lcd->setCursor(x, y + 1);
			lcd->sendChar(' ');
			lcd->write(4);
			lcd->write(5);
			break;
		case 10:
			lcd->setCursor(x, y);
			lcd->write(32);
			lcd->write(32);
			lcd->write(32);
			lcd->setCursor(x, y + 1);
			lcd->write(32);
			lcd->write(32);
			lcd->write(32);
			break;
	}
}

void LcdLogic::loadClock()
{
    lcd->createChar(0, LT);
	lcd->createChar(1, UB);
	lcd->createChar(2, RT);
	lcd->createChar(3, LL);
	lcd->createChar(4, LB);
	lcd->createChar(5, LR);
	lcd->createChar(6, UMB);
	lcd->createChar(7, LMB);
}

void LcdLogic::drawDots(bool dotState, uint8_t x, uint8_t y)
{
	uint8_t dot = dotState ? 0xA5 : 0x20;
    lcd->setCursor(x + 7, y);
    lcd->sendChar(dot);
    lcd->setCursor(x + 7, y + 1);
    lcd->sendChar(dot);
}

void LcdLogic::drawClock(uint8_t hours, uint8_t minutes, uint8_t x, uint8_t y)
{
	if (hours / 10 == 0)
		drawDig(10, x, y);
	else
		drawDig(hours / 10, x, y);
	drawDig(hours % 10, x + 4, y);

    drawDig(minutes / 10, x + 8, y);
    drawDig(minutes % 10, x + 12, y);

    lcd->home();

    mDotState = not mDotState;
    drawDots(mDotState, 0, 0);
}

void LcdLogic::drawDate(const DateTime& dateTime, uint8_t x, uint8_t y)
{
	lcd->setCursor(x, y);
	lcd->sendString(
	    utils::stringFormat("%02u.%02u", dateTime.day(), dateTime.month()));
	
	lcd->setCursor(x + 1, y + 1);
	lcd->sendString(
	    utils::stringFormat("%4s", weekDay[dateTime.dayOfTheWeek()].c_str()));
}

void LcdLogic::drawCO2(uint16_t co2, uint8_t x, uint8_t y)
{
	lcd->setCursor(x, y);
	lcd->sendString(utils::stringFormat("%4u ppm", co2));
}

void LcdLogic::drawTemp(float temp, uint8_t x, uint8_t y)
{
	lcd->setCursor(x, y);
	lcd->sendString(utils::stringFormat("%s", utils::ftostring(temp).c_str()));
	lcd->sendChar(0xDF);
	lcd->sendChar('C');
}

void LcdLogic::drawHumidity(uint8_t hum, uint8_t x, uint8_t y)
{
	lcd->setCursor(x, y);
	lcd->sendString(utils::stringFormat("%u", hum) + "%");
}


void LcdLogic::drawPressure(uint16_t press, uint8_t x, uint8_t y)
{
	lcd->setCursor(x, y);
	lcd->sendString(utils::stringFormat("%4u mm", press));
}

void LcdLogic::drawRainPercent(int8_t percent, uint8_t x, uint8_t y)
{
	lcd->setCursor(x, y);
	lcd->sendString(utils::stringFormat("rain %3i%%", percent));
}

void LcdLogic::mainShow()
{
	if (mTimer->elapsed()) {
		loadClock();
		const DateTime& dateTime = ClockLogic::instance()->getDateTime();

		drawClock(dateTime.hour(), dateTime.minute(), 0, 0);
		drawDate(dateTime, 15, 0);
		drawCO2(SensorLogic::instance()->getCO2(), 12, 2);
		drawTemp(SensorLogic::instance()->getTemp(), 0, 2);
		drawHumidity(SensorLogic::instance()->getHumidity(), 8, 2);
		drawPressure(SensorLogic::instance()->getPressure(), 0, 3);
		drawRainPercent(SensorLogic::instance()->getRainPercent(), 10, 3);
		mTimer->start();
	}
}

void drawCo2DayChart()
{
	
}
void drawCo2HourChart();

void LcdLogic::processLcd()
{

		LogicStateType logicState = LogicState::instance()->currentState();

		switch (logicState) {
			default:
			case LogicStateType::MainInfo:
				mainShow();
				break;
			case LogicStateType::Co2DayChart:
				lcd->clear();
				lcd->setCursor(0, 0);
				lcd->sendString("Hello day chart");
				break;
		}

}