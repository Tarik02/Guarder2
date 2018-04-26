#pragma once

#include "button.h"
#include "lamp.h"
#include "action_debouncer.h"

class GuarderModule {
public:
	enum class Status : uint8_t {
		Off = 0, // The module is turned off
		Idle = 1, // The module is waiting for trigger, then status becomes Warning
		Warning = 2, // The module is triggered
		WarningSilent = 3, // The module is triggered, but user knows

		Defer = 4, // The module is waiting for trigger, then status becomes Defer2
		Defer2 = 5 // The module is waiting for invert trigger, then status becomes Idle
	};

	Status status = Status::Off;

	Button button;
	Lamp statusLamp, warnLamp;
	Debouncer trigger;

	ActionDebouncer<500> ledDebouncer;

	uint16_t savePosition = 0;

	inline GuarderModule(Button button, Lamp statusLamp, Lamp warnLamp, Debouncer trigger)
		: button(button), statusLamp(statusLamp), warnLamp(warnLamp), trigger(trigger) {
		this->trigger.inverted = true;
	}

	void init() {
		button.init();
		statusLamp.init();
		warnLamp.init();
		trigger.init();
	}

	/**
	 * @param millisDiff
	 * @return true if need to beep
	 */
	bool update(uint16_t millisDiff) {
		ButtonState::Value buttonState = button.update(millisDiff);

		if (buttonState == ButtonState::LongClick) { // Toggle module after long click
			warnLamp.off();

			if (status == Status::Off) {
				status = Status::Idle;
				save();

				statusLamp.on();
			} else {
				status = Status::Off;
				save();

				statusLamp.off();
			}
		}

		if (status == Status::Off) {
			return false;
		}

		bool triggerState = trigger.update(millisDiff);
		switch (buttonState) {
			case ButtonState::Click:
				if (status == Status::Warning || status == Status::Defer ||
				    status == Status::Defer2) {
					status = triggerState ? Status::WarningSilent : Status::Idle;
					save();

					warnLamp.off();
					statusLamp.on();
				} else if (status == Status::WarningSilent && !triggerState) {
					status = Status::Idle;
					save();

					warnLamp.off();
					statusLamp.on();
				}
				break;
			case ButtonState::DoubleClick:
				status = Status::Defer;
				save();

				warnLamp.off();
				statusLamp.on();
				break;
			default:
				if (status == Status::Defer && triggerState) {
					status = Status::Defer2;
					save();
				} else if (status == Status::Defer2 && !triggerState) {
					status = Status::Idle;
					save();

					warnLamp.off();
					statusLamp.on();
				}
		}

		if (status == Status::Warning || status == Status::WarningSilent) {
			if (triggerState) {
				if (ledDebouncer.debounce(millisDiff)) {
					warnLamp.toggle();
				}
			} else {
				warnLamp.on();
			}
		} else if (status == Status::Defer || status == Status::Defer2) {
			if (ledDebouncer.debounce(millisDiff)) {
				statusLamp.toggle();
			}
		} else if (status == Status::Idle && triggerState) {
			status = Status::Warning;
			save();
		}

		return status == Status::Warning;
	}

	void save() {
		EEPROM.update(savePosition, static_cast<uint8_t>(status));
		EEPROM.update(savePosition + 1, ~static_cast<uint8_t>(status));
	}

	void load() {
		Status status1 = static_cast<Status>(EEPROM.read(savePosition));
		Status status2 = static_cast<Status>(~EEPROM.read(savePosition + 1));

		status = (status1 == status2 && status1 >= Status::Off && status1 <= Status::Defer2)
		         ? status1 : Status::Off;

		switch (status) {
			case Status::Off:
				statusLamp.off();
				warnLamp.off();
				break;
			case Status::Idle:
				statusLamp.on();
				warnLamp.off();
				break;
			case Status::Warning:
				statusLamp.on();
				warnLamp.on();
				break;
			case Status::WarningSilent:
				statusLamp.on();
				warnLamp.on();
				break;
			case Status::Defer:
				statusLamp.on();
				warnLamp.off();
				break;
			case Status::Defer2:
				statusLamp.on();
				warnLamp.off();
				break;
		}
	}
};
