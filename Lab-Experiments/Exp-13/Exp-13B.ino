#include <Arduino.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/queue.h"

QueueHandle_t mailbox;

// Producer task
void ProducerTask(void *pvParameters)
{
  uint32_t message = 1000;

  while (1)
  {
    message += 100;

    if (xQueueSend(mailbox, &message,
                   pdMS_TO_TICKS(500)) == pdPASS)
    {
      Serial.printf("[MAILBOX] Message sent: %lu\n",
                    (unsigned long)message);
    }
    else
    {
      Serial.println("[MAILBOX] Slot occupied; message not sent");
    }

    vTaskDelay(pdMS_TO_TICKS(1000));
  }
}

// Consumer task
void ConsumerTask(void *pvParameters)
{
  uint32_t receivedMessage;

  while (1)
  {
    if (xQueueReceive(mailbox, &receivedMessage,
                      portMAX_DELAY) == pdPASS)
    {
      Serial.printf("[MAILBOX] Message received: %lu\n",
                    (unsigned long)receivedMessage);

      Serial.println("---------------------------");

      // Simulate message processing
      vTaskDelay(pdMS_TO_TICKS(2000));
    }
  }
}

void setup()
{
  Serial.begin(115200);
  delay(1000);

  Serial.println();
  Serial.println("PART B: FREERTOS MAILBOX");
  
  // Mailbox-style queue: one item, uint32_t size
  mailbox = xQueueCreate(1, sizeof(uint32_t));

  if (mailbox == NULL)
  {
    Serial.println("ERROR: Mailbox creation failed");

    while (1)
    {
      vTaskDelay(pdMS_TO_TICKS(1000));
    }
  }

  Serial.println("One-item mailbox created successfully");

  xTaskCreate(ProducerTask, "Producer",
              4096, NULL, 2, NULL);

  xTaskCreate(ConsumerTask, "Consumer",
              4096, NULL, 1, NULL);
}

void loop()
{
  vTaskDelay(pdMS_TO_TICKS(1000));
}
