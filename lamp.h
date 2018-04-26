#pragma once

#include <Arduino.h>
#include "util.h"

class Lamp {
public:
	pin_t pin;

	inline explicit Lamp(pin_t pin) : pin(pin) {}

	inline void init() {
		pinMode(pin, OUTPUT);
	}

	inline void on() {
		digitalWrite(pin, HIGH);
	}

	inline void off() {
		digitalWrite(pin, LOW);
	}

	inline bool get() {
		return digitalRead(pin) == HIGH;
	}

	inline void set(bool status) {
		if (status) {
			on();
		} else {
			off();
		}
	}

	inline void toggle() {
		set(!get());
	}
};
