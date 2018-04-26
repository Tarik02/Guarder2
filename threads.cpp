#include "threads.h"
#include <fiber.h>


void threads_main() __attribute__((weak));
void threads_loop() __attribute__((weak));

//void threads_main() {}
//void threads_loop() {}


fiber *looper = nullptr;

void threads_init() {
	fibers_init();
}

thread *thread_create(thread_cb callback, void *data, void *stack, size_t stack_size) {
	if (stack == nullptr) {
		stack = malloc(stack_size);
	}
	return reinterpret_cast<thread *>(fiber_create(callback, stack, stack_size, data));
}

thread *thread_current() {
	return reinterpret_cast<thread *>(fiber_current());
}

void thread_yield() {
	fiber_wakeup(looper);
	looper = nullptr;

	fiber_cede();
}

void thread_delay(uint32_t length) {
	uint32_t now = millis();
	while (length != 0) {
//		fiber_cede();
		thread_yield();

		uint32_t old = now;
		now = millis();
		length -= now - old;
	}
}

void thread_delay_microseconds(uint32_t length) {
	thread_delay(length / 1000);
	length %= 1000;

	uint32_t now = micros();
	while (length != 0) {
		fiber_cede();

		uint32_t old = now;
		now = micros();
		length -= now - old;
	}
}


//void setup() {
//	threads_init();
//	threads_main();
//}
//
//void loop() {
//	looper = fiber_current();
//	fiber_schedule();
//	threads_loop();
//}
