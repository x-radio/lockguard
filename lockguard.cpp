//=============================================================================
#define LOCKGUARD_CPP
//=============================================================================
#include "lockguard.h"
//=============================================================================
LockGuard::LockGuard(TaskHandle_t task, TickType_t timeout)
{
    this->task = task;
    this->timeout = timeout;
}
//---------------------------
LockGuard::~LockGuard()
{
    if(amtx) {
        if(--amtx->recursionCount != 0)  return;
        amtx->owner = nullptr;
        amtx->locked.store(false, std::memory_order_release);
    }
    else if(mtx) xSemaphoreGiveRecursive(*mtx);
}
//---------------------------
bool LockGuard::take(SemaphoreHandle_t &mtx, TickType_t timeout)
{
    this->timeout = timeout;
    take(mtx);
    return true;
}
//---------------------------
bool LockGuard::take(SemaphoreHandle_t &mtx)
{
    if(!xSemaphoreTakeRecursive(mtx, timeout)) return false;
    this->mtx = &mtx;
    return true;
}
//---------------------------
bool LockGuard::take(LockGuardMtx &mtx, TickType_t timeout)
{
    this->timeout = timeout;
    take(mtx);
    return true;
}
//---------------------------
bool LockGuard::take(LockGuardMtx &mtx)
{
    if(task == NULL) task = xTaskGetCurrentTaskHandle();

    if(mtx.owner == task){
        mtx.recursionCount++;
        this->amtx = &mtx;
        return true;
    }

    if(timeout == 0){
        bool expected = false;
        if(!mtx.locked.compare_exchange_strong(expected, true, std::memory_order_acquire))
            return false;
    }else{
        assert(timeout == portMAX_DELAY && "LockGuardMtx supports only 0 or MAX_DELAY");
        bool expected = false;
        while(!mtx.locked.compare_exchange_strong(expected, true, std::memory_order_acquire)){
            expected = false;
            taskYIELD();
        }
    }

    mtx.recursionCount = 1;
    this->amtx = &mtx;
    mtx.owner = task;
    return true;
}
//=============================================================================
