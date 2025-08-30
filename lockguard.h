//=============================================================================
#ifndef LOCKGUARD_H
#define LOCKGUARD_H
//=============================================================================
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "freertos/task.h"
#include <atomic>
//=============================================================================
typedef std::atomic<bool> LockGuardMtx;
//=============================================================================
class LockGuard
{
    SemaphoreHandle_t*  mtx = nullptr;
    LockGuardMtx*       amtx = nullptr;
    TaskHandle_t        task;
    TickType_t          timeout;

    public:
    LockGuard(TaskHandle_t task = NULL, TickType_t timeout = portMAX_DELAY);
    ~LockGuard();

    bool ok(TaskHandle_t task, SemaphoreHandle_t &mtx);
    bool take(SemaphoreHandle_t &mtx, TickType_t timeout);
    bool ok(TaskHandle_t task, LockGuardMtx &mtx);
    bool take(LockGuardMtx &mtx, TickType_t timeout);
};
//=============================================================================
#endif
