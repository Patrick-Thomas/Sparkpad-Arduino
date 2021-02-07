### About

The Sparkpad is a reconfigurable control surface, primarily aimed at Streamers. The V1 Sparkpad is designed to communicate with streaming software - such as OBS - via HID commands sent over USB. However, due to it's modular hardware design and open source Arduino firmware, there is scope to do much more.

![](/images/sparkpad.jpg)

We are hoping to foster a development community, and we will continue to develop improvements for the Sparkpad as and when we can (currently the team only consists of two members)!

### Getting started

To start developing custom firmware for your V1 Sparkpad (or to update to newer firmware), you will first need to install the [Arduino IDE](https://www.arduino.cc/en/software){:target="_blank"}, and install the Sparkpad library using the Arduino Library Manager.

Your Sparkpad will be visible to the Arduino IDE as a serial device on a particular port, and the port name will depend on which OS you are using (COM for Windows, dev/tty for Mac and Linux). The board type is Arduino Leonardo. 

![](/images/port.png)

![](/images/board.png)

To upload the latest firmware version to your Sparkpad, simply choose Sparkpad->HID->Pro-Micro from the examples menu and hit upload. If you are looking to customise the firmware, then keep reading!

### Components

#### .ino file
This file - or 'sketch' in Arduino parlance - contains the core code which runs on the Sparkpad. You can break down this file into 3 parts. At the top are **definitions** and **includes**: definitions specify global parameters (such as mappings for the Sparkpad's 12 keys), and includes pull in resources from external files (such as the code for the OLED). Below these is the **setup** function which runs once when the Sparkpad boots up and initialises all the components. Finally the **loop** function is the code which runs continuously on the Sparkpad, and this is where all the good stuff happens! The .ino file also contains a few utility functions, which are just there for convenience and to avoid duplicate code.

If you are just looking to adjust key mappings or other global parameters on your Sparkpad, then you only have to make changes to the necessary definitions in the .ino file.

#### knob.h, leds.h and oled.h files

These files contain code specific to a particular Sparkpad component. Contained within these files are references to external libraries which are also part of the Arduino ecosystem. If you are looking to add bespoke functionality to your Sparkpad, then you will probably have to tweak these files.

### Contributing

We are excited to see what people can make their Sparkpad do! If you want to share your new functionality with the community then please fork this repository and submit a pull request. 