# lc3vm

This is a C implementation of a virtual machine capable of running [LC-3](https://en.wikipedia.org/wiki/Little_Computer_3) assembly programs. This project was made for educational purposes, though it should be capable of running anything, including an operating system.

The project was based on [this](https://www.jmeiners.com/lc3-vm/) guide by Justin Meiners and Ryan Pendleton

## Usage
To build this software in any Unix-like operating system, clone this repository:

    git clone https://github.com/cbidinot/lc3vm
   Then build the project using Make:
   

    make
   To run any program, execute the binary including the location of the program's file:
   

    ./lc3vm path/to/program

## Example Programs
Two example programs are included in the repository from the guide on which this virtual machine was based. The first one is an implementation of 2048, and the second is an implementation of Rogue. 
