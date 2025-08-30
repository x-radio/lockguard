# LockGuard

## Overview
**LockGuard** is a lightweight RAII-style wrapper for FreeRTOS recursive semaphores and yielding atomic locks (`std::atomic<bool>`), designed to simplify task-safe access to shared resources in C++ projects. It automatically acquires a semaphore or yielding atomic lock on construction and releases it on destruction, reducing boilerplate and minimizing the risk of deadlocks.

## Purpose
- Ensures safe, scoped access to shared resources
- Reduces manual semaphore or atomic lock handling and errors
- Supports optional task-specific locking and timeout control

## Features
- **RAII-based**: Automatically acquires/releases the mutex or atomic lock
- **Recursive semaphore support**: Works with FreeRTOS recursive semaphores
- **Yielding atomic lock support**: Works with `std::atomic<bool>` for lightweight, task-friendly locking
- **Task-aware**: Can conditionally acquire based on current task
- **Timeout support**: Specify max wait time for semaphore or atomic lock acquisition
- **Flexible API**: Provides `ok()` and `take()` methods for explicit control

## License
MIT License
