# FieldBus19

A new field bus for micro-controller boards without WiFi and LAN interfaces.

OSI layer 1 (physical) is a RS-485 bus, SoC silicon resources are limited to UARTs, GPIOs and timers.

The supported layers go up to OSI layer 7 (application), where bus participants can be managed.

The major differences to other field bus specifications are:
* The node address assignment is fully automatic (no DIL switches necessary)
* The bit rate is programmable, and the packet/frame size is variable
* Assuming sufficient hardware resources, the bus can be operated with redundant cabling, connectors and nodes
* Deterministic behavior can be easily achieved; this makes it well suited for safety critical applications
* It's partly open source, royalty free and operating system independent

A reference implementation on ST's NUCLEO-F401RE boards is under work.

For more information on the features of this bus system see the white paper in the docu folder.

Have fun!