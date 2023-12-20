# Example 1: FieldBus19 Controller and Subscriber on one STM32F401 Nucleo Board

**How to get this example running:**
* Prepare an STM32F401 Nucleo Board
  * Make sure, USART1, USART2 and USART6 are connected to the outside world
  * Observe the required solder bridge settings for USART2 on the Nucleo board: SB13: Off, SB14: Off, SB61: Off, SB62: On, SB63: On
* Attach a Nucleo Expansion Board _NEB_RS232+485_ revision 2.2 or higher to the Nucleo board **(1)**
* Configure the jumpers on the NEB as follows:
  * JP4, JP5: Connect pins 1 & 2<br>
    This selects RS-485 for UART2
  * JP6, JP7: Connect pins 1 & 2<br>
    This selects RS-485 for UART3
  * JP8, JP9: Connect pins 2 & 3<br>
    This connects UART3 to J2
* Use a loopback cable to connect the 2 RS-485 connectors J1 and J2 on the NEB: connect pin 2 with pin 2 and pin 3 with pin 3
* Connect the Nucleo board's USB connector with your computer
* Connect the serial console I/O on JP3 to an RS-232 to USB converter, then connect the USB side of the converter to your computer **(2)**
* Open a serial terminal program on your computer, configure these settings: 115200/8/N/1 **(2)**
* Open the connection in the serial terminal program; select the above RS-232 to USB converter as COM port **(2)**
* Download this directory to a local folder on your computer
* Open STM32CubeIDE version 1.12.0 or higher
* Import this project into STM32CubeIDE by navigating to _Menu > File > Open Projects from File System..._
* Build the project
* Download the project to the Nucleo Board

**Functionality:**
* If you push the push button PB1 on this node's NEB, its LED1 will light up
* Note: In order to reduce console output, just 2 messages per second are sent to the FB19 Subscriber

**Notes:**
* **(1)** If you prefer using your own RS-485 interface board, adjust the configurations in the files FB19CtrlCfg.c and FB19SubsCfg.c according to the design of your board.
* **(2)** These steps are optional; the serial console displays some information when messages are sent over the bus

If you encounter any problems, please contact us at info@bitcontrol.ch.
