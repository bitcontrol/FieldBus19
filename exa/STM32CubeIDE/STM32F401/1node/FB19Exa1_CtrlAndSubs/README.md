# Example 1: FieldBus19 Controller and Subscriber on one STM32F401 Nucleo Board

**How to get this example running:**
* Prepare an STM32F401 Nucleo Board
  * Make sure, USART1, USART2 and USART6 are connected to the outside world
  * For this purpose, the solder bridge ??? has to be removed TODO
* Attach a Nucleo Expansion Board _NEB_RS232+485_ revision 2.2 or higher to the Nucleo board **(1)**
* Use a loopback cable to connect the 2 RS-485 connectors on the NEB: connect pin 2 with pin 2 and pin 3 with pin 3
* Connect the Nucleo board's USB connector with your computer
* Optional: Connect the serial console I/O on JP3 to an RS-232 to USB converter, then connect the USB side of the converter to your computer
* Open a serial terminal program on your computer, configure these settings: 115200/8/N/1
* Open the connection in the serial terminal program; select the above RS-232 to USB converter as COM port
* Download this directory to a local folder on your computer
* Open STM32CubeIDE version 1.12.0 or higher
* Import this project into STM32CubeIDE by navigating to _Menu > File > Open Projects from File System..._
* Build the project
* Download the project to the Nucleo Board

If you encounter any problems, please contact us at info@bitcontrol.ch.

**Notes:**
* **(1)** If you prefer using your own RS-485 interface board, adjust the configurations in the files FB19CtrlCfg.c and FB19SubsCfg.c according to the design of your board.
