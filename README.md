# FieldBus19

**This is a no-cost field bus software library with a simple C API for STM32 microcontrollers.**

## Features
The library supports **2 to 5 bus participants** (nodes) per bus segment (1). The required microcontroller **resources are just 3 GPIO pins, 1 UART and 1 Timer** per node.

**No programming related to interrupt processing, response timeouts, packeting, checksum protection, or similar** is required when using this library. Communication results are indicated by return values and error codes.

Just configure what SoC resources it shall use, plug-in the interrupt service routines, start the library and periodically call a handler that does the work in the background. Then **focus on your application development.**

Layer 1 is **an RS-485 bus**, but **if only two boards** are to be connected, **RS-232 or simple 3.3V level UART connections** will also work.

For each **supported microcontroller family**, a **reference implementation and application examples are published** in this repo.

**The major differences to other field buses like CAN, Modbus or ProfiBus are:**
* 2 to 5 bus participants per bus segments (1)
* Configurable bit rate from 2kbit/s to 250kbit/s (2)
* Physical bus length is only limited by RS-485 specifications
* Variable message/frame size for high throughput
* Operating system independent implementation, works with any OS/RTK or on bare metal
* Supported OSI layers go from layer 1 (physical) up to layer 7 (application)
* The library guarantees fully deterministic behavior
* No proprietary hardware required (silicon, connectors, cables)

**Notes:**
* (1): The technical limit is 31 nodes per segment. In the no-cost version this number has been limited to 5 nodes.
* (2): The technical limit is around 1Mbit/s on a STM32F401 controller, depending on the system clock frequency, interrupt activities and priorities on the controller. In the no-cost version this number has been limited to 250kbit/s.

## Hardware
This library doesn't require specific hardware to run on. Although currently only the STM32F401 microcontrollers are supported, the port to different STM controllers is simple. They will be made available as soon as users request them.

Outside the microcontroller, RS-485 bus transceivers are required if the bus system shall utilize an RS-485 bus. However, for initial tests it can also be run on a peer-to-peer RS-232 'network'. If unprotected 3.3V signals suffice, no external hardware is required for running this bus system.

There is a Nucleo Expansion Board available from [https://bitcontrol.ch](Bitcontrol GmbH, Switzerland). It provides a dual-channel RS-485 bus transceiver chip, and the connectors necessary to operate the FieldBus19 bus system.<br>
In addition, a dual-channel RS-232 transceiver chip is incorporated on the board. This allows for the setup of a peer-to-peer network.<br>
Write an email to info@bitcontrol.ch if you are interested in a purchase.

**However, this board isn't bound to the FieldBus19.** It can be used wherever a STM Nucleo board is to be connected to an RS-232 or RS485 network. It thereby provides the necessary line drivers and receivers, and it **can be configured via jumpers in various ways**.<br>
**Its schematics are open source.** See the [doc/hw](doc/hw/) folder for details.

**Furthermore, starter kits are available.**<br>
The smallest contains one Nucleo-F401RE board and one Nucleo Expansion Board for the FieldBus19 bus system, plus two loopback cables.<br>
The next larger one contains 2 Nucleo-F401RE boards, 2 Nucleo Expansion Boards and several cables for the FieldBus19 bus system.<br>
The largest kit contains 3 boards of each type plus more cables for building a 'real' network.

## Future Versions
This is an imcomplete, non-binding list of features, that future versions may contain:
* Fully automatic bus address assignment to participants at bus initialization (no DIL switches or hard-coded addresses necessary anymore)
* Lock mechanism that allows to exclude new, unexpected bus participants
* Hardware fault detection that reports faults to the application and disables the RS-485 drivers permanently if desired

## For Business Users
For companies who are potentially interested in using this bus system in their products, feel free to use the no-cost version in your products whereever and as often as you like. There will be no claim for money, neither today nor in the future.

**The following services are available on request:**
* Implement additional features
* Ports to STM32 or other microcontroller products not included in this repo
* Remove the restrictions that apply to the no-cost version
* Safety certification up to SIL2
* Test applications for specific business domain hardware

Please contact info@bitcontrol.ch if you wish to discuss possibilities and costs. Thank you.

## Further Reading
* For more information about this bus system see the folders and files in the [doc/](doc/) folder.
