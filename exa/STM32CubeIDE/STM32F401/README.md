# STM32F401

This folder contains example projects that work with the ST Microelectronics IDE version 1.12.0 and above.

Follow these steps in order to run a specific example application:
* Connect an STM32F401RE Nucleo Board with an USB connector of your computer
* Open the STM32CubeIDE application
* Import the project to the STM32CubeIDE application
* Build the project
* Download the application to the Nucleo board
* It starts automatically after a successful download
* Press the black reset button on the Nucleo board if you like; this restarts the application
* Repeat above steps if more than one Nucleo board is involved


**Subdirectories:**
* 1node/            This folder contains one subfolder. It contains a project/application, that sends itself FB19 messages forth an back by the means of a loopback cable.
* 2nodes/           This folder contains two subfolders, each containing a project/application. One folder contains a FB19 Bus Controller project, the other a FB19 Bus Subscriber project.
* 3+nodes/          This folder contains three subfolders, each containing a project/application. One folder contains a FB19 Bus Controller project, the two other folders contain a FB19 Bus Subscriber project each. See their containing folders in what they do and differ.
* FB19LibFT/        This folder contains a functional test project/application.
* More examples on request
