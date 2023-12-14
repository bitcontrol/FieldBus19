# STM32F401

This folder contains example projects that work with the ST Microelectronics IDE version 1.12.0 and above and have been tested on a STM32F401RE Nucleo Board.

Follow these steps in order to run a specific example application:
* Connect a STM32F401RE Nucleo Board to an USB connector of your computer
* Open the STM32CubeIDE application
* Import the project to the STM32CubeIDE application
* Build the project
* Download the application to the Nucleo board
* It starts automatically after a successful download
* Press the black reset button on the Nucleo board if you yet like to restart the application
* Repeat the steps above if more than one Nucleo board is involved

**Subdirectories:**
* **1node/**<br>
This folder contains one subfolder. It contains a project/application, that sends itself FB19 messages forth an back by the means of a loopback cable.
* **2nodes/**<br>
This folder contains two subfolders, each containing a project/application. One folder contains a FB19 Bus Controller project, the other a FB19 Bus Subscriber project.
* **3+nodes/**<br>
This folder contains three subfolders, each containing a project/application. One folder contains a FB19 Bus Controller project, the two other folders contain a FB19 Bus Subscriber project each. See their containing folders in what they do and differ.
* More examples on request
