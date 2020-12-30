#include <logic/meteostation/dologic/lcdlogic.hpp>
#include <logic/meteostation/dologic/datastore.hpp>
#include <drivers/lib/stream/stringhandler.hpp>
#include <drivers/lib/stream/actions.hpp>

using namespace applogic;
using namespace lib::stream;

LcdLogic::LcdLogic()
    : lib::task::TaskBase(lib::time::Time::secs(1),
                          lib::task::Priority::Normal),
      lcd(20, 4, 1)
{
}

LcdLogic::~LcdLogic()
{
}

void LcdLogic::draw_dig(uint8_t dig, uint8_t x, uint8_t y)
{
	switch (dig) {
		case 0:
			lcd.set_cursor(x, y);
			lcd.write(0); 
			lcd.write(1);
			lcd.write(2);
			lcd.set_cursor(x, y + 1);
			lcd.write(3);
			lcd.write(4);
			lcd.write(5);
			break;
		case 1:
			lcd.set_cursor(x, y);
			lcd.send_char(' ');
			lcd.write(1);
			lcd.write(2);
			lcd.set_cursor(x, y + 1);
			lcd.send_char(' ');
			lcd.send_char(' ');
			lcd.write(5);
			break;
		case 2:
			lcd.set_cursor(x, y);
			lcd.write(6);
			lcd.write(6);
			lcd.write(2);
			lcd.set_cursor(x, y + 1);
			lcd.write(3);
			lcd.write(7);
			lcd.write(7);
			break;
		case 3:
			lcd.set_cursor(x, y);
			lcd.write(6);
			lcd.write(6);
			lcd.write(2);
			lcd.set_cursor(x, y + 1);
			lcd.write(7);
			lcd.write(7);
			lcd.write(5);
			break;
		case 4:
			lcd.set_cursor(x, y);
			lcd.write(3);
			lcd.write(4);
			lcd.write(2);
			lcd.set_cursor(x, y + 1);
			lcd.send_char(' ');
			lcd.send_char(' ');
			lcd.write(5);
			break;
		case 5:
			lcd.set_cursor(x, y);
			lcd.write(0);
			lcd.write(6);
			lcd.write(6);
			lcd.set_cursor(x, y + 1);
			lcd.write(7);
			lcd.write(7);
			lcd.write(5);
			break;
		case 6:
			lcd.set_cursor(x, y);
			lcd.write(0);
			lcd.write(6);
			lcd.write(6);
			lcd.set_cursor(x, y + 1);
			lcd.write(3);
			lcd.write(7);
			lcd.write(5);
			break;
		case 7:
			lcd.set_cursor(x, y);
			lcd.write(1);
			lcd.write(1);
			lcd.write(2);
			lcd.set_cursor(x, y + 1);
			lcd.send_char(' ');
			lcd.write(0);
			lcd.send_char(' ');
			break;
		case 8:
			lcd.set_cursor(x, y);
			lcd.write(0);
			lcd.write(6);
			lcd.write(2);
			lcd.set_cursor(x, y + 1);
			lcd.write(3);
			lcd.write(7);
			lcd.write(5);
			break;
		case 9:
			lcd.set_cursor(x, y);
			lcd.write(0);
			lcd.write(6);
			lcd.write(2);
			lcd.set_cursor(x, y + 1);
			lcd.send_char(' ');
			lcd.write(4);
			lcd.write(5);
			break;
		case 10:
			lcd.set_cursor(x, y);
			lcd.write(32);
			lcd.write(32);
			lcd.write(32);
			lcd.set_cursor(x, y + 1);
			lcd.write(32);
			lcd.write(32);
			lcd.write(32);
			break;
	}
}

void LcdLogic::load_clock()
{
    lcd.create_char(0, LT);
	lcd.create_char(1, UB);
	lcd.create_char(2, RT);
	lcd.create_char(3, LL);
	lcd.create_char(4, LB);
	lcd.create_char(5, LR);
	lcd.create_char(6, UMB);
	lcd.create_char(7, LMB);
}

void LcdLogic::draw_dots(bool dot_state, uint8_t x, uint8_t y)
{
	uint8_t dot = dot_state ? 0xA5 : 0x20;
    lcd.set_cursor(x + 7, y);
    lcd.send_char(dot);
    lcd.set_cursor(x + 7, y + 1);
    lcd.send_char(dot);
}

