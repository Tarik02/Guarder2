#pragma once

#include <Arduino.h>

/**
 * There are a bug, the application freezes when there's no threads
 */

#ifndef FIBER_STACK_SIZE
#define FIBER_STACK_SIZE 128
//#define FIBER_STACK_SIZE 64
#endif

#ifndef THREAD_STACK_SIZE
#define THREAD_STACK_SIZE FIBER_STACK_SIZE
#endif

#include <fiber.h>

typedef void (*thread_cb)(void *);
struct thread;

void threads_init();

thread *thread_create(thread_cb callback, void *data = nullptr, void *stack = nullptr,
                      size_t stack_size = THREAD_STACK_SIZE);
thread *thread_current();

void thread_yield();

void thread_delay(uint32_t length);
void thread_delay_microseconds(uint32_t length);

#define THREAD_CREATE_STACK(name, size) static char name[size];

#define thread_create_with_data(cb, data) do { \
    THREAD_CREATE_STACK(cb ## _stack, THREAD_STACK_SIZE) \
    thread_create(cb, data, cb ## _stack, sizeof(cb ## _stack)); \
} while (false);

#define thread_create_void(cb) thread_create_with_data(cb, nullptr)

