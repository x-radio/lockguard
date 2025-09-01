# LockGuard

## Overview
**LockGuard** is a small C++ utility designed to simplify synchronization in FreeRTOS-based applications.
It follows the RAII (Resource Acquisition Is Initialization) principle: once a guard object is created, it attempts to acquire the given lock, and when the guard goes out of scope, the lock is automatically released.
This approach ensures safer code by reducing boilerplate and minimizing the chance of forgetting to release a lock, which could otherwise lead to deadlocks or subtle bugs.

The class supports two locking backends:
- **FreeRTOS recursive semaphores** – full-featured, system-managed synchronization primitives.
- **Atomic mutex** – an extremely lightweight lock built on top of `std::atomic<bool>`, designed for cases where performance matters and contention is low.

In addition, LockGuard supports recursion (the same task can acquire the lock multiple times), optional task association, and simple timeout semantics.

## Features
- **RAII-based** – automatic acquisition and release tied to object lifetime
- **Recursive locking** – safe re-entry by the same task
- **Atomic mutex support** – efficient, low-overhead locking with cooperative yielding
- **FreeRTOS semaphore support** – integration with `xSemaphoreTakeRecursive` / `GiveRecursive`
- **Timeouts** – immediate try (`0`) or wait indefinitely (`portMAX_DELAY`)
- **Scoped design** – encourages clear, maintainable synchronization patterns

## Purpose
- Ensure safe and consistent access to shared resources in multitasking environments
- Simplify synchronization by hiding low-level API details
- Reduce human error in lock/unlock sequences
- Provide flexibility between system semaphores and lightweight atomic locks depending on performance needs

## License
*MIT License*
