# CANPiWi

Welcome to CANPiWi project.

The MERG-CANPiWi is an application that designed to run on a raspberry pi attached to a CAN module transceiver.
The app interfaces the MERG-CANBUS protocol and the JMRI Engine Drive application. 
It allows the user to drive model rail locomotives using the Engine Drive without the JMRI.
The app also works as a CAN to CANGrid format via a tcp connection.

## Building the project

Clone the project.
If developing in a PC with copy the file /lib/liblog4cpp.a.pc to /lib/liblog4cpp.a
If developing in a Raspberry pi with copy the file /lib/liblog4cpp.a.pi to /lib/liblog4cpp.a

Type **make all**.

Use the script initial_setup.sh to setup the raspberry pi in AP mode and compile the project.

For more information visit [MERG CANPiWi] (http://www.merg.org.uk/merg_wiki/doku.php?id=cbus:canwi)


