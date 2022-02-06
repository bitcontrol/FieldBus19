# FieldBus19

A field bus for resource limited micro-controller boards that have no WiFi or LAN interfaces.

The supported OSI layers go from layer 1 up to layer 7 (application), where bus participants can be managed.

OSI layer 1 (physical) is a RS-485 bus. SoC silicon resources are limited to UARTs, GPIOs and timers.

The major differences to other field busses are:
* The bus address assignment is fully automatic (no DIL switches necessary)
* The bit rate is programmable, and the packet/frame size varies with the size of the data it transports
* Assuming sufficient hardware resources, the bus can be operated with redundant cabling and connectors; this makes it suited for safety critical applications
* Deterministic behavior is easily achieved; simulations on computers are therefore simplified
* It's partly open source, royalty free and operating system independent

A reference implementation on ST's NUCLEO-F401RE boards is under work.

For more information on the features of this bus system see the white paper in the docu folder.

Have fun!
