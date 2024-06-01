# Update 1-6-2024
I am currently working on a new solution using a custom PCB and different hardware. I might even integrate it into my custom RTI solution, but for now I'm leaving this repository as is.


# Volvo-PHM

This project is designed to work on an ESP32 connected to the official Volvo Phone Module (2005-2009).

!!! My code is currently not ready to be used in any vehicle. A full rework is in progress. !!!



First of all, a **HUGE** thank you to Phuz from [Swedespeed](https://www.swedespeed.com) for helping me find the correct Canbus codes to send text to the display on the DIM.

In my testing I have found that the DIM has its own set of extra characters (besides the standard ascii characters from 0x20 to 0x7D). I have recreated
the custom chars on [this website](https://maxpromer.github.io/LCD-Character-Creator/) and put most of them in [this pdf](https://github.com/larslego/Volvo-PHM/blob/1a4ef09edf304cfe772891277dc4180d778dd996/VolvoDIM%20HEX%20Codes.pdf) for everyone to use as a reference.

## Todo
- Indicate car battery level on iPhone.
- Ability to use this device as a bluetooth keyboard to any device.
- Integrate with the DIM on the S60 (2005-2009) to show the current song title, caller name, etc.
- Integrate with the radio (HU-650).

This software is tested on an iPhone XR in combination with a Volvo S60 2.5T from 2009.
It should work for iPhone 5S and up.
It should also work for all Volvo S60 from 2005 to 2009.
