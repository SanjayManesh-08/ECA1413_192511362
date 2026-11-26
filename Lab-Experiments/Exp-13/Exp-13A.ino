#include <Arduino.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

QueueHandle_t dataQueue;

// Producer task
void ProducerTask(void *pvParameters)
{
  uint32_t data = 100;

  while (1)
  {
    data += 10;

    if (xQueueSend(dataQueue, &data,
                   pdMS_TO_TICKS(100)) == pdPASS)
    {
      Serial.printf("[PRODUCER] Data sent: %lu\n",
                    (unsigned long)data);
    }
    else
    {
      Serial.println("[PRODUCER] Queue full; data not sent");
    }

    vTaskDelay(pdMS_TO_TICKS(1000));
  }
}

// Consumer task
void ConsumerTask(void *pvParameters)
{
  uint32_t receivedData;

  while (1)
  {
    if (xQueueReceive(dataQueue, &receivedData,
                      portMAX_DELAY) == pdPASS)
    {
      Serial.printf("[CONSUMER] Data received: %lu\n",
                    (unsigned long)receivedData);

      Serial.println("---------------------------");

      // Simulate processing time
      vTaskDelay(pdMS_TO_TICKS(1500));
    }
  }
}

void setup()
{
  Serial.begin(115200);
  delay(1000);

  Serial.println();
  Serial.println("PART A: FREERTOS MESSAGE QUEUE");

  // Create queue: 5 items, each 32 bits
  dataQueue = xQueueCreate(5, sizeof(uint32_t));

  if (dataQueue == NULL)
  {
    Serial.println("ERROR: Queue creation failed");

    while (1)
    {
      vTaskDelay(pdMS_TO_TICKS(1000));
    }
  }

  Serial.println("Message queue created successfully");

  xTaskCreate(ProducerTask, "Producer",
              4096, NULL, 2, NULL);

  xTaskCreate(ConsumerTask, "Consumer",
              4096, NULL, 1, NULL);
}

void loop()
{
  vTaskDelay(pdMS_TO_TICKS(1000));
}
