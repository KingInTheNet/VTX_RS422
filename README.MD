# Setup
1. Install minGW with https://www.msys2.org/ on windows 
2. Edit the source codes variable portName to match the port name of your device
4. compile command : 
```g++ -o <executable name> master_main.cpp
   g++ -o <executable name> slave_main.cpp
```
5. Compile program with different portName run in separate windows in case of slave to simulate different slaves