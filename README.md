# Biometric Fingerprint Security System

This project is a biometric fingerprint security system designed to enhance security and accessibility in sensitive areas, such as libraries, offices, and gyms. By replacing traditional physical keys and magnetic cards - which can be lost, duplicated, or damaged - the system offers a reliable, secure, and convenient fingerprint-based access solution.

## Overview
The fingerprint system authenticates users through a fingerprint sensor, allowing access to authorized individuals only. With a user-friendly interface and efficient control process, this system is suitable for a variety of applications where controlled access is essential.

## Project Components
- **Arduino Uno**: Manages communication between components, processes sensor input, and controls actions.
- **LCD Touch Screen**: Provides a graphical user interface for the system, including password input for administrators and fingerprint scanning.
- **FPM10A Fingerprint Sensor**: Captures and authenticates fingerprints, communicating with the Arduino via UART.
- **Servo Motor**: Operates the door mechanism based on authentication results.

## System Workflow
1. **Main Screen**: Provides two options:
   - **Admin Login**: Password-protected area for managing fingerprints (enroll/delete).
   - **Open Door**: Allows authorized users to unlock the door using their fingerprint.

2. **Admin Functions**:
   - **Enroll New Fingerprint**: Guides administrators through adding a new fingerprint to the system.
   - **Delete Existing Fingerprint**: Allows administrators to delete stored fingerprints.

3. **Fingerprint Authentication**:
   - **Success**: Displays a success message and unlocks the door.
   - **Failure**: Shows an error message with retry options.

## Features
- **Enhanced Security**: Replaces traditional keys and cards with biometric access.
- **User-Friendly Interface**: LCD touch screen interface for easy navigation and management.
- **Access Control**: Only authorized users can open the door.
- **Admin Management**: Secure, password-protected access for adding and deleting fingerprints.

## Applications
This biometric security system can be used in a variety of environments, including:
- Libraries
- Offices
- Gyms
- Other sensitive areas requiring secure and controlled access.

## Technology and Libraries
- **Hardware**: Arduino Uno, LCD Touch Screen, FPM10A Fingerprint Sensor, Servo Motor
- **Software**: Arduino IDE, relevant libraries for fingerprint and screen control

Enjoy enhanced security and convenience with this biometric fingerprint access control system!
