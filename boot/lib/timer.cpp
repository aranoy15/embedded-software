//
// Created by Антон Федосеев on 07.02.2021.
//

#include "timer.h"

using namespace lib::timer;

void Timer::start()
{
    _current = bsp::tick::current();
    _is_started = true;
}

void Timer::stop()
{
    _is_started = false;
}

uint32_t Timer::elapsed()
{
    if (not _is_started) return 0;

    return bsp::tick::current() - _current;
}

bool Timer::has_expired(uint32_t tick)
{
    if (not _is_started) return false;

    return elapsed() >= tick;
}
