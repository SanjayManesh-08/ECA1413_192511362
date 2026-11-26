#include <Arduino.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"
 
SemaphoreHandle_t countingSemaphore;
 
void CountingProducerTask(void *pvParameters) {
  int eventNumber = 0;
  while (1) {
    eventNumber++;
    if (xSemaphoreGive(countingSemaphore) == pdTRUE)
      Serial.printf("[COUNTING] Event %d generated; count = %lu\n",
                    eventNumber,
                    (unsigned long)uxSemaphoreGetCount(countingSemaphore));
    else
      Serial.println("[COUNTING] Semaphore full; signal not added");
    vTaskDelay(pdMS_TO_TICKS(1000));
  }
}
 
void CountingConsumerTask(void *pvParameters) {
  while (1) {
    if (xSemaphoreTake(countingSemaphore, portMAX_DELAY) == pdTRUE) {
      Serial.printf("[COUNTING] Signal consumed; remaining = %lu\n",
                    (unsigned long)uxSemaphoreGetCount(countingSemaphore));
      vTaskDelay(pdMS_TO_TICKS(2500));
    }
  }
}
 
void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("COUNTING SEMAPHORE");
  countingSemaphore = xSemaphoreCreateCounting(3, 0);
  if (countingSemaphore == NULL) {
    Serial.println("ERROR: Counting semaphore creation failed");
    while (1) vTaskDelay(pdMS_TO_TICKS(1000));
  }
  xTaskCreate(CountingProducerTask, "CntProd", 4096, NULL, 2, NULL);
  xTaskCreate(CountingConsumerTask, "CntCons", 4096, NULL, 1, NULL);
}
 
void loop() {
  vTaskDelay(pdMS_TO_TICKS(1000));
}
