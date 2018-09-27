# BBB-UART-AnyBaud
Programming interface written in C++ that allows the use of UARTs on the BeagleBone Black(BBB) at any Baud rate.
This interface aims to fix the lack of frameworks that have wrappers for reading and writing to UARTS of the BBB, as well as the lack of options to set UARTs to any Baud rate. 

This interface allows for the configuring of UARTs with a few options including 
- Two stop bits
- Reading and writing, or both
- Setting of any Baud Rate that your system can support. 
- Writing of continoues zeroes (ignoring stop bits)
- Writing of continoues ones (ignoring stop bits)

If there are any questions or concerns feel free to contact me at lnowakow@ucsd.edu. 
