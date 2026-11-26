#include <Arduino.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/semphr.h"

SemaphoreHandle_t binarySemaphore;

// Producer Task
void BinaryProducerTask(void *pvParameters) {
  while (1) {
    Serial.println("Producer: Giving Semaphore");

    xSemaphoreGive(binarySemaphore);

    vTaskDelay(pdMS_TO_TICKS(2000));
  }
}

// Consumer Task
void BinaryConsumerTask(void *pvParameters) {
  while (1) {
    if (xSemaphoreTake(binarySemaphore, portMAX_DELAY)) {
      Serial.println("Consumer: Semaphore Received");
    }
  }
}

void setup() {
  Serial.begin(115200);

  binarySemaphore = xSemaphoreCreateBinary();

  if (binarySemaphore != NULL) {

    xTaskCreate(
      BinaryProducerTask,
      "Producer",
      2048,
      NULL,
      2,
      NULL
    );

    xTaskCreate(
      BinaryConsumerTask,
      "Consumer",
      2048,
      NULL,
      1,
      NULL
    );
  }
}

void loop() {
  // FreeRTOS tasks handle the operations
}
