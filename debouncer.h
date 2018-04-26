#pragma once

#include <Arduino.h>
#include "util.h"

class Debouncer {
public:
	uint8_t toUpdate = 0;
	uint16_t lastSignals = 0;

	pin_t pin;
	uint8_t time;
	bool inverted = false;

	inline explicit Debouncer(pin_t pin, uint8_t time = 5) : pin(pin), time(time) {}

	void init() {
		pinMode(pin, INPUT);
	}

	bool update(unsigned long diffMillis) {
		toUpdate += diffMillis;

		if (toUpdate >= time) {
			lastSignals = (lastSignals << 1) | (digitalRead(pin) == (inverted ? LOW : HIGH));
			toUpdate = 0;
		}

		return ~lastSignals == 0;
	}
};
