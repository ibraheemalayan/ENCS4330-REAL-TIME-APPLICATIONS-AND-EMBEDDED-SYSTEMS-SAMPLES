# ENCS4330 - Real time applications & embedded systems samples

This repository contains code samples and resources for the [ENCS4330 - Real time applications & embedded systems](https://www.birzeit.edu/en/content/encs4330-real-time-applications-and-embedded-systems) course at [Birzeit university (BZU)](https://www.birzeit.edu/en)

### Course Description
Variety of issues regarding the real-time application of embedded microprocessor systems, problems of real-time computer applications in process control or similar areas, digital processing, the physics of sensors and transducers, signal representation, and system design and software development, applications on automotive control, biomedical instrumentation, communication systems, speech processing, data compression and audio processing.

## My Projects
* [OpenGL Race Simulation](https://github.com/ibraheemalayan/first_project_the_big_race)
    > A multi-processing race simulation, written in C & OpenGL, using signals & named pipes.
* [Factory OpenGL Simulation](https://github.com/ibraheemalayan/factory_opengl_simulation)
    > A multi-threaded multi-processing application that simulates the behavior of a chocolate-manufacturing factory., written in C & OpenGL, using Linux IPC techniques for synchronization.
* [OIM OpenGL Simulation](https://github.com/ibraheemalayan/OIM_opengl_simulation)
    > ⚠️ THIS PROJECT HAS NEVER BEEN FINISHED ⚠️
* [PIC16F877A LCD Assembly](https://github.com/ibraheemalayan/PIC16F877A_LCD_Assembly)
    > control a 16x2 LCD using PIC16F877A assembly and a push button with timer interrupts to change the displayed text.


### Course Instuctor
[Dr. Hanna Bullata](mailto:hbullata@birzeit.edu)


## C Code Samples

* [Basic Code](./1_basic): fork, exec, args
* [Signals](./2_signal): signal, alarm, pause
* [Pipes](./3_pipe): pipes, named pipes, fifo
* [Queue](./4_queue): message queues, client-server example
* [OpenGL](./5_opengl): various OpenGL examples
* [Shared Memory](./6_shared_memory): shared memory, semaphores
* [Sockets](./7_socket): sockets, servers, clients
* [Threads](./8_threads): threads, pthread library

## [PIC16F877A Assembly Examples](./9_pic_assembly)

* [ADC](./9_pic_assembly/ADC): Analog to digital convertison to read a temperature sensor and trigger an LED
* [Calculator](./9_pic_assembly/CALC): A calculator that can add, subtract, multiply, and divide two numbers. output to a 16x2 LCD.
* [Keypad](./9_pic_assembly/KEYPAD): Reads a 4x4 keypad and displays the key pressed on a 7-segment display.
* [ISR Keypad](./9_pic_assembly/KEYPAD_ISR): modified version of the keypad example that uses interrupts to read the keypad.
* [LED Timer](./9_pic_assembly/LED1H): A timer that displays its output to a set of LEDs
* [Timer ISR](./9_pic_assembly/TIMER): Flips an LED on and off every 2.5 seconds using a TIMER0 interrupt service routine.
* [Analogue Input](./9_pic_assembly/VINTEST): Analogue input using an external reference voltage of 2.56V



## Documentations & Notes

* [MPLAB X Installtion & Assembler Note](./9_pic_assembly/README.md#L5)
* [Debugging Multi-Process](./docs/debugging_multi_process.md)

## PIC Resources
* [Microchip PIC16F877A Datasheet](https://ww1.microchip.com/downloads/en/devicedoc/39582b.pdf)
* [PIC Assembly Language Cheatsheet](http://www.niplesoft.com/blog/wp-content/uploads/2016/02/PIC-ASM-Cheatsheet.pdf)
* [PIC Timers 0/1/2 Cheatsheet](https://exploreembedded.com/wiki/PIC16f877a_Timer)
* [16x2 LCD Usage](https://circuitdigest.com/article/16x2-lcd-display-module-pinout-datasheet)

### Repository Author
Website: [Ibraheem Alyan](https://www.ibraheemalayan.dev/)    
Email: contact@ibraheemalayan.dev
