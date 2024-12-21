# Dual Microcontroller-Based Door Locker Security System Using Password Authentication

## Overview
This project implements a door locking system that utilizes two microcontrollers (HMI_ECU and Control_ECU) to ensure secure access through password authentication. The system interacts with a user interface for input and feedback, stores data in an external EEPROM, and integrates a PIR sensor to detect motion.

## Features
- **Password Protection**: Users can set and verify a password stored in external EEPROM.
- **LCD and Keypad Interface**: Allows easy interaction for entering and managing passwords.
- **UART Communication**: HMI_ECU sends and receives data to and from Control_ECU via UART.
- **EEPROM Storage**: Passwords and system data are stored securely in an external EEPROM.
- **Motorized Door Control**: The door is unlocked/locked using a motor driven by an H-bridge.
- **Buzzer Alert**: The buzzer is activated for failed password attempts and system alerts.
- **PIR Motion Sensor**: Detects motion to trigger door operations.
- **Password Change Option**: Users can change the password after verification.
- **Security Lock**: System locks for one minute if the password is entered incorrectly three times consecutively.

## Hardware Components
1. **Microcontrollers**:
   - **HMI_ECU**: Manages user interface and input.
   - **Control_ECU**: Handles door locking mechanisms and security functions.
2. **Keypad**: For password input.
3. **LCD Display (16x2)**: Displays prompts, messages, and status updates.
4. **External EEPROM**: Stores authentication passwords.
5. **PIR Sensor**: Detects motion near the door.
6. **Motor Driver (H-Bridge)**: Controls the motor for locking and unlocking the door.
7. **Buzzer**: Alerts for incorrect entries.

## System Overview
The system consists of two microcontrollers, each with distinct roles:
- **HMI_ECU**:
  - Receives input from the keypad.
  - Displays status messages and prompts on the LCD.
  - Sends password input to the Control_ECU via UART.
- **Control_ECU**:
  - Validates the password received from the HMI_ECU.
  - Controls the motor to lock/unlock the door using the H-bridge driver.
  - Activates the buzzer for security alerts.
  - Communicates the system status back to the HMI_ECU.

## Functional Workflow
1. **System Initialization**:
   - Both microcontrollers initialize their respective components (LCD, keypad, UART, EEPROM, motor driver, PIR sensor, buzzer).
2. **Password Entry**:
   - User enters the password via the keypad.
   - HMI_ECU sends the entered password to Control_ECU for validation.
3. **Password Verification**:
   - Control_ECU compares the received password with the one stored in EEPROM.
   - If valid, the door is unlocked.
   - If invalid, the buzzer alerts and counts the failed attempt.
   - After three consecutive incorrect attempts, the system locks for one minute.
4. **Motion Detection**:
   - PIR sensor detects motion and prompts the system to prepare for door operation.
5. **Password Change**:
   - User selects the password change option after successful authentication.
   - New password is entered and stored in EEPROM.

## Security Features
- **Encrypted Communication**: Secure UART communication between HMI_ECU and Control_ECU.
- **Failed Attempt Lockout**: Prevents brute force attacks by locking the system after three failed attempts.
- **External Storage**: EEPROM ensures secure password storage.


