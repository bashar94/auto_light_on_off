# Simple Light Automation Project for Room
Automatically Light on and off based on PIR sensors and LDR sensors. Used Solid State Relay to control 220v light

## Components
  
- MSP430F5529
- PIR sensors
- LDR sensor
- Relay
- Resistors (10k-ohm & 330 ohm)
- Push button
- LED light
- Jumper wires
- Breadboard

## Description

Used 1 MSP430F5529 for controling the whole system. There are 2 PIR sensors, one is inside of the room and another is outside of the the
room and both of them are faced towards the door. So that when someone is entering into the room the outside PIR sensor will send signal 
first and then the inside PIR sensor. Opposite will happend when someone is getting out of the room. There are 2 LDR sensors. LDR sensor
which is inside the room, senses the room brightness and sends it to the microcontroller. If the brightness of the room is not enough 
microcontroller turns the relay cum the ligh on. Outside LDR is used to calibrate the system and to sense if it is dark outside (night).
So light will turn on for this stituation as well. Push button is used to turn the automation system on and off. There are 2 LEDs for 
notifying which PIR sensors are sending signals. Also when the device starts, the 2 LEDs turn on for 5 seconds that means LDR calibration
is happening.
