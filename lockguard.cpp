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
    if(mtx) xSemaphoreGiveRecursive(*mtx);
}
//---------------------------
bool LockGuard::ok(TaskHandle_t task, SemaphoreHandle_t &mtx)
{
    if(this->task == task) return true;
    if(!xSemaphoreTakeRecursive(mtx, timeout)) return false;
    this->mtx = &mtx;
    return true;
}
//---------------------------
bool LockGuard::take(SemaphoreHandle_t &mtx, TickType_t timeout)
{
    if(!xSemaphoreTakeRecursive(mtx, timeout)) return false;
    this->mtx = &mtx;
    return true;
}
//=============================================================================
