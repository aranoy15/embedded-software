//
// Created by Антон Федосеев on 07.02.2021.
//

#ifndef TEST_TIMER_HPP
#define TEST_TIMER_HPP

#include <bsp.hpp>

namespace lib::timer
{
class Timer final
{
public:
    Timer();
    ~Timer() = default;

    static constexpr uint32_t millisecond(uint32_t ms) { return ms; }
    static constexpr uint32_t second(uint32_t s)
    {
        return s * millisecond(1000);
    }
    static constexpr uint32_t minute(uint32_t m) { return m * second(60); }

    void start();
    void stop();
    uint32_t elapsed();
    bool has_expired(uint32_t tick);

private:
    uint32_t _current;
    bool _is_started = false;
};
}

#endif  // TEST_TIMER_HPP
