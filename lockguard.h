//=============================================================================
#ifndef LOCKGUARD_H
#define LOCKGUARD_H
//=============================================================================
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "freertos/task.h"
#include <atomic>
//=============================================================================
#define if_lockguard_return(mtx, task, timeout) LockGuard grd(task, timeout); if(!grd.take(mtx)) return;
//=============================================================================
struct LockGuardMtx
{
    std::atomic<bool> locked{false};
    TaskHandle_t owner{nullptr};
    uint32_t recursionCount{0};
};
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

    bool take(SemaphoreHandle_t &mtx, TickType_t timeout);
    bool take(SemaphoreHandle_t &mtx);
    bool take(LockGuardMtx &mtx, TickType_t timeout);
    bool take(LockGuardMtx &mtx);
};
//=============================================================================
#endif
