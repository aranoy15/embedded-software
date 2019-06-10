#pragma once

enum class PinManageLogic 
{
	Inverse = true,
	Straight = false
};

template<class Pin, PinManageLogic = PinManageLogic::Straight>
struct Relay
{
	static void setup() { Pin::setup(); }
	static void on() { Pin::on(); }
	static void off() { Pin::off(); }
	static bool state() { return Pin::state(); }
};

template<class Pin>
struct Relay<Pin, PinManageLogic::Inverse>
{
	static void setup() { Pin::setup(); }
	static void on() { Pin::off(); }
	static void off() { Pin::on(); }
	static bool state() { return not Pin::state(); }
};