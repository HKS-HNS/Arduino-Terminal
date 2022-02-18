package me.HKS.HNS;
import java.io.*;
import java.net.InetAddress;
import java.net.ServerSocket;
import java.net.Socket;
import java.net.UnknownHostException;
import java.util.ArrayList;
import java.util.Scanner;
import java.util.regex.Pattern;

/***
 * This is my server for TCP incoming for the Arduino
 * @author HKS-HNS
 * @since 9
 */
public class Main {
  int port = 9222; // for the Standard Port
  Boolean Set = false;
  static Scanner ac = new Scanner(System.in);
  public static void main(String[] args) throws NumberFormatException, IOException {
    Main server = new Main();
    if (args.length >= 1) {
      ArrayList < String > arguments = new ArrayList < String > ();
      for (int i = 0; i < args.length; i++) {
        if (args[i].contains("-")) {

          arguments.add(args[i].toLowerCase());
        } else {
          arguments.add(args[i]);
        }
      }

      if (args.length <= 4) {
        if (arguments.contains("--help") || arguments.contains("-h")) { // Shows the help message
          server.messages("help");
          return;
        }
        if (arguments.contains("--port") || arguments.contains("-p")) { // Set's the port
          String Port = null;
          if (arguments.contains("--port")) {
            Port = args[arguments.indexOf("--port") + 1];
          } else if (arguments.contains("-p")) {
            Port = args[arguments.indexOf("-p") + 1];
          }

          if (Pattern.matches("[a-zA-Z]+", Port) == false && Port.length() <= 65535) { // locks if the port is correct writen
            server.port = Integer.parseInt(Port);
          } else {
            server.messages("port");
          }
        }
        if (arguments.contains("--exec") || arguments.contains("-e")) { // executes the programm
          String Path = null;
          if (arguments.contains("--exec")) {
            Path = args[arguments.indexOf("--exec") + 1];
          } else if (arguments.contains("-e")) {
            Path = args[arguments.indexOf("-e") + 1];
          }

          File tempFile = new File(Path);
          File tempFileSys = new File("C:\\WINDOWS\\system32\\" + Path);
          if (tempFile.exists() || tempFileSys.exists()) { // tests if file exists

            Process p = server.exec(Path);
            try {
              server.Start(p.pid());
            } catch (IOException e) {
              e.printStackTrace();
            }
          } else {
            ;
            System.out.println("If you wane run " + Path + " type Yes");
            String yesorno = ac.nextLine();
            if (yesorno.equalsIgnoreCase("Yes")) {
            if (Path != "Shutdown") {
              Process p = server.exec(Path);
              try {
                server.Start(p.pid());
              } catch (IOException e) {
                System.out.println(e.getMessage());
                e.printStackTrace();

              }}
            }

          }

        } else if (arguments.contains("--pid") || arguments.contains("-pi")) {

          String pid = null;
          if (arguments.contains("--pid")) {
            pid = args[arguments.indexOf("--pid") + 1];
          } else if (arguments.contains("-pi")) {
            pid = args[arguments.indexOf("-pi") + 1];
          } else {
            return;
          }
          if (isProcessRunning(Long.parseLong(pid))) {
            if (Pattern.matches("[a-zA-Z]+", pid) == false) { // locks if the pid is a number

              try {
                server.Start(Long.parseLong(pid));
              } catch (IOException e) {
                System.out.println(e.getMessage());
                e.printStackTrace();

              }

            } else {
              server.messages("pid");
            }
          }
        } else {
          server.messages("help"); // Show's the help message
        }

      }

    } else {
      server.messages("help");
    }

  }
  void Start(long pid) throws IOException { // starts the server
    InetAddress IPv4;
    if (isrightip()) {
      IPv4 = InetAddress.getByName(InetAddress.getLocalHost().getHostAddress());
    } else {

      String ip = getrightip();
      IPv4 = InetAddress.getByName(ip);
    }
    System.out.println("Start Server");

    ServerSocket serverSocket = new ServerSocket(port, 5, IPv4);

    while (true) {

      Socket client = waitforlogin(serverSocket);
      String message = readmessage(client);
      System.out.println(message);
      writemessage(client, message, pid);

    }
  }

  Socket waitforlogin(ServerSocket serverSocket) throws IOException { // waits for login
    Socket socket = serverSocket.accept();
    return socket;
  }

  String readmessage(Socket socket) throws IOException { // becoms the message and read it
    BufferedReader bufferedReader = new BufferedReader(new InputStreamReader(socket.getInputStream()));
    char[] buffer = new char[200];
    int howmany = bufferedReader.read(buffer, 0, 200); // block before message getted
    String message = new String(buffer, 0, howmany);
    return message;
  }

  void writemessage(Socket socket, String message, long pid) throws IOException { // writs the reply 
    PrintWriter printWriter = new PrintWriter(new OutputStreamWriter(socket.getOutputStream()));
    if (message.equalsIgnoreCase("Ram")) { // send's the ram if program (Server) started

      if (isWindows()) {
        ProcessBuilder builder = new ProcessBuilder("cmd.exe", "/c", "wmic process where processid=" + pid + " get WorkingSetSize");
        print(builder, "Ram", pid, printWriter);
      } else if (isLinux()) {
        ProcessBuilder builder = new ProcessBuilder("/bin/sh", "-c", "ps -p " + pid + " -o %mem");
        print(builder, "Ram", pid, printWriter);

      }

    } else if (message.equalsIgnoreCase("CPUusage")) { // send's the CPU usage 

      if (isWindows()) {
        ProcessBuilder builder = new ProcessBuilder("Powershell.exe", "-Command", "gwmi Win32_PerfFormattedData_PerfOS_Processor | Select -First 1 | %{'{0}%' -f $_.PercentProcessorTime}");

        print(builder, "CPU", pid, printWriter);
      } else if (isLinux()) {

        ProcessBuilder builder = new ProcessBuilder("/bin/sh", "-c", "top -b -n 1 |grep Cpu|cut -d \",\" -f 4|cut -d \" \" -f 2 | awk '{cpu_usage=(100-$1)} END {print cpu_usage \"%\"}'");

        builder.redirectErrorStream(true);
        print(builder, "CPU", pid, printWriter);

      }

    }
  }


