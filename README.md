# Arduino-Terminal
This is a Software for a server and for a Wio Terminal to become the ram and usage from the server.



## Features from the Server:

- Set the program from where you wanna have the ram or usage 
   - per process id 
   - to start a process
- Set the port
- Set the Adress to Bind
- Works on Windows and Unix based Distributions
- Connects over Wifi to the Wio Terminal

## Features from my Wio Terminal Software:
- Gui
- Set a PIN up to 12 Numbers
- Set the Wifi to connect
    - SSID
    - Password
- Set the Server
    - Port
    - Adress
-  Connects over Wifi to the Wio Terminal

## Server Usage:
#### Commands:
```
        --help / -h                  dsiplays this message
        --port / -p    <Port>        sets the Port
        --exec / -e    <Program>     open this Programm        or
        --pid  / -pi   <Process ID>  Dysplays ths Process
```

#### Format:
```
Java -jar <NAME.jar> -p <Port> (--exec <Program> or -pid <Process ID>)
```
#### Examples:

Windows:
```
Java -jar Arduino_Terminal.X.X.X.Jar -p 8080 --exec Notepad.exe
```
```
Java -jar Arduino_Terminal.X.X.X.Jar -p 18807 --pid 2133
```
Unix / Linux :
```
Sudo Java -jar Arduino_Terminal.X.X.X.Jar -p 46827 --exec gedit
```

```
Sudo Java -jar Arduino_Terminal.X.X.X.Jar -p 35216 --pid 123
```
## Installation
Pleas read all: Wiki

## for Java Developer:

#### Dependencies:

|Java 7 - 9| Java 9 - 16|
|----------|------------|
|[jna-5.8.0](https://github.com/java-native-access/jna#jna)     | Nothing       |
|[jna-platform-5.8.0.jar](https://github.com/java-native-access/jna#jna-platform) | Nothing|
