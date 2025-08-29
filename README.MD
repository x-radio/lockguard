# LockGuard

## Overview
**LockGuard** is a lightweight RAII-style wrapper for FreeRTOS recursive semaphores, designed to simplify task-safe access to shared resources in C++ projects. It automatically acquires a semaphore on construction and releases it on destruction, reducing boilerplate and minimizing risk of deadlocks.

## Purpose
- Ensures safe, scoped access to shared resources
- Reduces manual semaphore handling and errors
- Supports optional task-specific locking and timeout control

## Features
- **RAII-based**: Automatically acquires/releases the mutex
- **Recursive semaphore support**: Works with FreeRTOS recursive semaphores
- **Task-aware**: Can conditionally acquire based on current task
- **Timeout support**: Specify max wait time for semaphore acquisition
- **Flexible API**: Provides `ok()`, `take()` methods for explicit control

## License
MIT License
