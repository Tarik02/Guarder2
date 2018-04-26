#pragma once

int serialCounter = 0;

void markSerial() {
	Serial.print(serialCounter++);
	Serial.print(": ");
}
