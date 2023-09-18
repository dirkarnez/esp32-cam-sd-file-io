#include <SD.h>
#include <SPI.h>

File file;  // Shared file pointer
SemaphoreHandle_t fileMutex;  // Mutex for synchronizing file access

TaskHandle_t task1;
TaskHandle_t task2;

// Task 1 function
void task1Function(void* parameter) {
  while (true) {
    // Acquire the mutex
    if (xSemaphoreTake(fileMutex, portMAX_DELAY) == pdTRUE) {
      // Perform file I/O operations
      file.println("Task 1 - Hello, world!");
      file.flush();

      // Release the mutex
      xSemaphoreGive(fileMutex);
    }
    delay(1000);
  }
}

// Task 2 function
void task2Function(void* parameter) {
  while (true) {
    // Acquire the mutex
    if (xSemaphoreTake(fileMutex, portMAX_DELAY) == pdTRUE) {
      // Perform file I/O operations
      file.println("Task 2 - Hello, world!");
      file.flush();

      // Release the mutex
      xSemaphoreGive(fileMutex);
    }
    delay(1000);
  }
}

void setup() {
  Serial.begin(115200);

  // Initialize SD card
  if (!SD.begin(SS)) {
    Serial.println("Failed to initialize SD card");
    return;
  }

  // Create the mutex
  fileMutex = xSemaphoreCreateMutex();
  if (fileMutex == NULL) {
    Serial.println("Failed to create mutex");
    return;
  }

  // Open the file
  file = SD.open("/data.txt", FILE_WRITE);
  if (!file) {
    Serial.println("Failed to open file");
  }

  // Create and start Task 1
  xTaskCreatePinnedToCore(task1Function, "Task1", 10000, NULL, 1, &task1, 0);

  // Create and start Task 2
  xTaskCreatePinnedToCore(task2Function, "Task2", 10000, NULL, 1, &task2, 1);
}

void loop() {
  // Your main code here (if any)
}
