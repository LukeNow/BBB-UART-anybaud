#ifndef UART_BBB
#define UART_BBB

enum UART_TYPE {TX = 0, RX = 1, BOTH = 2};

class UART {
    private:
        int uartNum;
        int baudRate;
        int uartID;
        int initFlag;
        bool twoStopBits;
        UART_TYPE uartType;
        

    public:
        UART(int uartNum, int baudRate, UART_TYPE uartType, bool twoStopBits);
        
        int init();

        int uart_write(void* data, size_t len);
        
        int uart_read(void* buffer, size_t len);
        
        int dmx_write(void* data, size_t len);

        int status();
        
        int getID();
        
        ~UART();
};

#endif
