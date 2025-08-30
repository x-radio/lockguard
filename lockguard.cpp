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
    if(amtx) amtx->store(false, std::memory_order_release);
    else if(mtx) xSemaphoreGiveRecursive(*mtx);
}
//---------------------------
bool LockGuard::ok(TaskHandle_t task, SemaphoreHandle_t &mtx)
{
    if(this->task == task) return true;
    return take(mtx, timeout);
}
//---------------------------
bool LockGuard::take(SemaphoreHandle_t &mtx, TickType_t timeout)
{
    if(!xSemaphoreTakeRecursive(mtx, timeout)) return false;
    this->mtx = &mtx;
    return true;
}
//---------------------------
bool LockGuard::ok(TaskHandle_t task, LockGuardMtx &mtx)
{
    if(this->task == task) return true;
    return take(mtx, timeout);
}
//---------------------------
bool LockGuard::take(LockGuardMtx &mtx, TickType_t timeout)
{
    bool expected = false;
    if(timeout == 0){
        if(!mtx.compare_exchange_strong(expected, true, std::memory_order_acquire))
            return false;
    }else{
        assert(timeout == portMAX_DELAY);
        while (!mtx.compare_exchange_strong(expected, true, std::memory_order_acquire)){
            expected = false;
            taskYIELD();
    }}

    this->amtx = &mtx;
    return true;
}
//=============================================================================
