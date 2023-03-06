# PIC16F877A MPASM Assembly Examples

This directory contains a collection of examples for the PIC16F877A microcontroller. The examples are written in MPASM assembly language and are intended to be used with the MPASM assembler. ( not the new PIC-AS assembler )

### Note on MPLABX
Starting with MPLABX IDE version 5.40, the assembler MPASM toolchain is no longer installed with the IDE. To use MPASM, install a previous MPLABX version that comes with MPASM. The last version that worked with MPASM was MPLABX v5.35.

[Download MPLABX v5.35 from Microchip Download Archive ( Windows / Linux / Mac )](https://www.microchip.com/en-us/tools-resources/archives/mplab-ecosystem)
> unlike newer versions, v5.35 requires x86 architecture. If you are using an ARM based MAC (apple sillicon), you will need to install a virtual machine to run MPLABX v5.35. ( Parallels + Windows 11 ARM works pretty well )

## Code Samples

* [ADC](./ADC): Analog to digital convertison to read a temperature sensor and trigger an LED
* [Calculator](./CALC): A simple calculator that can add, subtract, multiply, and divide two numbers. output is displayed on a 16x2 LCD.
* [Keypad](./KEYPAD): Reads a 4x4 keypad and displays the key pressed on a 7-segment display.
* [ISR Keypad](./KEYPAD_ISR): modified version of the keypad example that uses interrupts to read the keypad.
* [LED Timer](./LED1H): A timer that displays its output to a set of LEDs
* [Timer ISR](./TIMER): Flips an LED on and off every 2.5 seconds using a TIMER0 interrupt service routine.
* [Analogue Input](./VINTEST): Analogue input using an external reference voltage of 2.56V


## Resources
* [Microchip PIC16F877A Datasheet](https://ww1.microchip.com/downloads/en/devicedoc/39582b.pdf)
* [PIC Assembly Language Cheatsheet](http://www.niplesoft.com/blog/wp-content/uploads/2016/02/PIC-ASM-Cheatsheet.pdf)
* [PIC Timers 0/1/2 Cheatsheet](https://exploreembedded.com/wiki/PIC16f877a_Timer)