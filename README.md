# Passcode Protected Smart Safe
This smart safe is designed to keep valuable belongings protected using embedded systems technology and a few hardware components. It uses the MSP430G2553 micro-controller to control the passcode input, passcode changing capability, and locking mechanism. The safe is equipped with a solenoid, which only unlocks when the correct passcode is sent. The safe also features an alarm that sounds when an incorrect passcode is sent, as well as easy re-locking with a button. UART communication from a laptop or PC device is used to send the passcode inputs to the micro-controller, which processes them and decides what to do.

## Inputs
The smart safe features two inputs: the UART passcode input, and the button locking input. The UART passcode is sent using a software called RealTerm using hexadecimal byte format. Up to nine bytes can be sent at a time, but the last five are only used when a user wants to change their passcode. The second input is the button locking. The micro-controller comes with a programmable button, which was set to lock the solenoid when pressed. This makes it easy to lock the smart safe after use.

## Outputs
There are two outputs for this device: the solenoid lock and the piezoelectric buzzer. The solenoid is connected to a pin on the micro-controller, and coded to lock and unlock as needed. When the button is pressed, the solenoid locks, or stays locked if it was already locked. When the wrong passcode is sent, the solenoid locks or remains locked. When the correct passcode is sent, the solenoid unlocks. Additionally, an alarm was implemented to signal when a wrong passcode is sent. A high frequency tone is sent out to alert in the case of break in attemps. 

## Algorithms
The micro-controller is programmed with algoritms to control the button, solenoid, and buzzer. The button is programmed easily by including it in the interrupt service routine and routing it to the solenoid's pin. The solenoid is programmed using if statements and comparisons of arrays. The input is given an array of size nine, and the passcode is given an array of size four. The contents of these arrays are compared for every case, including correct passcode when the user does not elect to change it, and the correct passcode when the user elects to change it. For each case, the solenoid is either told to lock or unlock. The buzzer is sent to a different pin on the micro-controller. It is also included in the if statements which compare the contents of each array, and is only set to turn on if the incorrect passcode is sent.

## Setup
In order to set up this system, the hardware components need to be properly connected and the UART protocol needs to be followed.
### UART
The UART communcation uses the software RealTerm to send the bytes. The display tab, port tab, and send tab need to be adjusted before sending anything to the system. In the display tab, make sure the Hex[space] format is seleted as well as Half-Duplex. Push the "Change" button to save the settings. In the port tab, select the proper COM port for UART, change the baudrate to 9600, and once again push the "change" button. Finally, the bytes can be sent to the micro-controller when it is plugged in to a computer. The format for the passcode input is shown in the table below.

| Byte Number | Contents | Example |
| ----------- | --------- | ------- |
| Bytes 0-4 | Passcode input | 0x01 0x02 0x03 0x04 |
| Byte 5 | Would you like to change the passcode? | 0xFF = yes, any other byte = no |
| Bytes 6-9 | New passcode | 0x05 0x06 0x07 0x08 |

### Hardware
The system is set up by plugging the micro-controller into the USB port of a computer, and connecting the hardware components to their proper pins. The solenoid has two wires, one connected to ground, and one to pin P2.5. A low side switch is needed to regulate the voltage. The piezo buzzer has one pin going to ground, and the other connected to pin P2.3.
