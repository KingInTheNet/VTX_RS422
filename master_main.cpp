#include <iostream>
#include <windows.h>
#include <chrono>
#include <mutex>
#include <thread>

using namespace std;
#define TYPE_A 2
#define TYPE_B 1
static bool processing = true;
static bool transfer = 0;
static bool b_type = 0;
unsigned char msg[6] = { 0xAD,0xFF,0xFF,0xFF,0xFF,0xAC};
unsigned char off_msg[6] = { 0xA1,0xFF,0xFF,0xFF,0xFF,0xCC};
unsigned char readBuffer[1];
unsigned char writeBuffer[1];
char dataBuffer[1];
char big_data[24];
// Serial port configuration
const char* portName = "COM7";  // Replace with your serial port (e.g., COM3, COM4, etc.)
int baudRate = CBR_38400;         // Baud rate, change if needed
int count = 0;
int cycle = 0;
int n = 0;
int nByte = 0;
bool running = true;
DWORD bytesWritten;
DWORD bytesRead;
bool next_step = true;
std::mutex serialMutex;
HANDLE hSerial;

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

void writeCommand(){
    while (running){
        // std::lock_guard<std::mutex> lock(serialMutex);
        // auto beg = std::chrono::high_resolution_clock::now();
        // while (processing){
            
            // cout << "Data sent: " << writeBuffer[0] << endl;
            // if(b_type && transfer){
            //     while (nByte <4){
            //         DWORD bytesRead;
            //         // n = readFromSerialPort(fd,dataBuffer, sizeof(dataBuffer));
            //         if (!ReadFile(hSerial, dataBuffer, sizeof(dataBuffer), &bytesRead, NULL)) {
            //             std::cerr << "Error reading from serial port" << std::endl;
            //             CloseHandle(hSerial);
            //             return 1;
            //         }
            //         // else {
            //         //     cout << "Read byte from serial port: "
            //         //         << std::hex <<(int) dataBuffer[0] << "len :" << (int)n << endl;
            //         // }
            //         // big_data+=(nByte*6);
            //         memcpy(&big_data[nByte*6],&dataBuffer,sizeof(dataBuffer));
            //         nByte++;
                
            //     }
            //     transfer = 0;
            //     nByte = 0;
            //     cout << "Read big data from serial port: "
            //                 << std::string(big_data, sizeof(big_data)) << endl;
            //     count++;
                
            // }else {
                
            //     // n = readFromSerialPort(fd,readBuffer, sizeof(readBuffer));
            //     if (!ReadFile(hSerial, readBuffer, sizeof(readBuffer), &bytesRead, NULL)) {
            //         std::cerr << "Error reading from serial port" << std::endl;
            //         CloseHandle(hSerial);
            //         return 1;
            //     }
            //     if(bytesRead > 0){
            //         // if (readBuffer[0] == 0xAC){
            //         //     count = 0;
            //         // }
            //         // else {
            //         cout << "Read from serial port: 0x"
            //             << std::hex <<(int) readBuffer[0] <<endl;//std::string(readBuffer, n) << "len :" << n << endl;
            //         count++;
            //         cout<< "Data in buffer" << endl;
            //         // }
            //         if(readBuffer[0]==writeBuffer[0]){
            //             cout<< "Bytes returned" << endl;
            //             next_step = true;
            //         }
                    
            //     }
                

            // }
            
            // cout << "count :" << count << endl;
            
            // count++;
            // count++;
            // count++;
            // count++;
        // }
        if ((count == 5) && b_type && !(transfer)) {
            transfer = 1;
            cout << "Communication end" << endl;
        }
        // else if(count == 5){
        //     cout << "Communication end" << endl;
        // }
        else if(count == 0) {
            cout << "current write cycle: "
                << std::dec << (int)cycle << endl;
            
            b_type = (cmd_keys[msg[0]] == TYPE_B);
        }
        // msg[0] = 0xDE;
        if(next_step) {
            writeBuffer[0] = msg[count];
            if (!WriteFile(hSerial, writeBuffer, sizeof(writeBuffer), &bytesWritten, NULL)
                < 0) {
                std::cerr << "Error writing to serial port" << std::endl;
            }
            next_step =false;
            cout << "Written data" << std::hex<<(int) writeBuffer[0] << endl;
        }
        
        // auto end = std::chrono::high_resolution_clock::now();
        // auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - beg);
        // int res = duration.count();
        // // Displaying the elapsed time
        // cout << "Time taken by this cycle: "
        //     << res << " milliseconds" << endl;
        
    }
    cout << "Exiting write thread" << endl;
    running = false;
    return;
}

