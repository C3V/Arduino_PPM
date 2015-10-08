# Arduino_PPM

An arduino board running arduino.ino code can dialogue with a server to obtain measures using (at the moment):

-temperature sensor

-luminosity sensor

and operate in an environment (following indications from the server) using:

-LED actuator

-Servomotor actuator

Components can work all together with no conflicts. Board can handle only one sensor per type, from all the sensor types specificated. Sensors, at the moment, are supposed to be only analog (in order not to use too much of the limited arduino memory with an expensive array implementation for digital measures), but it will be optimized in the final steps of the project in order to handle the PING sensor, which is digital.

Instruction set (instructions start with "@" and end with "#"):




	-@pin:00#   (00 pin will be used in the current application)

	-@sen:00#  (00 pin is a sensor pin)

-@tmp:00#  (00 pin is a temperature sensor pin)

-@lum:01# (00 pin is a luminosity sensor pin)

-@led:05# (05 pin is a LED pin)

	-@att:05# (05 pin is an actuator pin)

	-@ack#  (Ack signal from Port)

	-@prq# ("tell me the pin you are using")

	-@stp:00#  ("stop using 00 pin")

	-@sda:[num_pin]:[value]# set digital actuator; num_pin is the position of the actuator, value canbe 0 or 1

	-@saa:[num_pin]:[value]# set analog actuator; num_pin is the position of the actuator, value is an integer between 000 and 999;

-@m09:60#  ("rotate the servo on 09 pin of a 60 degrees angle")

-


