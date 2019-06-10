#pragma once

#include <gpio.h>
#include <timer.h>

template<class Pin>
struct Button
{
	static void setup() { Pin::setup(); }

	static bool isPressed()
	{
		if (Pin::state()) {
			m_timer.start(50);

			while (not m_timer.elapsed()) {
				if (not Pin::state())
					return true;

				Time::sleep(Time(1));
			}

			return false;
		} else return false;
		
	}

private:
	static Timer m_timer;
};

template<class Pin>
Timer Button<Pin>::m_timer = Timer();