  void messages(String messag) {

    if (messag.equalsIgnoreCase("help")) { // sends the help messages
      System.out.println("Commands:\r\n" +
        "\r\n" +
        "        --help / -h                  dsiplays this message\r\n" +
        "        --port / -p    <Port>        sets the Port\r\n" +
        "        --exec / -e    <Program>     open this Programm        or\r\n" +
        "        --pid  / -pi   <Process ID>  Dysplays the Process");

    } else if (messag.equalsIgnoreCase("port")) {

      System.out.println("        --port / -p    <Port>        sets the Port");

    } else if (messag.equalsIgnoreCase("exec")) {

      System.out.println("        --exec / -e    <Program>     open this Programm        or");

    } else if (messag.equalsIgnoreCase("pid")) {

      System.out.println("        --pid  / -pi   <Process ID>  Dysplays ths Process");

    }

  }

  Process exec(String program) { // execute's the program
    try {

      Runtime run = Runtime.getRuntime();
      Process pr = run.exec(program);
      return pr;
    } catch (Exception e) {
      System.out.println(e.getMessage());
      return null;
    }

  }
  public static String print(ProcessBuilder builder, String Component, long pid, PrintWriter printWriter) throws IOException { // is to get the reply from the terminal

    builder.redirectErrorStream(true);
    Process p = builder.start();

    BufferedReader r = new BufferedReader(new InputStreamReader(p.getInputStream()));
    String line = "";
    while (true) {
      line = r.readLine();
      if (line == null) {
        break;
      }
      if (!line.matches("\\p{IsWhite_Space}*")) {
        if (Component.equalsIgnoreCase("CPU")) {
          System.out.println(line);
          printWriter.print(line);
        } else if (Component.equalsIgnoreCase("Ram") && !line.contains("WorkingSetSize") && !line.contains("%MEM")) {
          if (isProcessRunning(pid)) { 
            if (isLinux()) {
              System.out.println("MB: " + line);
              printWriter.print("MB: " + line);
            } else if (isWindows()) {

              System.out.println("MB: " + (double) Math.round(((Double.parseDouble(line)) / 1e+6) * 100) / 100);
              printWriter.print("MB: " + (double) Math.round(((Double.parseDouble(line)) / 1e+6) * 100) / 100);
            }
          } else {
            System.out.println("00");
            printWriter.print("MB: " + 0.0);

          }

        }
      }

    }

    printWriter.flush();
    return line;
  }

  public static boolean isLinux() { // tests if linux / unix
    String os = System.getProperty("os.name");
    return os.toLowerCase().indexOf("nix") >= 0 || os.toLowerCase().indexOf("nux") >= 0;
  }

  public static boolean isWindows() { // tests if Windows
    String os = System.getProperty("os.name");
    return os.toLowerCase().indexOf("windows") >= 0;
  }

  public static String getrightip() throws IOException {

    int time = 0;
    while (time <= 100) {
      System.out.println("Type the Right IP");
      String yessorno = ac.nextLine();

      if (InetAddress.getByName(yessorno).isReachable(100)) {
        ac.close();
        return yessorno;

      } else {
        time++;
        System.out.println("The ip " + yessorno + " Is not reachable or not valid");
      }
    }
    System.out.println("Pleas restart tanks");
    ac.close();
    System.exit(0);
    return "";

  }

  public static Boolean isrightip() throws UnknownHostException {

    System.out.println("IPAdress: " + InetAddress.getLocalHost().getHostAddress() + "");
    System.out.println("Right? type Yes");
    String yesorno = ac.nextLine();
    if (yesorno.equalsIgnoreCase("Yes")) {
      return true;
    } else {
      return false;
    }
  }

  public static boolean isProcessRunning(long pid) throws IOException { // tests if a process is running 
    ProcessBuilder builder;
    if (isWindows()) {
      builder = new ProcessBuilder("Powershell.exe", "-Command", "if((get-process -id " + pid + " -ea SilentlyContinue) -eq $Null){ echo \"Not Running\" }else{ echo \"isRunning\"}");

    } else {
      builder = new ProcessBuilder("/bin/sh", "-c", "ps -p " + pid);

    }
    ArrayList < String > output = new ArrayList < String > ();
    builder.redirectErrorStream(true);
    Process p = builder.start();
    BufferedReader r = new BufferedReader(new InputStreamReader(p.getInputStream()));
    Boolean Condition = false;
    String line1 = "";
    while ((line1 = r.readLine()) != null) {
      if (!line1.matches("\\p{IsWhite_Space}*")) {

        output.add(line1);
        if (isLinux()) {
          if (output.size() == 2) {

            Condition = true;
          } else {

            Condition = false;

          }

        }
        if (isWindows()) {
          if (line1.toLowerCase().contains("isrunning")) {

            Condition = true;
          } else {
            Condition = false;
          }

        }

      }
    }
    return Condition;

  }
}