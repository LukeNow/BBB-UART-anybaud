#include<iostream>
#include<unistd.h>
#include<fcntl.h>
#include<sys/ioctl.h>
#include<asm/termbits.h>
#include<string.h>

#include "bbb_uart.h"

using namespace std;

UART::UART(int num, int baud, UART_TYPE type, bool twoStopBitsBool) {
    uartNum = num;    
    baudRate = baud;
    uartType = type;
    twoStopBits = twoStopBitsBool;
    
    initFlag = 0; //init is 0 meaning not yet initialized, shouldnt be able to write/read
    uartID = -1;
    uartType = TX; //setting defaults
}

/*
 * Function to initialize and error check UART setup
 * BUG: Maybe Open, set options, close, then reopen to operate normally
 */
int UART::init() {
    
    string name = "/dev/ttyO";
    string namepath = name + to_string(uartNum);
    
    /* Open devide in appropriate mode */
    if (uartType == TX) {
        uartID = open(namepath.c_str(), O_WRONLY | O_NOCTTY | O_NDELAY);
    }
    else if (uartType == RX) {
        uartID = open(namepath.c_str(), O_RDONLY | O_NOCTTY | O_NDELAY);
    }
    else {
        uartID = open(namepath.c_str(), O_RDWR | O_NOCTTY | O_NDELAY); 
    }
    /* Checking if opened correctly */
    if (uartID < 0) {
        cerr << "INIT: UART" << uartNum << " could not be opened" << endl;
        return -1;
    }
    
    /* Struc to be populated with uart data */
    struct termios2 uartTerm;
    
    /* getting info on uart */
    if (ioctl(uartID, TCGETS2, &uartTerm) < 0) {
        cerr << "INIT: UART" << uartNum << " Termios2 getting failure" << endl;
        return -1;
    }
    
    uartTerm.c_cflag &= ~CBAUD; //Ignoring baudrate
    uartTerm.c_cflag |= BOTHER; //termios2, other baud rate
    uartTerm.c_cflag |= CLOCAL; //Ignore control lines

    if (uartType == TX) {
        uartTerm.c_ospeed = baudRate; //Setting output rate
    }
    else if (uartType == RX) {
        uartTerm.c_cflag |= CREAD; //Reading
        uartTerm.c_ispeed = baudRate; //setting input rate
    }
    else {
        uartTerm.c_cflag |= CREAD; //Reading and writing
        uartTerm.c_ospeed = baudRate; 
        uartTerm.c_ispeed = baudRate; //Setting output and input speeds
    }
    if (twoStopBits == true) {
        uartTerm.c_cflag |= CSTOPB; //two stop bits set
    }
    
    /* Writing termios options to uart */
    if (ioctl(uartID, TCSETS2, &uartTerm) < 0) {
        cerr << "INIT: UART" << uartNum << " Termios2 setting failure" << endl;
        return -1;
    }
    
    initFlag = 1; //init complete, recoginizing

    return 0;
}

int UART::uart_write(void* data, size_t len) {
    if (initFlag != 1) {
        cerr << "uart_write: Uart" << uartNum << " has not been initiated." << endl;
        return -1;
    }

    if (write(uartID, data, len) < 0) {
        return -1;
    }

    return 0;
}

int UART::uart_read(void* buffer, size_t len) {
    int count;
    
    if (initFlag != 1) {
        cerr << "uart_read: Uart" << uartNum << " has not been initiated." << endl;
        return -1;
    }

    if ((count = read(uartID, buffer, len) < 0)) {
        cerr << "UART" << uartNum << " Couldnt read data" << endl;
        return -1;
    }
    
    return count;
}

int UART::dmx_write(void* data, size_t len) {
    
    if (initFlag != 1) {
        cerr << "uart_write: Uart" << uartNum << " has not been initiated." << endl;
        return -1;
    }
    
    if( ioctl(uartID, TIOCSBRK) < 0) {
        cerr << "uart_dmxWrite: Uart" << uartNum << " MAB failed" << endl;
        return -1;
    }
    
    usleep(100);

    if( ioctl(uartID, TIOCCBRK) < 0) {
        cerr << "uart_dmxWrite: Uart" << uartNum << " MAB failed" << endl;
        return -1;
    }
    
    usleep(15);

    if (write(uartID, data, len) < 0) {
        return -1;
    }

    return 0;
}
int UART::status() {

    return 0;
}

int UART::getID() {
    return uartID;
}

UART::~UART() {     
    if (uartID != -1) {     
        close(uartID);
    }
}

