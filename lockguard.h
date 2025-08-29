//=============================================================================
#ifndef LOCKGUARD_H
#define LOCKGUARD_H
//=============================================================================
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "freertos/task.h"
//=============================================================================
class LockGuard
{
    SemaphoreHandle_t   *mtx = nullptr;
    TaskHandle_t        task;
    TickType_t          timeout;

    public:
    LockGuard(TaskHandle_t task = NULL, TickType_t timeout = portMAX_DELAY);
    ~LockGuard();

    bool ok(TaskHandle_t task, SemaphoreHandle_t &mtx);
    bool take(SemaphoreHandle_t &mtx, TickType_t timeout);
};
//=============================================================================
#endif
