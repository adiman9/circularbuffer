# circularbuffer

Buffers are extremely useful things in all sorts of applications. This repo is a basic implementation of a circular buffer written in C++. 

A circular buffer will overwrite itself in the event of an overflow, making it a very desirable tool in many programs where the exact size of input is known. This reduces the overall security risk to that particular application by removing some potential buffer overflow exploits.

Originally this program was written in a typical C style procedural manner. Later that was refactored into an object oriented implementation to learn more about the nature of C++ and increase my overall understanding of OO programming in general. 

The implementation itself uses operator overloading withing C++. This was done deliberately to give me a real world chance of using operator overloading to more easily achieve the desired result in a given program.