void LcdLogic::draw_clock(uint8_t hours, uint8_t minutes, uint8_t x, uint8_t y)
{
	if (hours / 10 == 0)
		draw_dig(10, x, y);
	else
		draw_dig(hours / 10, x, y);

	lcd.set_cursor(x + 3, y);
	lcd.send_char(' ');
	lcd.set_cursor(x + 3, y + 1);
	lcd.send_char(' ');

	draw_dig(hours % 10, x + 4, y);

    draw_dig(minutes / 10, x + 8, y);

	lcd.set_cursor(x + 7, y);
	lcd.send_char(' ');
	lcd.set_cursor(x + 7, y + 1);
	lcd.send_char(' ');

    draw_dig(minutes % 10, x + 12, y);

    lcd.home();

	static bool dot_state = false;
    dot_state = not dot_state;
    draw_dots(dot_state, 0, 0);
}

void LcdLogic::draw_date(const datetime_t& datetime, uint8_t x, uint8_t y)
{
	StringStream ss;

	ss << IntWidthAction(2) << JustifyAction(Stream::Justify::Right);
	ss << datetime.day() << "." << datetime.month();
	lcd.set_cursor(x, y);
	lcd.send_string(ss.data());
	ss.clear();
	
	uint8_t int_day_of_the_week = datetime.day_of_the_week();
	auto str_week_day = week_day[int_day_of_the_week];
	ss << str_week_day;
	lcd.set_cursor(x + 1, y + 1);
	lcd.send_string(ss.data());
}

void LcdLogic::draw_co2(uint16_t co2, uint8_t x, uint8_t y)
{
	StringStream ss;
	ss << IntWidthAction(4, ' ') << JustifyAction(Stream::Justify::Left);
	ss << "C: " << co2 << "ppm";

	lcd.set_cursor(x, y);
	lcd.send_string(ss.data());
}

void LcdLogic::draw_temp(float temp, uint8_t x, uint8_t y)
{
	StringStream ss;
	DoubleFormatAction df(temp);
	df.set_digits_after_dot(1);

	ss << "T: " << df;

	lcd.set_cursor(x, y);
	lcd.send_string(ss.data());
	lcd.send_char(0xDF);
	lcd.send_char('C');
	lcd.send_char(' ');
}

void LcdLogic::draw_humidity(uint8_t hum, uint8_t x, uint8_t y)
{
	StringStream ss;

	ss << "H: " << hum << "%";

	if (ss.data().size() < 9)
		for (int i = 0; i < (10 - ss.data().size()); i++)
			ss << " ";

	lcd.set_cursor(x, y);
	lcd.send_string(ss.data());
}


void LcdLogic::draw_pressure(uint16_t press, uint8_t x, uint8_t y)
{
	StringStream ss;

	ss << IntWidthAction(3, ' ') << JustifyAction(Stream::Justify::Left);
	ss << "P: " << press << " mm";

	lcd.set_cursor(x, y);
	lcd.send_string(ss.data());
}

void LcdLogic::draw_rain_percent(int8_t percent, uint8_t x, uint8_t y)
{
	StringStream ss;
	ss << IntWidthAction(3, ' ') << JustifyAction(Stream::Justify::Right);
	ss << percent << "%      ";

	lcd.set_cursor(x, y);
	lcd.send_string(ss.data());
	//lcd->sendString(utils::stringFormat("rain %3i%%", percent));
}

void LcdLogic::main_show()
{
	load_clock();
	const datetime_t& datetime = DataStore::datetime;

	draw_clock(datetime.hour(), datetime.minute(), 0, 0);
	draw_date(datetime, 15, 0);
	draw_temp(DataStore::temperature, 0, 2);
	draw_humidity(DataStore::humidity, 11, 2);
	draw_co2(DataStore::co2, 0, 3);
	draw_pressure(DataStore::pressure, 11, 3);
	//draw_rain_percent(DataStore::disp_rain, 10, 3);
}

void LcdLogic::setup()
{
	lcd.init();
}

void LcdLogic::func()
{
	main_show();
}