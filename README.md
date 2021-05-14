# About

The Sparkpad is a reconfigurable control surface, primarily aimed at Streamers. The V1 Sparkpad is designed to communicate with streaming software - such as OBS - via HID commands sent over USB. However, due to it's modular hardware design and open source Arduino firmware, there is scope to do much more.

We are hoping to foster a development community, and we will continue to develop improvements for the Sparkpad as and when we can.

![](/images/sparkpad.jpg)

# Getting started

Once Sparkpad is plugged into your computer, it will be recognised automatically as a HID device, so there is no need to install additional drivers. The Sparkpad's 12 keys are mapped to F13 - F24 (the bottom left key is mapped to shift) by default. In Windows 10, these function keys do not serve any function within the OS, so you are free to use them as hotkeys within your chosen software.

To adjust the lighting of your Sparkpad, you can use the OLED and 3 menu buttons located in the bottom left. The middle menu button is the "select" button, and the left and right buttons let you cycle through the available options. The OLED menu is hidden by default to prevent "burn in"; to show it simply press the "select" button. The menu consists of two levels, and these are described below.

## Main menu

**Colour** : The global lighting colour. Unfortunately due to memory limitations the menu displays numbers instead of actual colours. Listed below are the colour assignments:

* 0 : Off
* 1 : Red
* 2 : Blue
* 3 : Green
* 4 : Pink
* 5 : Yellow
* 6 : Cyan
* 7 : White

**Brightness** : The global lighting brightness, with higher numbers being brighter.

**Key** : Use this option to tailor lighting for individual keys. For example, to adjust lighting for the top right key, choose "2" for this option and proceed to the "Key settings" menu below. Here is how the numbers and keys are related: 

![](/images/keypad.png)

**Key settings** : Selecting this option takes you to the second menu level, which lists lighting options for the selected key.

## Key settings

**Lighting mode** : The key lighting mode. In "global" mode, the key colour will be the same as the global colour selected in the main menu. In "unique" mode, you can set a unique colour for this key. The assignments are as follows:

* 0 : Global
* 1 : Unique

**Colour** : The key lighting colour. If the lighting mode is "unique", this option selects the colour for this key. The colour assignments are the same as previously described.

**Switch mode** : The key switch mode. In "static" mode, pressing the key will not effect it's lighting. In "toggle" mode, pressing the key will turn it's lighting off, and pressing it again will switch it on. In "delay" mode, once the key is pressed the lighting will stay lit for a predetermined time set by the delay option. The assignments are as follows:

* 0 : Static
* 1 : Toggle
* 2 : Delay

**Delay** : The key delay setting. This option is only relevant if this key is in "delay" mode. You can choose any delay between 1 and 255 seconds.

## Cleaning your Sparkpad

To clean the surfaces of your Sparkpad case, you can use soapy water and a non abrasive scrubber such as a sponge or toothbrush. You can also use Isopropyl Alcohol (IPA), and this tends to a better job cleaning the engraved parts. If using soapy water, you should first remove the PCB from the case by undoing the 4 corner screws. With Isopropyl Alcohol you can leave the PCB in the case. You may find that white marks are left behind after cleaning; to remove these, rub the surfaces with clean white paper.

**Do not use Acetone or other strong solvents, as these will dissolve the black paint on the top panel.**

# Firmware

To develop custom firmware for your Sparkpad (or to update to newer firmware), there are 3 options available to you.

## The easy way - Arduino IDE

### Installation

You will first need to download and install the [Arduino IDE](https://www.arduino.cc/en/software), and import the Sparkpad library using the Arduino Library Manager:

![](/images/libman.png)

![](/images/libman2.png)

You may get prompted to install some additional libraries, ensure you do else the code will not work. There is one library you will have to install slightly differently, due to an issue with the Arduino Library Manager. Click on Code -> Download Zip on [this page](https://github.com/Nullkraft/Keypad) to download the library, then  import it into the Arduino IDE using the Add .ZIP Library tool:

![](/images/addzip.png)

### Uploading

When plugged in via USB, your Sparkpad will be visible to the Arduino IDE as a serial device on a particular port, and the port name will depend on which OS you are using (COM for Windows, dev/tty for Mac and Linux). The board type is Arduino Leonardo: 

![](/images/port.png)

![](/images/board.png)

To upload the latest firmware version to your Sparkpad, simply choose Sparkpad  -> Pro-Micro -> default from the examples menu and hit upload. If you are looking to customise the firmware, then keep reading!

### Customisation

The Arduino firmware is split into two components - the .ino file, and .h (or header) files.

#### .ino file

This file - or 'sketch' in Arduino parlance - contains the core code which runs on the Sparkpad. You can break down this file into 3 parts. At the top are **definitions** and **includes**: definitions specify global parameters (such as mappings for the Sparkpad's 12 keys), and includes pull in resources from external files (such as the code for the OLED). Below these is the **setup** function which runs once when the Sparkpad boots up and initialises all the components. Finally the **loop** function is the code which runs continuously on the Sparkpad, and this is where all the good stuff happens! The .ino file also contains a few utility functions, which are just there for convenience and to avoid duplicate code.

If you are just looking to adjust key mappings or other global parameters on your Sparkpad, then you only have to make changes to the necessary definitions in the .ino file.

#### knob.h, leds.h and oled.h files

These files contain code specific to a particular Sparkpad component. These files have their own **definitions** and **includes**, and all the functions which define their behaviour. If you are looking to add bespoke functionality to your Sparkpad, then you will probably have to tweak one or more of these files.

## The lightweight way - avrdude

If you don't want to look at the code and just want to upload firmware, you can use [this tool] (https://github.com/vanbwodonk/leonardoUploader). Once installed, you can download the .hex file you want from the "releases" section of this repo, and upload it straight to your Sparkpad using the command line.

## The pro way - platformio

If you intend to make serious modifications to your Sparkpad firmware, then we recommend downloading VScode and installing the platformio plugin. This repo contains a platformio.ini file which allows you to open it inside VScode as a platformio project. Coding inside platformio lets you to take advantage of tools such as code completion and syntax checking, and gives you easy access to the .h files which are more hidden inside the Arduino IDE.

The only snag with using platformio is it does not recognise .ino files as source files, so you will have to change the .ino suffix to .cpp on the files you are working on. If you intend to push changes to the main Sparkpad branch, please remember to change these back to .ino before you push to maintain backwards compatibility with the Arduino IDE.

To further ensure backwards compatibility, when writing new and experimental code please create a new example within the examples folder, and add the example as an environment inside the platformio.ini file. **Don't add your code to one of the default examples**. If you feel the code you are writing deserves to be part of the default firmware, then please raise an issue in Github first.

# Contributing

We are excited to see what people can make their Sparkpad do! If you want to share your new functionality with the community then please fork this repository and submit a pull request. 

# Support

If you would like to join The Sparkpad Discord Community where we also have a wider community dedicated to streamers please feel free to [join](https://discord.gg/uvYdVn9TBU) or for more information please visit the [Sparkpad Website](https://sparkpad.co.uk)
