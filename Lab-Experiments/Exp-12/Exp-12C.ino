#include <Arduino.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
 
SemaphoreHandle_t counterMutex;
int sharedCounter = 0;
 
void CounterTask1(void *pvParameters) {
  while (1) {
    if (xSemaphoreTake(counterMutex, portMAX_DELAY) == pdTRUE) {
      sharedCounter++;
      Serial.printf("[MUTEX] Task 1 counter = %d\n", sharedCounter);
      xSemaphoreGive(counterMutex);
    }
    vTaskDelay(pdMS_TO_TICKS(500));
  }
}
 
void CounterTask2(void *pvParameters) {
  while (1) {
    if (xSemaphoreTake(counterMutex, portMAX_DELAY) == pdTRUE) {
      sharedCounter++;
      Serial.printf("[MUTEX] Task 2 counter = %d\n", sharedCounter);
      xSemaphoreGive(counterMutex);
    }
    vTaskDelay(pdMS_TO_TICKS(700));
  }
}
 
void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("MUTEX - SHARED DATA PROTECTION");
  counterMutex = xSemaphoreCreateMutex();
  if (counterMutex == NULL) {
    Serial.println("ERROR: Mutex creation failed");
    while (1) vTaskDelay(pdMS_TO_TICKS(1000));
  }
  xTaskCreate(CounterTask1, "Counter1", 4096, NULL, 1, NULL);
  xTaskCreate(CounterTask2, "Counter2", 4096, NULL, 1, NULL);
}
 
void loop() {
  vTaskDelay(pdMS_TO_TICKS(1000));
}
