#include <Arduino.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "freertos/stream_buffer.h"
StreamBufferHandle_t streamBuffer;
// Producer task
void ProducerTask(void *pvParameters)
{
  uint8_t sequenceNumber = 0;
  while (1)
  {    char message[6];
    // Generate a five-character data sequence
    message[0] = 'A' + (sequenceNumber % 26);
    message[1] = 'A' + ((sequenceNumber + 1) % 26);
    message[2] = 'A' + ((sequenceNumber + 2) % 26);
    message[3] = 'A' + ((sequenceNumber + 3) % 26);
    message[4] = '\n';
    message[5] = '\0';
    size_t bytesSent = xStreamBufferSend(
      streamBuffer,
      message,
      5,
      pdMS_TO_TICKS(100)
    );

    if (bytesSent == 5)
    {
      Serial.printf("[PIPE] Producer sent: %s", message);
    }
    else
    {
      Serial.printf("[PIPE] Partial send: %u bytes\n",
                    (unsigned int)bytesSent);
    }

    sequenceNumber++;
    vTaskDelay(pdMS_TO_TICKS(1000));
  }
}

// Consumer task
void ConsumerTask(void *pvParameters)
{
  char receivedData[17];

  while (1)
  {
    size_t bytesReceived = xStreamBufferReceive(
      streamBuffer,
      receivedData,
      sizeof(receivedData) - 1,
      portMAX_DELAY
    );

    if (bytesReceived > 0)
    {
      receivedData[bytesReceived] = '\0';

      Serial.printf("[PIPE] Consumer received %u bytes: %s",
                    (unsigned int)bytesReceived,
                    receivedData);

      Serial.println("---------------------------");

      // Simulate data processing
      vTaskDelay(pdMS_TO_TICKS(1500));
    }
  }
}

void setup()
{
  Serial.begin(115200);
  delay(1000);

  Serial.println();
  Serial.println("PART C: FREERTOS STREAM BUFFER");

  // Create a 32-byte stream buffer.
  // Trigger level = 1 byte.
  streamBuffer = xStreamBufferCreate(32, 1);

  if (streamBuffer == NULL)
  {
    Serial.println("ERROR: Stream buffer creation failed");

    while (1)
    {
      vTaskDelay(pdMS_TO_TICKS(1000));
    }
  }

  Serial.println("Stream buffer created successfully");

  xTaskCreate(ProducerTask, "Producer",
              4096, NULL, 2, NULL);

  xTaskCreate(ConsumerTask, "Consumer",
              4096, NULL, 1, NULL);
}

void loop()
{
  vTaskDelay(pdMS_TO_TICKS(1000));
}
