#pragma once

#include <Arduino.h>

template<uint16_t Interval>
class ActionDebouncer {
public:
	uint16_t counter = 0;

	bool debounce(uint16_t diff) {
		counter += diff;
		if (counter > Interval) {
			counter -= Interval;
			return true;
		}
		return false;
	}
};
