#include <Arduino.h>

// Task handles
TaskHandle_t HighTaskHandle   = NULL;
TaskHandle_t MediumTaskHandle = NULL;
TaskHandle_t LowTaskHandle    = NULL;

// Execution counters
volatile unsigned long highCount   = 0;
volatile unsigned long mediumCount = 0;
volatile unsigned long lowCount    = 0;

// High Priority Task (Priority 3, 500 ms)
void HighTask(void *parameter)
{
  // Allow setup() to finish printing before the tasks start
  vTaskDelay(pdMS_TO_TICKS(100));

  while (1)
  {
    highCount++;

    Serial.print("HIGH   | Priority: ");
    Serial.print(uxTaskPriorityGet(NULL));
    Serial.print(" | Count: ");
    Serial.print(highCount);
    Serial.println(" | RUNNING -> BLOCKED (500 ms)");

    vTaskDelay(pdMS_TO_TICKS(500));
  }
}

// Medium Priority Task (Priority 2, 1000 ms)
void MediumTask(void *parameter)
{
  vTaskDelay(pdMS_TO_TICKS(100));

  while (1)
  {
    mediumCount++;

    Serial.print("MEDIUM | Priority: ");
    Serial.print(uxTaskPriorityGet(NULL));
    Serial.print(" | Count: ");
    Serial.print(mediumCount);
    Serial.println(" | RUNNING -> BLOCKED (1000 ms)");

    vTaskDelay(pdMS_TO_TICKS(1000));
  }
}

// Low Priority Task (Priority 1, 1500 ms)
void LowTask(void *parameter)
{
  vTaskDelay(pdMS_TO_TICKS(100));

  while (1)
  {
    lowCount++;

    Serial.print("LOW    | Priority: ");
    Serial.print(uxTaskPriorityGet(NULL));
    Serial.print(" | Count: ");
    Serial.print(lowCount);
    Serial.println(" | RUNNING -> BLOCKED (1500 ms)");

    vTaskDelay(pdMS_TO_TICKS(1500));
  }
}

void setup()
{
  Serial.begin(115200);
  delay(1000);

  Serial.println("========================================");
  Serial.println(" FreeRTOS TASK PRIORITY EXPERIMENT");
  Serial.println("========================================");

  // HIGH = Priority 3
  BaseType_t r1 = xTaskCreatePinnedToCore(
    HighTask,
    "HighTask",
    2048,
    NULL,
    3,
    &HighTaskHandle,
    1
  );

  // MEDIUM = Priority 2
  BaseType_t r2 = xTaskCreatePinnedToCore(
    MediumTask,
    "MediumTask",
    2048,
    NULL,
    2,
    &MediumTaskHandle,
    1
  );

  // LOW = Priority 1
  BaseType_t r3 = xTaskCreatePinnedToCore(
    LowTask,
    "LowTask",
    2048,
    NULL,
    1,
    &LowTaskHandle,
    1
  );

  if (r1 != pdPASS || r2 != pdPASS || r3 != pdPASS)
  {
    Serial.println("Task creation failed!");
    return;
  }

  Serial.println("Three tasks created successfully.");
  Serial.println("Priority: HIGH=3, MEDIUM=2, LOW=1");
  Serial.println("State: RUNNING -> BLOCKED -> READY -> RUNNING");
  Serial.println("========================================");
}

void loop()
{
  // All work is done by the FreeRTOS tasks
  vTaskDelay(pdMS_TO_TICKS(1000));
}
