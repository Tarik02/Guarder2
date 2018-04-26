#pragma once

#include <Arduino.h>
#include "debouncer.h"
#include "util.h"

#ifndef BUTTON_SHORT_CLICK_MILLIS
#define BUTTON_SHORT_CLICK_MILLIS 60
#endif

#ifndef BUTTON_LONG_CLICK_MILLIS
#define BUTTON_LONG_CLICK_MILLIS 600
#endif

#ifndef BUTTON_DOUBLE_CLICK_MILLIS
#define BUTTON_DOUBLE_CLICK_MILLIS 300
#endif

namespace ButtonState {
	enum Value {
		Idle, Click, LongClick, DoubleClick
	};
}

class Button {
public:
	pin_t pin;
	Debouncer debouncer;

	unsigned short touchTime = 0;
	unsigned short waitingForDouble = 0;

	inline explicit Button(pin_t pin) : pin(pin), debouncer(pin, 1) {}

	void init() {
		debouncer.init();
	}

	ButtonState::Value update(unsigned long millisDiff) {
		ButtonState::Value state = ButtonState::Idle;
		bool touched = debouncer.update(millisDiff);

		if (touched) {
			if (waitingForDouble != 0) {
				waitingForDouble = -1;
			} else {
				touchTime += millisDiff;
			}
		} else {
			if (touchTime != 0) {
				if (touchTime >= BUTTON_LONG_CLICK_MILLIS) {
					state = ButtonState::LongClick;
				} else if (touchTime >= BUTTON_SHORT_CLICK_MILLIS) {
					waitingForDouble = 1;
				}
			} else if (waitingForDouble != 0) {
				if (waitingForDouble == -1) {
					waitingForDouble = 0;
					state = ButtonState::DoubleClick;
				} else {
					waitingForDouble += millisDiff;
					if (waitingForDouble > BUTTON_DOUBLE_CLICK_MILLIS) {
						waitingForDouble = 0;
						state = ButtonState::Click;
					}
				}
			}

			touchTime = 0;
		}

		return state;
	}
};
