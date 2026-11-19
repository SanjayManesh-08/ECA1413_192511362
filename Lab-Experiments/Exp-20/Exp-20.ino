#include <Arduino.h>

#define BLOCK_SIZE 32
#define NUM_BLOCKS 4

uint8_t pool[NUM_BLOCKS][BLOCK_SIZE];
bool used[NUM_BLOCKS];

int allocateBlock()
{
  for (int i = 0; i < NUM_BLOCKS; i++)
  {
    if (!used[i])
    {
      used[i] = true;
      return i;
    }
  }
  return -1;
}

void releaseBlock(int block)
{
  if (block >= 0 && block < NUM_BLOCKS)
    used[block] = false;
}

void MemoryTask(void *parameter)
{
  // Heap Memory Test
  Serial.println("\n--- HEAP MEMORY ---");

  size_t before = ESP.getFreeHeap();

  int *data = (int *)pvPortMalloc(10 * sizeof(int));

  if (data != NULL)
  {
    size_t after = ESP.getFreeHeap();

    Serial.print("Heap before : ");
    Serial.println(before);

    Serial.print("Heap after  : ");
    Serial.println(after);

    Serial.print("Heap reduction : ");
    Serial.println(before - after);

    vPortFree(data);

    Serial.print("Heap after release : ");
    Serial.println(ESP.getFreeHeap());
  }

  // Stack High-Water Mark
  Serial.println("\n--- STACK HIGH-WATER MARK ---");

  Serial.print("Minimum remaining stack : ");
  Serial.print(uxTaskGetStackHighWaterMark(NULL));
  Serial.println(" words");

  // Memory Pool Test
  Serial.println("\n--- MEMORY POOL ---");

  int blocks[NUM_BLOCKS];

  Serial.print("Pool size : ");
  Serial.print(NUM_BLOCKS * BLOCK_SIZE);
  Serial.println(" bytes");

  for (int i = 0; i < NUM_BLOCKS; i++)
  {
    used[i] = false;
    blocks[i] = allocateBlock();

    Serial.print("Block ");
    Serial.print(i + 1);
    Serial.println(" allocated");
  }

  // Pool full condition
  if (allocateBlock() == -1)
    Serial.println("Block 5 : FAILED - Pool FULL");

  // Release all blocks
  for (int i = 0; i < NUM_BLOCKS; i++)
    releaseBlock(blocks[i]);

  Serial.println("All blocks released");

  // Reallocation test
  int test = allocateBlock();

  if (test >= 0)
  {
    Serial.println("Block successfully reallocated");
    releaseBlock(test);
  }

  Serial.println("\n--- MEMORY TEST COMPLETE ---");

  vTaskDelete(NULL);
}

void setup()
{
  Serial.begin(115200);
  delay(1000);

  Serial.println("================================");
  Serial.println(" RTOS MEMORY MANAGEMENT TEST");
  Serial.println("================================");

  xTaskCreate(
    MemoryTask,
    "MemoryTask",
    4096,
    NULL,
    1,
    NULL
  );
}

void loop()
{
  delay(1000);
}
