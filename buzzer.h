#pragma once

#include <Arduino.h>
//#include <NewTone.h>
#include <rtttl.h>
#include "threads.h"
#include "sound.h"
#include "util.h"

class Buzzer {
public:
	pin_t pin;
	ProgmemPlayer player;
//	RamPlayer player;
	bool active = false;

	inline Buzzer(pin_t pin, const char *ringtone) : pin(pin), player(pin) {
		player.setSong(ringtone);
	}

	void init() {
		pinMode(pin, OUTPUT);
	}

	void enable() {
		if (!active) {
			active = true;
//			player.initSong();
		}
	}

	void disable() {
		if (active) {
			active = false;
//			player.silence();
		}
	}

	void update() {
		if (!active) return;
		player.pollSong();
	}
};
