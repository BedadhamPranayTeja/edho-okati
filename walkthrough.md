# ESP8266 Robot Verification Guide

## 1. Upload Firmware
1.  Open `d:\New folder\edho-okati\RobotControl.ino` in Arduino IDE.
2.  Select Board: **NodeMCU 1.0 (ESP-12E Module)**.
3.  Upload the sketch followed by opening the Serial Monitor (115200 baud).

## 2. Wiring Check (Important!)
Ensure your connections match the code:

| Component | Pin  | ESP8266 Pin | Notes |
| :--- | :--- | :--- | :--- |
| **Left Motor** | IN1  | **D1** (GPIO 5) | Direction 1 |
|  | IN2  | **D2** (GPIO 4) | Direction 2 |
|  | ENA  | **D7** (GPIO 13) | **Remove Jumper!** Connect to D7 |
| **Right Motor** | IN3  | **D5** (GPIO 14) | Direction 1 |
|  | IN4  | **D6** (GPIO 12) | Direction 2 |
|  | ENB  | **D8** (GPIO 15) | **Remove Jumper!** Connect to D8 |
| **Power** | VCC  | Power Bank + | 5V-12V Source |
|  | GND  | Power Bank - | **Common Ground** with ESP GND |

## 3. Testing
1.  Power up the robot.
2.  On your phone/laptop, look for WiFi **"Robot_Car"**.
3.  Connect (Password: `password123`).
4.  Open Browser: `http://192.168.4.1`
5.  Test buttons: **Reference Left/Right based on robot facing forward**.
    - If a motor spins backward when it should go forward, swap its INx/INy wires.
