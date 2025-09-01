#if 0

#include <freertos/FreeRTOS.h>
#include <freertos/semphr.h>
#include <esp_log.h>
#include <esp_cpu.h>
#include "LockGuard.h"

static const char* TAG = "MUTEX_TEST";

extern "C" void app_main() {
    SemaphoreHandle_t sys_mtx = xSemaphoreCreateRecursiveMutex();
    LockGuardMtx atomic_mtx;

    const int iterations = 1000;
    uint32_t start, end;
    float avg_cycles;

    start = esp_cpu_get_cycle_count();
    for (int i = 0; i < iterations; i++){
        LockGuard guard;
        guard.take(sys_mtx);
    }
    end = esp_cpu_get_cycle_count();

    avg_cycles = (float)(end - start) / iterations;
    ESP_LOGI(TAG, "LockGuard (system mutex): %.2f", avg_cycles);                //1118 cycles

    TaskHandle_t task = xTaskGetCurrentTaskHandle();                            //51 cycles
    start = esp_cpu_get_cycle_count();
    for (int i = 0; i < iterations; i++){
            LockGuard guard(task, 0);
            guard.take(atomic_mtx);
        }
    end = esp_cpu_get_cycle_count();

    avg_cycles = (float)(end - start) / iterations;
    ESP_LOGI(TAG, "LockGuard (atomic): %.2f", avg_cycles);                      //111 cycles

    start = esp_cpu_get_cycle_count();
    for (int i = 0; i < iterations; i++){
            LockGuard guard(task, 0);
            guard.take(atomic_mtx, 0);
        }
    end = esp_cpu_get_cycle_count();

    avg_cycles = (float)(end - start) / iterations;
    ESP_LOGI(TAG, "LockGuard (atomic) - take(mtx, 0): %.2f", avg_cycles);       //121 cycles

    vSemaphoreDelete(sys_mtx);
}

#endif

#if 0

#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "lockguard.h"

LockGuardMtx testMtx;

//---------------------------
void task1(void *pv)
{
    while (1) {
        vTaskDelay(pdMS_TO_TICKS(1000));
        LockGuard g;
        if (g.take(testMtx)) {
            printf("task1: in critical section\n");
            {
                LockGuard g2;
                if (g2.take(testMtx)) {
                    printf("task1: recursive lock ok\n");
                    {
                        LockGuard g3;
                        if (g3.take(testMtx)) {
                            printf("task1: recursive lock ok\n");
                            vTaskDelay(pdMS_TO_TICKS(500));
                        }
                    } // g3 release
                }
            } // g2 release
        }
    } // g release
}
//---------------------------
void task2(void *pv)
{
    while (1) {
        vTaskDelay(pdMS_TO_TICKS(800));
        {
            LockGuard g(0, portMAX_DELAY);
            printf("task2: waiting for lock (MAX_DELAY)\n");
            if (g.take(testMtx)) {
                printf("task2: acquired lock after wait\n");
            }
        } // g release
    }
}
//---------------------------
void task3(void *pv)
{
    while (1) {
        vTaskDelay(pdMS_TO_TICKS(900));
        {
            LockGuard g(0, 0);
            if (!g.take(testMtx)) {
                printf("task3: immediate try failed (timeout=0)\n");
            }else{
                printf("task3: acquired lock (timeout=0)\n");
            }
        } // g release
    }
}
//---------------------------
extern "C" void app_main(void)
{
    xTaskCreatePinnedToCore(task1, "Task1", 4096, NULL, 5, NULL, 0);
    xTaskCreatePinnedToCore(task2, "Task2", 4096, NULL, 5, NULL, 1);
    xTaskCreatePinnedToCore(task3, "Task3", 4096, NULL, 5, NULL, 1);
}

#endif