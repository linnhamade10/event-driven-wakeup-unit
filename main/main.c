#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "driver/gpio.h"
#include "esp_timer.h"
#include "esp_rom_sys.h"
#include "esp_sleep.h"

// GPIO pin assignments
#define TRIG_PIN GPIO_NUM_10      // Ultrasonic sensor trigger pin
#define ECHO_PIN GPIO_NUM_11      // Ultrasonic sensor echo pin
#define LED_PIN  GPIO_NUM_4       // Output used to simulate camera trigger
#define VOLT_REG_PIN GPIO_NUM_5   // Controls power to the voltage regulator

/*
 * Measure the distance using the ultrasonic sensor.
 *
 * Returns:
 *   Distance in centimeters if successful.
 *   -1 if a timeout occurs while waiting for the echo signal.
 */
float measure_distance_cm()
{
    // Ensure trigger pin is low before sending a pulse
    gpio_set_level(TRIG_PIN, 0);
    esp_rom_delay_us(5);

    // Send a 15 µs trigger pulse to start a measurement
    gpio_set_level(TRIG_PIN, 1);
    esp_rom_delay_us(15);
    gpio_set_level(TRIG_PIN, 0);

    // Wait for the echo signal to go high
    int64_t start = esp_timer_get_time();
    while (gpio_get_level(ECHO_PIN) == 0)
    {
        // Abort if no echo is received within 50 ms
        if (esp_timer_get_time() - start > 50000)
            return -1;
    }

    // Record the time when the echo pulse starts
    int64_t echo_start = esp_timer_get_time();

    // Wait until the echo pulse ends
    while (gpio_get_level(ECHO_PIN) == 1)
    {
        // Abort if the pulse duration exceeds 50 ms
        if (esp_timer_get_time() - echo_start > 50000)
            return -1;
    }

    // Calculate pulse duration and convert it to distance
    int64_t duration = esp_timer_get_time() - echo_start;

    // Speed of sound ≈ 0.0343 cm/µs.
    // Divide by two because the sound travels to the target and back.
    return (duration * 0.0343f) / 2.0f;
}

void app_main(void)
{
    // Configure GPIO directions
    gpio_set_direction(TRIG_PIN, GPIO_MODE_OUTPUT);
    gpio_set_direction(ECHO_PIN, GPIO_MODE_INPUT);
    gpio_set_direction(LED_PIN, GPIO_MODE_OUTPUT);
    gpio_set_direction(VOLT_REG_PIN, GPIO_MODE_OUTPUT);

    // Enable the voltage regulator supplying the sensor
    gpio_set_level(VOLT_REG_PIN, 1);

    // Allow the sensor and circuitry to stabilize
    vTaskDelay(pdMS_TO_TICKS(1000));

    printf("Taking one measurement...\n");

    // Perform a distance measurement
    float distance = measure_distance_cm();

    if (distance < 0)
    {
        printf("Timeout\n");
    }
    else
    {
        printf("Distance: %.2f cm\n", distance);
    }

    /*
     * Event detection:
     * If the measured distance is below 50 cm,
     * activate the output to simulate triggering
     * a camera or external subsystem.
     */
    if (distance > 0 && distance < 50)
    {
        printf("Event detected -> activating output\n");

        // Activate output
        gpio_set_level(LED_PIN, 1);

        // Keep output active for 0.5 seconds
        vTaskDelay(pdMS_TO_TICKS(500));

        // Deactivate output
        gpio_set_level(LED_PIN, 0);

        printf("Output deactivated\n");
    }
    else
    {
        printf("No event detected\n");
    }

    // Disable the voltage regulator before entering deep sleep
    gpio_set_level(VOLT_REG_PIN, 0);

    // Configure a timer wake-up after 20 seconds
    printf("Entering deep sleep...\n");
    esp_sleep_enable_timer_wakeup(20 * 1000000ULL);

    // Enter deep sleep to minimize power consumption
    esp_deep_sleep_start();
}
