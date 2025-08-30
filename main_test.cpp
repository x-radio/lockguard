#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>
#include <esp_log.h>
#include <esp_cpu.h>
#include "LockGuard.h"

#ifdef LOCKGUARD_MAIN_TEST

static const char* TAG = "MUTEX_TEST";

extern "C" void app_main() {
    SemaphoreHandle_t sys_mtx = xSemaphoreCreateRecursiveMutex();
    LockGuardMtx atomic_mtx{false};

    const int iterations = 1000;
    uint32_t start, end;
    float avg_cycles;

    start = esp_cpu_get_cycle_count();
    for (int i = 0; i < iterations; i++){
        LockGuard guard;
        guard.take(sys_mtx, 0);
    }
    end = esp_cpu_get_cycle_count();

    avg_cycles = (float)(end - start) / iterations;
    ESP_LOGI(TAG, "Average cycles with LockGuard (system mutex): %.2f", avg_cycles);    //1118 cycles

    start = esp_cpu_get_cycle_count();
    for (int i = 0; i < iterations; i++){
            LockGuard guard;
            guard.take(atomic_mtx, 0);
        }
    end = esp_cpu_get_cycle_count();

    avg_cycles = (float)(end - start) / iterations;
    ESP_LOGI(TAG, "Average cycles with LockGuard (atomic): %.2f", avg_cycles);          //91 cycles

    vSemaphoreDelete(sys_mtx);
}

#endif