void readResponse(){
    while (running){
        // std::lock_guard<std::mutex> lock(serialMutex);
        // auto beg = std::chrono::high_resolution_clock::now();
        // while (processing){
            // if ((count == 5) && b_type) {
            //     transfer = 1;
            //     cout << "Communication end" << endl;
            // }
            // // else if(count == 5){
            // //     cout << "Communication end" << endl;
            // // }
            // // else if(count == 0) {
            // //     cout << "Communication start" << endl;
            // // }
            // // msg[0] = 0xDE;
            // if(next_step) {
            //     writeBuffer[0] = msg[count];
            //     if (!WriteFile(hSerial, writeBuffer, sizeof(writeBuffer), &bytesWritten, NULL)
            //         < 0) {
            //         std::cerr << "Error writing to serial port" << std::endl;
            //     }
            //     next_step =false;
            //     cout << "Written data" << std::hex<<(int) writeBuffer[0] << endl;
            // }
            
            // cout << "Data sent: " << writeBuffer[0] << endl;
            
            
            // cout << "count :" << count << endl;
            
            // count++;
            // count++;
            // count++;
            // count++;
        // }
        if(b_type && transfer){
            while (nByte < 24){
                // DWORD bytesRead;
                // n = readFromSerialPort(fd,dataBuffer, sizeof(dataBuffer));
                if (!ReadFile(hSerial, dataBuffer, sizeof(dataBuffer), &bytesRead, NULL)) {
                    std::cerr << "Error reading from serial port" << std::endl;
                    CloseHandle(hSerial);
                    // return 1;
                }
                // else {
                //     cout << "Read byte from serial port: "
                //         << std::hex <<(int) dataBuffer[0] << "len :" << (int)n << endl;
                // }
                // big_data+=(nByte*6);
                if(bytesRead >0){
                    // memcpy(&big_data[nByte],&dataBuffer,sizeof(dataBuffer));
                    // cout << "Read from serial port: 0x"
                    //     << std::hex <<(int) readBuffer[0] <<endl;//std::string(readBuffer, n) << "len :" << n << endl;
                    count++;
                    big_data[nByte] = dataBuffer[0];
                    nByte++;
                }
                
            
            }
            // if (!ReadFile(hSerial, big_data, sizeof(big_data), &bytesRead, NULL)) {
            //     std::cerr << "Error reading from serial port" << std::endl;
            //     CloseHandle(hSerial);
            //     // return 1;
            // }
            // if(bytesRead >0){
            //     // memcpy(&big_data[nByte*6],&dataBuffer,sizeof(dataBuffer));
            //     // nByte++;

            // }
            count = 0;
            transfer = 0;
            nByte = 0;
            b_type = 0;
            cout << "Read big data from serial port: "
                        << std::string(big_data, sizeof(big_data)) << endl;
            cycle++;
            next_step = true;
            cout << "current read cycle: "
            << (int)cycle << endl;
            if ((int)cycle == 16){
                running = false;
                // processing = false;
                break;
            }
            
            
        }else {
            
            // n = readFromSerialPort(fd,readBuffer, sizeof(readBuffer));
            if (!ReadFile(hSerial, readBuffer, sizeof(readBuffer), &bytesRead, NULL)) {
                std::cerr << "Error reading from serial port" << std::endl;
                CloseHandle(hSerial);
                // return 1;
            }
            if(bytesRead > 0){
                // if (readBuffer[0] == 0xAC){
                //     count = 0;
                // }
                // else {
                cout << "Read from serial port: 0x"
                    << std::hex <<(int) readBuffer[0] <<endl;//std::string(readBuffer, n) << "len :" << n << endl;
                count++;
                // cout<< "Data in buffer" << endl;
                // }
                if(readBuffer[0]==writeBuffer[0]){
                    
                    next_step = true;
                    if(readBuffer[0]==0xAC){
                        cout<< "End of cycle" << endl;
                        count = 0;
                        cycle++;
                        cout << "current read cycle: "
                        << (int)cycle << endl;
                        if ((int)cycle == 16){
                            running = false;
                            // processing = false;
                            break;
                        }
                    }
                }
                
            }
            

        }
        // cycle++;
        // count = 0;
        // count = 0;
        // cycle++;
        // auto end = std::chrono::high_resolution_clock::now();
        // auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - beg);
        // int res = duration.count();
        // // Displaying the elapsed time
        // cout << "Time taken by this cycle: "
        //     << res << " milliseconds" << endl;
        
    }
    
    cout << "Exiting read thread" << endl;
    return;
}

