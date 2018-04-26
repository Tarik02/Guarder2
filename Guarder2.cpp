#include <Arduino.h>

#include "button.h"
#include "serial.h"
#include "sound.h"
#include "buzzer.h"
#include "threads.h"
#include "guarder.h"
#include "guarder_module.h"

const uint16_t MODULE_DEBOUNCER = 70;

unsigned long lastUpdate = 0;
uint16_t millisDiff = 0;

GuarderModule modules[] = {
	{Button(A5), Lamp(2),  Lamp(3),  Debouncer(A0, MODULE_DEBOUNCER)},
	{Button(A4), Lamp(4),  Lamp(5),  Debouncer(A1, MODULE_DEBOUNCER)},
	{Button(6),  Lamp(7),  Lamp(8),  Debouncer(A2, MODULE_DEBOUNCER)},
	{Button(9),  Lamp(10), Lamp(11), Debouncer(A3, MODULE_DEBOUNCER)},
};
Guarder guarder(modules, modules + (sizeof(modules) / sizeof(modules[0])),
                Buzzer(12, guarder_ringtone));

void setup() {
	pinMode(13, OUTPUT);

	guarder.init();
	guarder.load();
}

void loop() {
	unsigned long now = millis();
	millisDiff = static_cast<uint16_t>(now - lastUpdate);
	lastUpdate = now;

	guarder.update(millisDiff);
}
