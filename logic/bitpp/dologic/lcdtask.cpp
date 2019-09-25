#include <lcdtask.h>
#include <fatfsclass.h>
#include <datetimemanager.h>
#include <flash.h>

namespace
{
__attribute__((constructor)) void reg()
{
	TaskScheduler::instance()->reg(new LcdTask());
}
}  // namespace

LcdTask::LcdTask()
    : TaskBase(Time::second(), TaskPriority::Low),
      lcd1(lcd1Address, 20, 4, 1),
      lcd2(lcd2Address, 20, 4, 1),
      tempSource(),
      analogInput()
{
}

LcdTask::~LcdTask() {}

void LcdTask::setup()
{
    lcd1.init();
    lcd2.init();

    //loadDate();
}

void LcdTask::func()
{
    static uint32_t count = 0;

    lcd2.setCursor(0, 0);
	lcd2.sendString("Pressure data:");

    lcd1.setCursor(0, 0);
	lcd1.sendString("Temp data:");

    auto [result1, temp1] = tempSource.update(0);
    lcd1.setCursor(0, 1);
    if (result1) {
		lcd1.sendString(utils::stringFormat(
		    "Temp 1: %s", utils::ftostring(Ds18b20::calcValue(temp1)).c_str()));
	    lcd1.sendChar(0xDF);
	    lcd1.sendChar('C');
    } else
        lcd1.sendString("Temp 1: error");

    auto [result2, temp2] = tempSource.update(1);
    lcd1.setCursor(0, 2);
    if (result2) {
		lcd1.sendString(utils::stringFormat(
		    "Temp 2: %s", utils::ftostring(Ds18b20::calcValue(temp2)).c_str()));
	    lcd1.sendChar(0xDF);
	    lcd1.sendChar('C');
    } else
        lcd1.sendString("Temp 2: error");

    lcd1.setCursor(0, 3);
    DateTime now = DateTimeManager::instance()->now();
    lcd1.sendString("Time: ");
    lcd1.sendString(now.timeString("%02u:%02u"));
    
    uint16_t pressure1 = analogInput.getValue(0);
    uint16_t pressure2 = analogInput.getValue(1);

    float pressure1Value = Ads7844::calcValue(pressure1);
    float pressure2Value = Ads7844::calcValue(pressure2);

    lcd2.setCursor(0, 1);
    lcd2.sendString(utils::stringFormat("Pres 1: %sV", utils::ftostring(pressure1Value).c_str()));

    lcd2.setCursor(0, 2);
    lcd2.sendString(utils::stringFormat("Pres 2: %sV", utils::ftostring(pressure2Value).c_str()));

    FatFsClass& fatFs = *FatFsClass::instance();
    bool cd = FatFsClass::instance()->cardDetected();

    lcd2.setCursor(0, 3);
    lcd2.sendString(utils::stringFormat("CD: %-3s", cd ? "yes" : "no"));

    if (cd) {
        if (fatFs.init()) {

            FileFs file;
            std::string fileName = utils::stringFormat("/TEST_%u.txt", count);

            uint8_t mode = FA_WRITE | FA_READ;

            if (fatFs.fileExists(fileName.data())) mode |= FA_OPEN_APPEND;
            else mode |= FA_CREATE_ALWAYS;

            if (file.open(fileName.data(), mode)) {
                file.writeString(utils::stringFormat("Test #%u\r\n", Time::getTicks()).data());
                file.close();
            }
		}
    }
    count++;
}