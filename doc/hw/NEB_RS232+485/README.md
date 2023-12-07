# Nucleo Expansion Board (NEB) for Serial Communication

This is an **add-on board to ST Microelectronics STM32 Nucleo-64 development boards**. It connects to their ST morpho connector and **provides RS-232 and RS-485 connectivity**.

It's not only **suitable for FieldBus19 bus systems**, but **can be added to any Nucleo-64 board that requires protection by serial communication drivers**.

## Feature Summary
**The Nucleo Expansion Board type A offers these features:**
* A dual-channel RS-232 transceiver MAX3222
* A dual-channel RS-485 transceiver SN75ALS1178N
* 4 male 6-pin Molex MicroFit 3.0 connectors, one for each serial channel
* All necessary passive components to operate them
* A 3-pin header connected to USART1 (Rx, Tx, Gnd), 3.3V/unprotected, for the connection to a serial console
* Several jumpers for various configuration options
* A user push button and a user LED for prototyping and tests

**The Nucleo Expansion Board type B differs in this feature from type A:**
* It carries 4 screw terminals with 6 contacts each, instead of the Molex MicroFit 3.0 connectors
