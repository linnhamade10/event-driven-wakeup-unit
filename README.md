# Event-Driven Wake-Up Unit

Low-power wake-up unit for water-level monitoring using an ESP32-S3 and ultrasonic sensing, developed in collaboration with Waltero and Kristianstad University.

## Overview

This project originated from a Systems Engineering course and was later experimentally evaluated during a bachelor's thesis. The system periodically wakes from deep sleep, measures the water level using an ultrasonic sensor, and triggers an existing camera device developed by Waltero when predefined threshold conditions are detected.

## Systems Engineering Project

* Defined system requirements and overall architecture.
* Investigated alternative sensing approaches, including radar and ultrasonic sensing.
* Selected ultrasonic sensing based on complexity, cost, and suitability for the application.
* Designed and implemented a low-power event-driven wake-up unit using an ESP32-S3 and an AJ-SR04M ultrasonic sensor.
* Integrated the wake-up unit with an existing camera device provided by Waltero.
* Developed the prototype in collaboration with Waltero.

## Bachelor Thesis Evaluation

* Experimentally evaluated the wake-up unit under controlled laboratory conditions.
* Characterized power consumption during deep sleep, sensing, and processing states.
* Investigated wake-up timing and threshold-based event detection.
* Assessed measurement stability under still water, disturbed water, nearby reflective surfaces, and partial obstructions.
* Analyzed detection reliability, responsiveness, and overall system performance.
* Examined the suitability of ultrasonic sensing and periodic wake-up operation for low-power water-level monitoring applications.

## Technologies

* C
* ESP-IDF
* ESP32-S3
* AJ-SR04M Ultrasonic Sensor
* Embedded Systems
* Event-Driven Systems
* Low-Power Design

## Tools

* Git
* GitHub

## Collaboration

Developed in collaboration with Waltero and Kristianstad University.
