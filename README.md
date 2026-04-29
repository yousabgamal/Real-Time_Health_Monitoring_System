# VitaLink – Smart Sports Health Monitoring Belt

## Overview
**VitaLink** is an IoT-based smart sports belt designed to monitor an athlete’s vital body parameters in real time.  
The system continuously collects physiological and motion data using multiple sensors and processes them through the powerful **ESP32-S3** microcontroller.

VitaLink helps improve athletic performance, monitor health conditions, and provide real-time insights during physical activity.

---

## Features
- Real-time health monitoring
- Heart rate and blood oxygen tracking
- Body temperature measurement
- Sweat/effort detection
- Motion and acceleration tracking
- ESP32-S3 based IoT system
- Designed for wearable sports applications

---

## System Description
VitaLink is a wearable smart sports belt equipped with several sensors that measure the athlete’s physiological and motion data in real time.

All sensors are connected to and controlled by the **ESP32-S3**, which reads, processes, and manages the collected data efficiently for monitoring and future IoT integration.

---

## Hardware Components

### MAX30100 – Heart Rate & SpO₂ Sensor
- Measures heart rate (BPM)
- Measures blood oxygen level (SpO₂)
- Provides real-time cardiovascular data

### Sweat Sensor – Effort Detection
- Detects sweat levels
- Indicates physical exertion and fatigue

### MLX90614 – Temperature Sensor
- Infrared non-contact sensor
- Measures body temperature accurately

### MPU6050 – Motion Sensor
- Accelerometer
- Measures acceleration and motion
- Tracks athlete movement and activity

### ESP32-S3 – Main Controller
- Controls all sensors
- Processes real-time data
- Enables IoT system functionality
- Ready for wireless/cloud integration

---

## Setup & Usage
1. Clone the repository:
```bash
git clone https://github.com/yousabgamal/Athletic_Belt.git


