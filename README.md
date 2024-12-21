Dual Microcontroller-Based Door Locker Security System Using Password Authentication
Overview
This project implements a door locking system that utilizes two microcontrollers (HMI_ECU and Control_ECU) to ensure secure access through password authentication. The system interacts with a user interface for input and feedback, storing data in an external EEPROM, and integrates a PIR sensor to detect motion.

Features
Password Protection: Users can set and verify a password stored in external EEPROM.
LCD and Keypad Interface: Allows easy interaction for entering and managing passwords.
UART Communication: HMI_ECU sends and receives data to and from Control_ECU via UART.
EEPROM Storage: Passwords and system data are stored securely in an external EEPROM.
Motorized Door Control: The door is unlocked/locked using a motor driven by an Hbridge.
Buzzer Alert: The buzzer is activated for failed password attempts and system alerts.
PIR Motion Sensor: Detects motion to trigger door operations.
Password Change Option: Users can change the password after verification.
Security Lock: System locks for one minute if the password is entered incorrectly three times consecutively.
Hardware Components
Microcontrollers:
HMI_ECU: Manages user interface and input.
Control_ECU: Handles door locking mechanisms and security functions.
Keypad: For password input.
LCD Display (16x2): Displays prompts, messages, and status updates.
External EEPROM: Stores the authentication passwords.
PIR Sensor: Detects motion near the door.
Motor Driver (H-Bridge): Controls the motor for locking and unlocking the door.
Buzzer: Alerts for incorrect entries.
System Overview
The system consists of two microcontrollers: one handles user input and display, while the other manages the door locking mechanism. The HMI_ECU receives input from the keypad and displays status messages on the LCD. The Control_ECU communicates with the HMI_ECU via UART to authenticate the password and control the door lock motor based on the input.
