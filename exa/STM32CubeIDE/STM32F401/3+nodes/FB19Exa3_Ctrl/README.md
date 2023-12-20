# Example 3: FieldBus19 Controller on a STM32F401 Nucleo Board

**How to get this example running:**
* Prepare an STM32F401 Nucleo Board
  * Make sure USART1 and USART2 are connected to the outside world
  * Observe the required solder bridge settings for USART2 on the Nucleo board: SB13: Off, SB14: Off, SB61: Off, SB62: On, SB63: On
* Attach a Nucleo Expansion Board _NEB_RS232+485_ revision 2.2 or higher to the Nucleo board **(1)**
* Configure the jumpers on the NEB as follows:
  * JP4, JP5: Connect pins 1 & 2<br>
    This selects RS-485 for UART2
  * JP6, JP7: Don't care<br>
    If pins 2 & 3 are connected, UART3 can be used as RS-232 interface on J4 
  * JP8, JP9: Connect pins 1 & 2<br>
    This allows daisy chaining the boards via J1 and J2
* Connect the Nucleo board's USB connector to your computer
* Connect the serial console I/O on JP3 to an RS-232 to USB converter, then connect the USB side of the converter to your computer **(2)**
* Open a serial terminal program on your computer, configure these settings: 115200/8/N/1 **(2)**
* Open the connection in the serial terminal program; select the above RS-232 to USB converter as COM port **(2)**
* Download this directory to a local folder on your computer
* Open STM32CubeIDE version 1.12.0 or higher
* Import this project into STM32CubeIDE by navigating to _Menu > File > Open Projects from File System..._
* Build the project
* Download the project to the Nucleo Board
* Use a cable to connect J1 of this NEB to J2 of the 1st Subscriber node's NEB: connect pin 2 with pin 2, pin 3 with pin 3 and pin 4 with pin 4
* Use a cable to connect J2 of this NEB to J2 of the 2nd Subscriber node's NEB: the wiring is identical to above

**Functionality:**
* If you push the push button PB1 on this node's NEB, LED1 on both Subscriber's NEBs will light up **(3)**
* If you push the push button PB1 on the FB19 Subscriber1's NEB, LED1 on the FB19 Subscriber2's NEB and LED1 on this node's NEB will light up **(3)**
* If you push the push button PB1 on the FB19 Subscriber2's NEB, LED1 on the FB19 Subscriber1's NEB and LED1 on this node's NEB will light up **(3)**
* Note: In order to reduce console output, just 2 messages per second are sent to the FB19 Subscribers

**Notes:**
* **(1)** If you prefer using your own RS-485 interface board, adjust the configurations in the files FB19CtrlCfg.c according to the design of your board.
* **(2)** These steps are optional; the serial console displays some information when messages are sent over the bus
* **(3)** Assuming the FB19Exa3_Subs1 and FB19Exa3_Subs2 examples are running on the connected Nucleo STM32F401 boards

If you encounter any problems, please contact us at info@bitcontrol.ch.
