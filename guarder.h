#pragma once

#include <EEPROM.h>
#include "button.h"
#include "buzzer.h"
#include "guarder_module.h"

class Guarder {
public:
	GuarderModule *modulesBegin;
	GuarderModule *modulesEnd;
	Buzzer buzzer;
	ActionDebouncer<1> updateModulesDebouncer;

	inline Guarder(GuarderModule *modulesBegin, GuarderModule *modulesEnd, Buzzer buzzer) :
		modulesBegin(modulesBegin), modulesEnd(modulesEnd), buzzer(buzzer) {}

	void init() {
		uint16_t savePosition = 312;
		for (GuarderModule *module = modulesBegin; module != modulesEnd; ++module) {
			module->init();
			module->savePosition = savePosition;
			savePosition += 2;
		}
	}

	ActionDebouncer<50> buzzerDebouncer;
	int buzzPosition = 0;
	bool buzz = false;

	void update(uint16_t millisDiff) {
		if (updateModulesDebouncer.debounce(millisDiff)) {
			buzz = false;
			for (GuarderModule *module = modulesBegin; module != modulesEnd; ++module) {
				buzz = module->update(millisDiff) || buzz;
			}
			if (buzz) {
				buzzer.enable();
			} else {
				buzzer.disable();
			}
		}
//		buzzer.enable();

//		buzzer.update();

		if (buzz) {
			if (buzzerDebouncer.debounce(millisDiff)) {
				char notes[] = "c2g2C2C2";
				int length = sizeof(notes) / sizeof(notes[0]) / 2;

				auto note = notes[buzzPosition * 2];
				auto beat = (int) (notes[buzzPosition * 2 + 1] - '0');

				char names[] = {'c', 'd', 'e', 'f', 'g', 'a', 'b', 'C'};
				int tones[] = {1915, 1700, 1519, 1432, 1275, 1136, 1014, 956};

				for (int i = 0; i < 8; i++) {
					if (names[i] == note) {
						tone(buzzer.pin, tones[i], 1000 / beat);
						break;
					}
				}

				buzzPosition = (buzzPosition + 1) % length;
			}
		} else {
			noTone(buzzer.pin);
		}
	}

	void save() {
		for (GuarderModule *module = modulesBegin; module != modulesEnd; ++module) {
			module->save();
		}
	}

	void load() {
		for (GuarderModule *module = modulesBegin; module != modulesEnd; ++module) {
			module->load();
		}
	}
};
