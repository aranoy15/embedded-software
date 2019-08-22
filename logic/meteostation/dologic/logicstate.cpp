#include <logicstate.h>

LogicState::LogicState() : mCurrentState(0) {}

void LogicState::init()
{
    button::setup();
}

void LogicState::process()
{

    if (button::isPressed()) {
        if (mCurrentState == static_cast<uint8_t>(LogicStateType::Last)) {
            mCurrentState = static_cast<uint8_t>(LogicStateType::First);
        } else {
            mCurrentState ++;
        }
    }
}