int main() {
    
    
    init_map();
    // Open the serial port
    hSerial = CreateFile(
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
    // for (int i=0; i<255;i++){
        
    //     if (cmd_keys[i] == TYPE_A || cmd_keys[i] == TYPE_B){
    //         cout << "Command keys: " << (unsigned char )cmd_keys[i] << endl;
    //         cout << "Is command" << endl;
    //     }
    // }
    auto beg_program = std::chrono::high_resolution_clock::now();
    std::thread readThread(readResponse);
    std::thread writeThread(writeCommand);
    
    writeThread.join();
    readThread.join();
    
    
    
    while (count < 6){
        if ((count == 5) && b_type) {
            transfer = 1;
            cout << "Communication end" << endl;
        }
        // else if(count == 5){
        //     cout << "Communication end" << endl;
        // }
        // else if(count == 0) {
        //     cout << "Communication start" << endl;
        // }
        // msg[0] = 0xDE;
        writeBuffer[0] = off_msg[count];
        DWORD bytesWritten;
        if (!WriteFile(hSerial, writeBuffer, sizeof(writeBuffer), &bytesWritten, NULL)
            < 0) {
            std::cerr << "Error writing to serial port" << std::endl;
            
        }
        // cout << "Data sent: " << writeBuffer[0] << endl;
        if(b_type && transfer){
            while (nByte <4){
                DWORD bytesRead;
                // n = readFromSerialPort(fd,dataBuffer, sizeof(dataBuffer));
                if (!ReadFile(hSerial, dataBuffer, sizeof(dataBuffer), &bytesRead, NULL)) {
                    std::cerr << "Error reading from serial port" << std::endl;
                    CloseHandle(hSerial);
                    return 1;
                }
                // else {
                //     cout << "Read byte from serial port: "
                //         << std::hex <<(int) dataBuffer[0] << "len :" << (int)n << endl;
                // }
                // big_data+=(nByte*6);
                memcpy(&big_data[nByte],&dataBuffer,sizeof(dataBuffer));
                nByte++;
            
            }
            transfer = 0;
            nByte = 0;
            cout << "Read big data from serial port: "
                        << std::string(big_data, sizeof(big_data)) << endl;
            
        }else {
            DWORD bytesRead;
            // n = readFromSerialPort(fd,readBuffer, sizeof(readBuffer));
            if (!ReadFile(hSerial, readBuffer, sizeof(readBuffer), &bytesRead, NULL)) {
                std::cerr << "Error reading from serial port" << std::endl;
                CloseHandle(hSerial);
                return 1;
            }
            if (readBuffer[0] == 0xAC){
                
                count = 0;
            }
            else {
                cout << "Read from serial port: 0x"
                    << std::hex <<(int) readBuffer[0] <<endl;//std::string(readBuffer, n) << "len :" << n << endl;

            }

        }
        
        // cout << "count :" << count << endl;
        count++;
        // count++;
        // count++;
        // count++;
        // count++;
    }
    // while 
    
    
    auto end_program = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end_program - beg_program);
    int res_program = duration.count();
    // Displaying the elapsed time
    cout << "Time taken by program: "
         << std::dec << res_program << " milliseconds" << endl;

    

    // Close the serial port
    CloseHandle(hSerial);
    return 0;
}
