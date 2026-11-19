#include <Arduino.h>

#define BUTTON_PIN 0

TaskHandle_t InterruptTaskHandle;

volatile int interruptCount = 0;

// Interrupt Service Routine
void IRAM_ATTR ButtonISR()
{
  interruptCount++;

  BaseType_t xHigherPriorityTaskWoken = pdFALSE;

  vTaskNotifyGiveFromISR(
    InterruptTaskHandle,
    &xHigherPriorityTaskWoken
  );

  if (xHigherPriorityTaskWoken)
  {
    portYIELD_FROM_ISR();
  }
}

// High Priority Task
void InterruptTask(void *parameter)
{
  while (1)
  {
    // Wait for interrupt notification
    ulTaskNotifyTake(
      pdTRUE,
      portMAX_DELAY
    );

    Serial.print("HIGH PRIORITY TASK -> Interrupt Count: ");
    Serial.println(interruptCount);

    Serial.println("Deferred processing executed.");
    Serial.println("------------------------------");
  }
}

// Low Priority Background Task
void BackgroundTask(void *parameter)
{
  while (1)
  {
    Serial.println("LOW PRIORITY TASK -> Background processing");

    vTaskDelay(pdMS_TO_TICKS(1000));
  }
}

void setup()
{
  Serial.begin(115200);
  delay(1000);

  pinMode(BUTTON_PIN, INPUT_PULLUP);

  Serial.println("=================================");
  Serial.println(" ISR PRIORITY AND DEFERRED PROCESSING");
  Serial.println("=================================");

  // High priority interrupt processing task
  xTaskCreate(
    InterruptTask,
    "InterruptTask",
    2048,
    NULL,
    3,
    &InterruptTaskHandle
  );

  // Low priority background task
  xTaskCreate(
    BackgroundTask,
    "BackgroundTask",
    2048,
    NULL,
    1,
    NULL
  );

  // Attach interrupt
  attachInterrupt(
    digitalPinToInterrupt(BUTTON_PIN),
    ButtonISR,
    FALLING
  );

  Serial.println("Interrupt configured on GPIO 0.");
  Serial.println("Interrupt Task Priority = 3");
  Serial.println("Background Task Priority = 1");
  Serial.println("Press the BOOT button.");
  Serial.println("=================================");
}

void loop()
{
  delay(1000);
}
