# Example 3: FieldBus19 Subscriber1 on a STM32F401 Nucleo Board

**How to get this example running:**
* Prepare an STM32F401 Nucleo Board
  * Make sure USART1 and USART6 are connected to the outside world
  * For this purpose, the solder bridge ??? has to be removed TODO
* Attach a Nucleo Expansion Board _NEB_RS232+485_ revision 2.2 or higher to the Nucleo board **(1)**
* Configure the jumpers on the NEB as follows:
  * JP4, JP5: Don't care<br>
    If pins 2 & 3 are connected, UART2 can be used as RS-232 interface on J3
  * JP6, JP7: Connect pins 1 & 2<br>
    This selects RS-485 for UART3
  * JP8, JP9: Connect pins 1 & 2<br>
    This allows daisy chaining the boards via J1 and J2
* Connect the Nucleo board's USB connector with your computer
* Connect the serial console I/O on JP3 to an RS-232 to USB converter, then connect the USB side of the converter to your computer **(2)**
* Open a serial terminal program on your computer, configure these settings: 115200/8/N/1 **(2)**
* Open the connection in the serial terminal program; select the above RS-232 to USB converter as COM port **(2)**
* Download this directory to a local folder on your computer
* Open STM32CubeIDE version 1.12.0 or higher
* Import this project into STM32CubeIDE by navigating to _Menu > File > Open Projects from File System..._
* Build the project
* Download the project to the Nucleo Board
* Use a cable to connect J1 or J2 of this NEB to J1 or J2 of one of the other node's NEB: connect pin 2 with pin 2, pin 3 with pin 3 and pin 6 with pin 6

**Functionality:**
* If you push the push button PB1 on this node's NEB, LED1 on the other FB19 Subscriber's NEB will light up **(3)**
* If you push the push button PB1 on the other FB19 Subscriber's NEB, LED1 on this node's NEB will light up **(3)**
* In either case, LED1 on the FB19 Controller's NEB will light up **(3)**

**Notes:**
* **(1)** If you prefer using your own RS-485 interface board, adjust the configurations in the files FB19CtrlCfg.c according to the design of your board.
* **(2)** These steps are optional; the serial console displays some information when messages are sent over the bus
* **(3)** Assuming the FB19Exa3_Ctrl and FB19Exa3_Subs2 examples are running on the connected Nucleo STM32F401 boards

If you encounter any problems, please contact us at info@bitcontrol.ch.
