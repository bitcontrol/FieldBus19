# Example 2: FieldBus19 Subscriber on a STM32F401 Nucleo Board

**How to get this example running:**
* Prepare an STM32F401 Nucleo Board
  * Make sure USART1 and USART6 are connected to the outside world
  * For this purpose, the solder bridge ??? has to be removed TODO
* Attach a Nucleo Expansion Board _NEB_RS232+485_ revision 2.2 or higher to the Nucleo board **(1)**
* Connect the Nucleo board's USB connector with your computer
* Connect the serial console I/O on JP3 to an RS-232 to USB converter, then connect the USB side of the converter to your computer **(2)**
* Open a serial terminal program on your computer, configure these settings: 115200/8/N/1 **(2)**
* Open the connection in the serial terminal program; select the above RS-232 to USB converter as COM port **(2)**
* Download this directory to a local folder on your computer
* Open STM32CubeIDE version 1.12.0 or higher
* Import this project into STM32CubeIDE by navigating to _Menu > File > Open Projects from File System..._
* Build the project
* Download the project to the Nucleo Board
* Use a cable to connect J2 of this NEB to J1 of the FB19 Controller's NEB: connect pin 2 with pin 2, pin 3 with pin 3 and pin 6 with pin 6
* If you push the push button PB1 on the FB19 Controller's NEB, LED1 on this NEB will light up **(3)**; note that the push button is intentionally just polled twice per second

If you encounter any problems, please contact us at info@bitcontrol.ch.

**Notes:**
* **(1)** If you prefer using your own RS-485 interface board, adjust the configurations in the files FB19CtrlCfg.c according to the design of your board.
* **(2)** These steps are optional; the serial console displays some information when messages are sent over the bus
* **(3)** Assuming the FB19Exa2_Ctrl example is running on the connected Nucleo STM32F401 board
