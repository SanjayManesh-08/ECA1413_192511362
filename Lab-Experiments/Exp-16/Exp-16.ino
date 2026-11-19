#include <Arduino.h>

EventGroupHandle_t eventGroup;

#define TIMER1_EVENT  (1 << 0)   // BIT 0
#define TIMER2_EVENT  (1 << 1)   // BIT 1

TimerHandle_t timer1;
TimerHandle_t timer2;

// Timer 1 callback
void Timer1Callback(TimerHandle_t xTimer)
{
  xEventGroupSetBits(eventGroup, TIMER1_EVENT);
}

// Timer 2 callback
void Timer2Callback(TimerHandle_t xTimer)
{
  xEventGroupSetBits(eventGroup, TIMER2_EVENT);
}

// Event monitoring task
void EventTask(void *parameter)
{
  while (1)
  {
    Serial.println();
    Serial.println("Waiting for BIT 0 and BIT 1...");

    // Wait until both events are set
    EventBits_t bits = xEventGroupWaitBits(
      eventGroup,
      TIMER1_EVENT | TIMER2_EVENT,
      pdTRUE,        // Clear bits after receiving
      pdTRUE,        // Wait for BOTH bits
      portMAX_DELAY
    );

    Serial.println("Both Timer Events detected!");

    if (bits & TIMER1_EVENT)
    {
      Serial.println("Timer 1 Event -> BIT 0");
    }

    if (bits & TIMER2_EVENT)
    {
      Serial.println("Timer 2 Event -> BIT 1");
    }

    Serial.println("Synchronization complete.");
    Serial.println("------------------------------");
  }
}

void setup()
{
  Serial.begin(115200);
  delay(1000);

  Serial.println("=================================");
  Serial.println(" RTOS TIMER AND EVENT GROUP TEST");
  Serial.println("=================================");

  // Create Event Group
  eventGroup = xEventGroupCreate();

  // Create Timer 1 - 1000 ms
  timer1 = xTimerCreate(
    "Timer1",
    pdMS_TO_TICKS(1000),
    pdTRUE,
    NULL,
    Timer1Callback
  );

  // Create Timer 2 - 2000 ms
  timer2 = xTimerCreate(
    "Timer2",
    pdMS_TO_TICKS(2000),
    pdTRUE,
    NULL,
    Timer2Callback
  );

  // Create Event monitoring task
  xTaskCreate(
    EventTask,
    "EventTask",
    2048,
    NULL,
    1,
    NULL
  );

  // Start timers
  xTimerStart(timer1, 0);
  xTimerStart(timer2, 0);

  Serial.println("Event Group created.");
  Serial.println("Timer 1 = 1000 ms -> BIT 0");
  Serial.println("Timer 2 = 2000 ms -> BIT 1");
  Serial.println("Both events required for synchronization.");
  Serial.println("=================================");
}

void loop()
{
  delay(1000);
}
