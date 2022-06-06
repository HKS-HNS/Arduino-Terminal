import argparse
import ctypes
import os
import platform
import re
import socket
import subprocess
import time
import psutil
from os import path


parser = argparse.ArgumentParser(description='My Python3 Arduino Server')  # add the arguments
requiredNamed = parser.add_argument_group('required arguments')
switch = requiredNamed.add_mutually_exclusive_group(required=True)
switch.add_argument("-e", "--exec", help="Open's this Program")
switch.add_argument("-pi", "--pid", help="Displays the Process", type=int)
requiredNamed.add_argument("-p", "--port", help="Sets the Port", type=int, required=True)
parser.add_argument("-d", "--debug", help="Adds Debug messages", type=bool, required=False)
args = parser.parse_args()
port = args.port
pid = args.pid
exec = args.exec
debug = args.debug

logo = ("\x1b[0m\n"
        + "            ___             __      _                 ______                    _             __\r\n"
        + "           /   |  _________/ /_  __(_)___  ____      /_  __/__  _________ ___  (_)___  ____ _/ /\r\n"
        + "          / /| | / ___/ __  / / / / / __ \\/ __ \\______/ / / _ \\/ ___/ __ `__ \\/ / __ \\/ __ `/ / \r\n"
        + "         / ___ |/ /  / /_/ / /_/ / / / / / /_/ /_____/ / /  __/ /  / / / / / / / / / / /_/ / /  \r\n"
        + "        /_/  |_/_/   \\__,_/\\__,_/_/_/ /_/\\____/     /_/  \\___/_/  /_/ /_/ /_/_/_/ /_/\\__,_/_/   \r\n"
        + "                                                                                        \r\n"
        + "")


def load_Title():  # Show the title
    ctypes.windll.kernel32.SetConsoleTitleW("Arduino-Terminal - By HKS-HNS")


def load_Page():  # Displays the Page
    load_Title()
    os.system('cls')
    print("\n" + logo + "\n")


def pid_become():  # Is to become the right Process ID
    load_Page()
    pid = input("  Set a valid PID: ")
    time.sleep(1)
    if psutil.pid_exists(int(pid)):
        return pid
    else:
        return pid_become()


def port_become():  # Test's if the port matches the structure of a port
    load_Page()
    port = input("  Pleas set a Port up to 65535: ")
    time.sleep(1)
    if not re.match("[a-zA-Z]+", port) and int(port) <= 65535:
        return port
    else:
        return port_become()


def path_become():  # Test's if the path from the file exists and runs it
    load_Page()
    exec = input("  Pleas enter a valid path: ")
    if path.exists(exec):
        return str(exec)
    else:
        return path_become()


def check_host(host):  # Check's if the address is available
    if platform.system().lower() == "windows":
        response = os.system("ping -n 1 -w 500 " + host + " > nul")
        if response == 0:
            return True
        else:
            return False
    else:
        response = os.system("ping -c 1 -W 0.5" + host + "> /dev/null")
        if response == 1:
            return True
        else:
            return False


def getPort():  # Tests if the port makes sens
    if not re.match("[a-zA-Z]+", str(port)) and int(port) <= 65535:
        return port
    else:
        return port_become()


def get_host():  # Asks for the right ip
    load_Page()
    ip = input("  Type the Right IP: ")
    if check_host(ip) is True:
        return ip
    else:
        return get_host()


def getRightIP():  # Asks if this ip is the correct
    load_Page()
    hostname = socket.gethostname()
    IPAddr = socket.gethostbyname(hostname)
    print("  IPAdress: " + IPAddr)
    if input("  Right? type Yes: ").lower() == "yes".lower():
        return IPAddr
    else:
        return get_host().replace(" ", "")


def PrintDebug(Message):  # prints the message if debug is enabled
    if debug is True:
        print(Message)


def processfinder(execute, process_id):  # tests if process exists and returns the PID
    if execute is not None:
        if path.exists(execute):
            pid = subprocess.Popen(execute, shell=True).pid
            return str(pid)
        else:
            pid = subprocess.Popen(path_become(), shell=True).pid
            return str(pid)
    elif process_id is not None:
        if psutil.pid_exists(int(process_id)):
            return str(process_id)
        else:
            return str(pid_become())


def Main():  # The Main method for all to run
    Port = getPort()
    IP = getRightIP()
    PID = processfinder(exec, pid)
    load_Page()
    print("  \033[94mINFO:\x1b[0m")
    print("  If you want to stop the server Press:")
    print("  CTRL-Break")
    print("  ")
    input("  Press Enter to continue...")
    time.sleep(0.5)
    load_Page()
    print("  \033[92mStarting Server...\x1b[0m")
    time.sleep(1)
    load_Page()
    print("  PID: " + str(PID) + " IP: " + IP + ":" + str(Port))
    Server(IP, Port, PID)


def Server(HOST, PORT, PID):  # The Server sends the client the required information
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.bind((HOST, int(PORT)))
        s.listen()
        try:
            print("  Server started")
            while True:
                conn, addr = s.accept()
                with conn:
                    while True:
                        data = conn.recv(1024)
                        if not data:
                            break
                        if "Ram".lower() in str(data.lower()):  # Sends the Used Ram from this Process
                            PrintDebug("  Ram")
                            if psutil.pid_exists(int(PID)):
                                process = psutil.Process(int(PID))
                                MB = "MB: " + str(round(process.memory_info().rss / 1e+6, 2))
                                data = str.encode(MB)
                                PrintDebug("  " + MB)
                            else:
                                data = b"MB: 0.0"
                                PrintDebug("  MB: 0.0")
                        elif "CPUusage".lower() in str(data.lower()):  # Sends the System cpu usage
                            PrintDebug("  CPUusage")
                            cpu = str(round(psutil.cpu_percent(interval=0.1), 2)) + "%"
                            data = str.encode(cpu)
                            PrintDebug("  " + cpu)
                        else:
                            PrintDebug(str(data))

                        conn.sendall(data)
        except KeyboardInterrupt:
            exit(0)


Main()
