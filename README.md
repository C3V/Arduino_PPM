# Arduino_PPM


Instruction set (instructions start with "@" and end with "#"):

-@pin:00#   (00 pin will be used in the current application)

-@sen:00#  (00 pin is a sensor pin)

-@tmp:00#  (00 pin is a temperature sensor pin)

-@lum:01# (00 pin is a luminosity sensor pin)

-@led:05# (05 pin is a LED pin)

-@att:05# (05 pin is an actuator pin)

-@a#  (Ack signal from Port)

-@pinreq# ("tell me the pin you are using")

-@stp:00#  ("stop using 00 pin")

-@sda:05:1# (turn on the digital actuator on pin 05)

-@saa:08:090# (turn on the analog actuator (servo) and rotate in of a 90 degrees angle)

-@png:02# (ping sensor on 02 pin)

-@dis:040# (if an object is 40 cm near the ping, the sensor will notify the server)

-@rgb:11:255# (turn on the rgb led on pin 11 (red or blue or green) at max power (255))


