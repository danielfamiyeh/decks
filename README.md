# Miniature DJ Decks?
I'm not entirely sure what this will be but I have and ESP32, STM32 Nucleo and another STM F440RE or something, and some other components.
The idea is that I'm gonna build a miniature set of DJ decks, constrained to the few components I have now.

So ESP32 will drive UI, IO inputs, Bluetooth and I'll write some DSP firmware for the STM32 to send to an op-amp circuit and then stero speakers.
It will get files from SD card, or USB (but right now SD card) and I should be able to select through folders and songs, via the simple 16x2 LCD display.
Load it into either channel and connect audio interfaces via phono, bluetooth, or a mix of the two. 
And then yeah presumably if I want some filters, and since its not analogue input some FFT somewhere or something to be able to cut frequencies depending on the mix level mode.

There's no OS with this (FreeRTOS but in the common sense of an OS, this is not an 'OS').
No raspberry PI or anything, just like a raw embedded systems project, we'll see how it goes.