#include "rtos.h"

void BlinkTask_Init(void) {
    Serial.begin(11500);
    pinMode(LED_PIN, OUTPUT);

    xTaskCreatePinnedToCore(
        BlinkTask,
        "BlinkTask",
        1000,
        NULL,
        1,
        &BlinkTaskHandle,
        1
    );
}

void BlinkTask(void* parameter) {
    for(;;) {
        digitalWrite(LED_PIN, HIGH);
        Serial.println("BlinkTask: LED ON");
        vTaskDelay(1000 / portTICK_PERIOD_MS); // 100ms
        digitalWrite(LED_PIN, LOW);
        Serial.println("BlinkTask: LED OFF");
        vTaskDelay(1000 / portTICK_PERIOD_MS);
        Serial.print("BlinkTask running on core");
        Serial.println(xPortGetCoreID());
    }
}
