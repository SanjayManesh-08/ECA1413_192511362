#include <Arduino.h>

SemaphoreHandle_t resourceMutex;

// Task handles
TaskHandle_t LowTaskHandle;
TaskHandle_t MediumTaskHandle;
TaskHandle_t HighTaskHandle;

// Low Priority Task
void LowTask(void *parameter)
{
  // Allow other tasks to start
  vTaskDelay(pdMS_TO_TICKS(100));

  Serial.println("LOW -> Trying to lock shared resource");

  xSemaphoreTake(resourceMutex, portMAX_DELAY);

  Serial.println("LOW -> Shared resource LOCKED");
  Serial.println("LOW -> Current Priority: 1");

  // Hold the resource and perform work
  for (int i = 1; i <= 10; i++)
  {
    Serial.print("LOW -> Working... Current Priority: ");
    Serial.println(uxTaskPriorityGet(NULL));

    vTaskDelay(pdMS_TO_TICKS(100));
  }

  Serial.println("LOW -> Releasing shared resource");

  xSemaphoreGive(resourceMutex);

  Serial.print("LOW -> Priority after release: ");
  Serial.println(uxTaskPriorityGet(NULL));

  vTaskDelay(pdMS_TO_TICKS(1000));

  vTaskDelete(NULL);
}

// Medium Priority Task
void MediumTask(void *parameter)
{
  vTaskDelay(pdMS_TO_TICKS(400));

  while (1)
  {
    Serial.println("MEDIUM -> Running background task");

    vTaskDelay(pdMS_TO_TICKS(200));
  }
}

// High Priority Task
void HighTask(void *parameter)
{
  vTaskDelay(pdMS_TO_TICKS(300));

  Serial.println("HIGH -> Trying to lock shared resource");

  xSemaphoreTake(resourceMutex, portMAX_DELAY);

  Serial.println("HIGH -> Shared resource LOCKED");
  Serial.println("HIGH -> Resource processing complete");

  xSemaphoreGive(resourceMutex);

  Serial.println("HIGH -> Shared resource RELEASED");

  vTaskDelete(NULL);
}

void setup()
{
  Serial.begin(115200);
  delay(1000);

  Serial.println("========================================");
  Serial.println(" PRIORITY INHERITANCE TEST");
  Serial.println("========================================");

  resourceMutex = xSemaphoreCreateMutex();

  if (resourceMutex == NULL)
  {
    Serial.println("Mutex creation failed!");
    return;
  }

  Serial.println("Mutex created successfully.");

  // LOW = Priority 1
  xTaskCreatePinnedToCore(
    LowTask,
    "LowTask",
    2048,
    NULL,
    1,
    &LowTaskHandle,
    1
  );

  // MEDIUM = Priority 2
  xTaskCreatePinnedToCore(
    MediumTask,
    "MediumTask",
    2048,
    NULL,
    2,
    &MediumTaskHandle,
    1
  );

  // HIGH = Priority 3
  xTaskCreatePinnedToCore(
    HighTask,
    "HighTask",
    2048,
    NULL,
    3,
    &HighTaskHandle,
    1
  );

  Serial.println("LOW    Priority = 1");
  Serial.println("MEDIUM Priority = 2");
  Serial.println("HIGH   Priority = 3");
  Serial.println("All tasks are running on Core 1.");
  Serial.println("========================================");
}

void loop()
{
  delay(1000);
}
