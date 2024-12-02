#include <iostream>
#include <windows.h>
#include <chrono>

using namespace std;
#define TYPE_A 2
#define TYPE_B 1
static bool processing = 0;
static bool transfer = 0;
static bool b_type = 0;
// static bool transfer_phase
static unsigned char cmd_keys[255];
void init_map() {
    cmd_keys[(unsigned char)0xA1] = TYPE_A;
    cmd_keys[(unsigned char)0xA2] = TYPE_A;
    cmd_keys[(unsigned char)0xA3] = TYPE_A;
    cmd_keys[(unsigned char)0xA4] = TYPE_A;
    cmd_keys[(unsigned char)0xA5] = TYPE_A;
    cmd_keys[(unsigned char)0xA6] = TYPE_A;
    cmd_keys[(unsigned char)0xA7] = TYPE_A;
    cmd_keys[(unsigned char)0xA8] = TYPE_A;
    cmd_keys[(unsigned char)0xA9] = TYPE_A;
    cmd_keys[(unsigned char)0xAA] = TYPE_A;
    cmd_keys[(unsigned char)0xAD] = TYPE_B;
    cmd_keys[(unsigned char)0xAE] = TYPE_B;
    cmd_keys[(unsigned char)0xD1] = TYPE_A;
    cmd_keys[(unsigned char)0xD2] = TYPE_A;
    cmd_keys[(unsigned char)0xD3] = TYPE_A;
    cmd_keys[(unsigned char)0xDC] = TYPE_A;
    cmd_keys[(unsigned char)0xDE] = TYPE_A;
    cmd_keys[(unsigned char)0xDD] = TYPE_B;
    cmd_keys[(unsigned char)0xE0] = TYPE_B;
    cmd_keys[(unsigned char)0xED] = TYPE_A;
    cmd_keys[(unsigned char)0xED] = TYPE_B;
}

int main() {
    // Serial port configuration
    const char* portName = "COM3";  // Replace with your serial port (e.g., COM3, COM4, etc.)
    int baudRate = CBR_38400;         // Baud rate, change if needed
    int count = 0;
    int nByte = 0;
    unsigned char readBuffer[1];
    unsigned char big_data[24] = {0};
    init_map();
    // Open the serial port
    HANDLE hSerial = CreateFile(
        portName, 
        GENERIC_READ | GENERIC_WRITE, 
        0, 
        NULL, 
        OPEN_EXISTING, 
        0, 
        NULL
    );

    if (hSerial == INVALID_HANDLE_VALUE) {
        std::cerr << "Error opening serial port" << std::endl;
        return 1;
    }

    // Set the serial port parameters
    DCB dcbSerialParams = {0};
    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);

    if (!GetCommState(hSerial, &dcbSerialParams)) {
        std::cerr << "Error getting serial port state" << std::endl;
        CloseHandle(hSerial);
        return 1;
    }

    dcbSerialParams.BaudRate = baudRate;  // Set the baud rate
    dcbSerialParams.ByteSize = 8;          // 8 data bits
    dcbSerialParams.StopBits = ONESTOPBIT; // 1 stop bit
    dcbSerialParams.Parity   = NOPARITY;   // No parity

    if (!SetCommState(hSerial, &dcbSerialParams)) {
        std::cerr << "Error setting serial port state" << std::endl;
        CloseHandle(hSerial);
        return 1;
    }

    // Set timeouts for reading and writing
    COMMTIMEOUTS timeouts = {0};
    timeouts.ReadIntervalTimeout = 0;  // No wait time between bytes
    timeouts.ReadTotalTimeoutConstant = 1; // Minimal timeout for total read
    timeouts.ReadTotalTimeoutMultiplier = 0; // Zero multiplier (read as fast as possible)
    timeouts.WriteTotalTimeoutConstant = 50;
    timeouts.WriteTotalTimeoutMultiplier = 10;

    if (!SetCommTimeouts(hSerial, &timeouts)) {
        std::cerr << "Error setting timeouts" << std::endl;
        CloseHandle(hSerial);
        return 1;
    }
    for (int i=0; i<255;i++){
        
        if (cmd_keys[i] == TYPE_A || cmd_keys[i] == TYPE_B){
            cout << "Command keys: " << (unsigned char )cmd_keys[i] << endl;
            cout << "Is command" << endl;
        }
    }
    while (true){
        auto beg = std::chrono::high_resolution_clock::now();

        // Read data from the serial port
        
        
        DWORD bytesRead;
        if (!ReadFile(hSerial, readBuffer, sizeof(readBuffer), &bytesRead, NULL)) {
            std::cerr << "Error reading from serial port" << std::endl;
            CloseHandle(hSerial);
            return 1;
        }
        
        if (bytesRead > 0) {
            cout<<"Bytes read "<< endl;
            auto end = std::chrono::high_resolution_clock::now();
            auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - beg);
            int res = duration.count();
            // Displaying the elapsed time
            cout << "Time taken by function: "
                << std::dec << res << " milliseconds" << endl;
            // readBuffer[bytesRead] = '\0';  // Null-terminate the read data
            
            if (!processing){
                if (cmd_keys[readBuffer[0]] != 0){
                        // auto beg = std::chrono::high_resolution_clock::now();
                        cout << "B command " << endl;
                        processing = 1;
                        b_type = (cmd_keys[readBuffer[0]] == TYPE_B);
                }
            }
            if (b_type && (readBuffer[0] == 0xAC)) {
                transfer = 1;
            }
            std::cout << "Received: " << std::hex << (int) readBuffer[0] << std::endl;
            if(processing){
                if (transfer){
                    DWORD bytesWritten;
                    cout << "Big transfer " << endl;
                    while (nByte < 24){
                        readBuffer[0] = big_data[nByte];
                        if (!WriteFile(hSerial, readBuffer, sizeof(readBuffer), &bytesWritten, NULL)) {
                            std::cerr << "Error writing to serial port" << std::endl;
                            // CloseHandle(hSerial);
                            
                            // return 1;
                        }
                        nByte++;
                    }
                    nByte = 0;
                    transfer = 0;
                    processing = 0;
                    b_type = 0;
                    // count ++;
                    // std::cout << "Data sent: " << dataToSend << std::endl;
                }else {
                    DWORD bytesWritten;
                    cout << " Responding" << endl;
                    if (!WriteFile(hSerial, readBuffer, sizeof(readBuffer), &bytesWritten, NULL)) {
                        std::cerr << "Error writing to serial port" << std::endl;
                        // CloseHandle(hSerial);
                        return 1;
                    }
                    if(readBuffer[0] == 0xAC){
                        transfer = 0;
                        processing = 0;
                        b_type = 0;
                        
                    }else if (readBuffer[0] == 0xCC){
                        // Close the serial port
                        CloseHandle(hSerial);
                        return 0;
                    }
                    
                    // count ++;
                    bool isEnd = (readBuffer[0] == 0xAC);
                    std::cout << "is 0xAC: " << isEnd << std::endl;
                }
            }
            
        }
        // } else {
        //     std::cout << "No data received. Count is: " << count << std::endl;
        //     std::cout << "Is processing : " << processing << std::endl;

        // }
        // Send data to the serial port
        // const char* dataToSend = "Hello, Serial Port!\n";
        

        
    }

    

    // Close the serial port
    CloseHandle(hSerial);
    return 0;
}
