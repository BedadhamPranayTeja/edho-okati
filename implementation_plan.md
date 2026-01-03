# ESP8266 WiFi Robot Implementation Plan

## Goal Description
Create a WiFi-controlled 2WD robot using ESP8266 NodeMCU and L298N motor driver in Access Point (AP) mode. The robot will host a web server serving a control interface to move Forward, Backward, Left, and Right.

## User Review Required
> [!IMPORTANT]
> **Pinout Confirmation**:
> The proposed pinout for NodeMCU (ESP8266) to L298N is:
> - **Left Motor**: Connect L298N IN1 to `D1 (GPIO 5)`, IN2 to `D2 (GPIO 4)`
> - **Right Motor**: Connect L298N IN3 to `D5 (GPIO 14)`, IN4 to `D6 (GPIO 12)`
> - **Enable Pins (ENA/ENB) for Speed Control**:
>     - **Left Motor Speed (ENA)**: Connect to `D7 (GPIO 13)`
>     - **Right Motor Speed (ENB)**: Connect to `D8 (GPIO 15)`
> - **Ground**: Connect ESP8266 GND to L298N GND and Power Bank GND.
> - **Power**: Connect L298N 12V/VCC to Power Bank (+), ESP8266 VIN to Power Bank (+).
> 
> *Note: I will implement the firmware with PWM support. For now, the buttons will drive motors at set speeds, but the system will be ready for joystick integration.*

## Proposed Changes
I will create a new Arduino sketch file `RobotControl.ino`.

### Firmware
#### [NEW] [RobotControl.ino](file:///d:/New%20folder/edho-okati/RobotControl.ino)
- **Setup**:
    - Configures GPIOs as Outputs.
    - sets up WiFi Access Point (SSID: "Robot_Car", Password: "password123").
    - Starts a Web Server on port 80.
- **Web Server**:
    - Serves a simple HTML page with joystick/buttons.
    - Endpoints:
        - `/` : Serve UI
        - `/move?dir=F` : Forward
        - `/move?dir=B` : Backward
        - `/move?dir=L` : Left
        - `/move?dir=R` : Right
        - `/move?dir=S` : Stop
- **Logic**:
    - `moveRobot(direction)` function to set GPIO states.

### Frontend
The HTML/CSS/JS will be embedded in the `.ino` file as a raw string literal to keep it single-file.
- **UI**: 5 Buttons (Forward, Left, Stop, Right, Backward).
- **Style**: Dark mode, large touch-friendly buttons.
- **JS**: Fetch API to signal movement commands asynchronously.

## Verification Plan
### Automated Tests
- None (Hardware dependency).

### Manual Verification
1.  **Compile**: User needs to open `RobotControl.ino` in Arduino IDE.
2.  **Upload**: Flash to ESP8266.
3.  **Circuit**: Wire up the robot according to the pinout.
4.  **Connect**: Connect phone/laptop to WiFi "Robot_Car".
5.  **Control**: Open `http://192.168.4.1` and press buttons.
    - Verify Left Motor moves forward when "Forward" is pressed.
    - Verify Right Motor moves forward when "Forward" is pressed.
    - Verify turns and backward motion.
