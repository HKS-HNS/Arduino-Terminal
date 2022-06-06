#include <Arduino.h>
#include"TFT_eSPI.h"
#include"rpcWiFi.h"
TFT_eSPI tft;

///////////////////////[Config Before Uploading]///////////////////////
//                                                                   //
String Password = "1234"; //Enter a PIN up to 12 Numbers             //
//                                                                   //
const char* WifiSSID = "SS1D"; //Enter The SSID form your Wifi       //
//                                                                   //
const char* WifiPassword = "Passw0rd"; //Enter your Wifi Password    //
//                                                                   //
const char* ServerIp = "192.168.1.45"; //Enter the Server ip         //
//                                                                   //
const uint16_t ServerPort = 342; //Enter your Server Port            //
//                                                                   //
/////////////////////////////{Information}/////////////////////////////
//                                                                   //
// @author HKS-HNS                                                   //
//                                                                   //
// @Dependencies:                                                    //
//                                                                   //
// Seeed Arduino rpcWiFi, seeed rpcUnified, Seeed Arduino FS         //
// Seeed Arduino mbedtls                                             //
//                                                                   //
///////////////////////////////////////////////////////////////////////



WiFiClient client; // Client from Wifi
int Screen = 0;  // Screen Number
int Button = 0;  // Selected Button
int Viewnm = 0;  // Selected Number for Login
int on; // If Internet is connected
int delaysr = 1001; // For delay from server to here
String Dysplay = ""; // String for text on dysplay
String Usedmem = "MB:"; // Save current used memory
boolean wifi = false; // That wifi only ones get executed

void setup() {



  // Put your setup code here, to run once:
  tft.begin(); // Start Screen
  tft.setRotation(3);
  screen(0);
  pinMode(WIO_KEY_A, INPUT_PULLUP);
  pinMode(WIO_KEY_B, INPUT_PULLUP);
  pinMode(WIO_KEY_C, INPUT_PULLUP);
  pinMode(WIO_5S_UP, INPUT_PULLUP);
  pinMode(WIO_5S_DOWN, INPUT_PULLUP);
  pinMode(WIO_5S_LEFT, INPUT_PULLUP);
  pinMode(WIO_5S_RIGHT, INPUT_PULLUP);
  pinMode(WIO_5S_PRESS, INPUT_PULLUP);
  pinMode(WIO_5S_UP, INPUT_PULLUP);
  WiFi.mode(WIFI_STA);
  WiFi.disconnect(); // Disconnect from other wifis
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    WiFi.begin(WifiSSID, WifiPassword);
  }
}

void loop() {
  if (Screen == 0) {
    if (digitalRead(WIO_5S_PRESS) == LOW || digitalRead(WIO_KEY_A) == LOW || digitalRead(WIO_KEY_B) == LOW || digitalRead(WIO_KEY_C) == LOW || digitalRead(WIO_5S_RIGHT) == LOW || digitalRead(WIO_5S_LEFT) == LOW || digitalRead(WIO_5S_DOWN) == LOW || digitalRead(WIO_5S_UP) == LOW) {
      //if any input
      screen(1);
    }
  }




  if (Screen == 0) { // Controll for if Wifi is connected
    if (WiFi.status() != WL_CONNECTED) {
      WiFi.begin(WifiSSID, WifiPassword);
      if (on == 1) {
        tft.fillRect(0, 100, 320, 30, TFT_GREEN);
        tft.setTextSize(2);
        tft.drawString("Wait for Internet", 59.3, 100);
        tft.setTextSize(3);
        on = 0;
      }
    } else {
      if (on == 0) {
        tft.fillRect(0, 100, 320, 30, TFT_GREEN);
        tft.setTextSize(2);
        tft.drawString("Press any Button", 65.2, 100);
        tft.setTextSize(3);
        on = 1;
      }
    }

  }

  if (Screen == 1) {

    if (Button  == 0 && digitalRead(WIO_5S_RIGHT) == LOW) { // Button Switch function
      tft.fillScreen(TFT_GREEN); // Button Back
      tft.fillRoundRect(35, 35, 95, 30, 3, TFT_GREEN);
      tft.fillRoundRect(195, 35, 100, 30, 3, TFT_LIGHTGREY);
      lbtn();
      Button  = 1;
    } else if (Button == 1 && digitalRead(WIO_5S_LEFT) == LOW ) {
      tft.fillScreen(TFT_GREEN); //Button Login
      tft.fillRoundRect(35, 35, 80, 30, 3, TFT_LIGHTGREY);
      tft.fillRoundRect(195, 35, 100, 30, 3, TFT_GREEN);
      lbtn();
      Button  = 0;
    } else if (digitalRead(WIO_5S_PRESS) == LOW) {
      button(Button);//Selected Button click
    } else if (digitalRead(WIO_KEY_C)  == LOW) {
      screen(0);//Back to Home Screen
    }

  }

  if (Screen == 2) {
    if (digitalRead(WIO_KEY_C) == LOW) {
      screen(1);//Back to Menu
    } else if (Button  == 0 && digitalRead(WIO_5S_RIGHT) == LOW ) { // Select Number For Password
      numbersel(Viewnm, "Right");
    } else if (Button  == 0 && digitalRead(WIO_5S_LEFT) == LOW) {
      numbersel(Viewnm, "Left");
    } else if (Button  == 0 && digitalRead(WIO_5S_PRESS) == LOW) {
      setnumb();
    } else if ( digitalRead(WIO_5S_DOWN) == LOW) { //Button REM
      loadpassowrdbtn("Down");

    } else if ( digitalRead(WIO_5S_RIGHT) == LOW) { //Button Right
      loadpassowrdbtn("Right");

    } else if ( digitalRead(WIO_5S_LEFT) == LOW) { //Button Left
      loadpassowrdbtn("Left");

    } else if ( digitalRead(WIO_5S_UP) == LOW) { //Button Up
      loadpassowrdbtn("Up");

    } else if (digitalRead(WIO_5S_PRESS) == LOW) {
      button(Button);//Selected Button click
    }
  }
  if (Screen == 3) {
    if (digitalRead(WIO_KEY_C) == LOW) {
      screen(2);//Back to Menu
    } else if (WiFi.status() != WL_CONNECTED) {
      screen(4);
    } else if ( digitalRead(WIO_5S_DOWN) == LOW) { //Button Down
      loadpassowrdbtn("Down");

    } else if ( digitalRead(WIO_5S_RIGHT) == LOW) { //Button Right
      loadpassowrdbtn("Right");

    } else if ( digitalRead(WIO_5S_LEFT) == LOW) { //Button Left
      loadpassowrdbtn("Left");

    } else if ( digitalRead(WIO_5S_UP) == LOW) { //Button Up
      loadpassowrdbtn("Up");

    } else if (digitalRead(WIO_5S_PRESS) == LOW) {
      button(Button); //Selected Button click
    }
  }

  if (Screen == 4) {
    if (digitalRead(WIO_KEY_C) == LOW) {
      screen(1);//Back to Menu
    } else if (WiFi.status() != WL_CONNECTED) {
      if (wifi == false) {
        wifi = true;
        WiFi.begin(WifiSSID, WifiPassword);
        wifi = false;
      }
    } else if (WiFi.status() == WL_CONNECTED) {
      screen(3);

    }
  }

  if (Screen == 5) {
    if (digitalRead(WIO_KEY_C) == LOW) {

      screen(3); //Back to Menu
    } else if (WiFi.status() != WL_CONNECTED) {
      screen(4); // to NoWifi wait Menu
    }  else if (digitalRead(WIO_5S_PRESS) == LOW) {
      screen(3); // Main Menu
    } else {
      getinfofromserver("Ram");
    }
  }
  if (Screen == 6) {
    if (digitalRead(WIO_KEY_C) == LOW) {
      screen(3); //Back to Menu
    } else if (WiFi.status() != WL_CONNECTED) {
      screen(4);
    }  else if (digitalRead(WIO_5S_PRESS) == LOW) {
      screen(3); // Main Menu
    } else {
      getinfofromserver("CPUusage");
    }



  }
  delay(150);
}


void setnumb() { // Add Number to Screen
  if (Dysplay.length() <= 12 && Dysplay != "Error") {
    tft.fillRect(0, 0, 320, 50, TFT_DARKGREEN);
    Dysplay = Dysplay + String(Viewnm);
    tft.drawString(Dysplay, 5, 17);
  } else if (Dysplay == "Error") {
    tft.fillRect(0, 0, 320, 50, TFT_DARKGREEN);
    Dysplay = String(Viewnm);
    tft.drawString(Dysplay, 5, 17);
  } else  {
    tft.fillRect(0, 0, 320, 50, TFT_DARKGREEN);
    Dysplay = "Error";
    tft.drawString(Dysplay, 5, 17);

  }


}

void numbersel(int num, String Direct) { // To switch Dysplayed Numbers
  if (Direct == "Right") {
    if (num == 9) {
      tft.fillRoundRect(170 - 18, 50, 21, 30, 3, TFT_LIGHTGREY);
      tft.drawString(String(0), 170 - 15, 55);
      Viewnm = 0;

    } else {
      tft.fillRoundRect(170 - 18, 50, 21, 30, 3, TFT_LIGHTGREY);
      tft.drawString(String(num + 1), 170 - 15, 55);
      Viewnm++;
    }
  } else if (Direct == "Left") {
    if (num == 0) {
      tft.fillRoundRect(170 - 18, 50, 21, 30, 3, TFT_LIGHTGREY);
      tft.drawString(String(9), 170 - 15, 55);
      Viewnm = 9;


    } else {
      tft.fillRoundRect(170 - 18, 50, 21, 30, 3, TFT_LIGHTGREY);
      tft.drawString(String(num - 1), 170 - 15, 55);
      Viewnm--;
    }
  }
}

void screen(int num) { // To select the screen
  Button = 0;
  delaysr = 1001;
  if (num == 1) { // Menu Screen
    tft.fillScreen(TFT_GREEN);
    tft.fillRect(35, 35, 80, 30, TFT_LIGHTGREY);
    tft.fillRect(195, 35, 100, 30, TFT_GREEN);
    Screen = num;
    lbtn();

    Screen = num;
  } else if (num == 0) { // Start Screen
    tft.fillScreen(TFT_GREEN);
    tft.setTextColor(TFT_BLACK);
    tft.setTextSize(2);
    tft.drawString("Welcome to my Terminal", 27, 50);
    if (WiFi.status() != WL_CONNECTED) {
      tft.drawString("Wait for Internet", 59.3, 100);
    } else {
      tft.drawString("Press any Button", 65.2, 100);
    }
    tft.setTextSize(3);

    Screen = num;
  } else if (num == 2) { // Login Screen
    tft.setTextSize(3);
    Viewnm = 0;
    Screen = num;
    Dysplay = "";
    btn0();
  } else if (num == 3) { // Server Screen
    tft.setTextSize(3);
    Screen = num;
    Button = 0;
    if (WiFi.status() == WL_CONNECTED) {
      btn0();
    } else if (WiFi.status() != WL_CONNECTED) {
      screen(4);
    }

  } else if (num == 4) { // No wifi Screen
    tft.setTextSize(3);

    Screen = num;
    wifi = false;
    btn0();
  } else if (num == 5) { // No Ram Screen
    Usedmem = "MB:";
    tft.setTextSize(3);
    Screen = num;
    wifi = false;
    if (WiFi.status() == WL_CONNECTED) {
      btn0();
    } else if (WiFi.status() != WL_CONNECTED) {
      screen(4);
    }
  } else if (num == 6) { // CPU Screen
    tft.fillRect(0, 125, 320, 50, TFT_GREEN);
    tft.drawString("Usage: 0%", 31, 125);
    tft.setTextSize(3);
    Screen = num;
    wifi = false;
    if (WiFi.status() == WL_CONNECTED) {
      btn0();
    } else if (WiFi.status() != WL_CONNECTED) {
      screen(4);
    }
  }
}

void loadpassowrdbtn(String Direction) {// Button selection
  tft.setTextSize(3);
  Viewnm = 0;
  if (Button == 0 && Direction == "Down") {// Button 1
    if (Screen == 2) {
      Button = 3;
      btn3();
    } else if (Screen == 3) {
      Button = 2;
      btn2();
    }


  } else if (Button == 0 && Direction == "Right") {// Button 1
    if (Screen == 2) {
      return;
    } else if (Screen == 3) {
      Button = 1;
      btn1();
    }


  } else if (Button == 1 && Direction == "Up") {// Button 2
    if (Screen == 2) {
      Button = 0;
      btn0();
    } else if (Screen == 3) {
      return;
    }


  } else if (Button == 1 && Direction == "Left") {// Button 2
    if (Screen == 2) {
      Button = 2;
      btn2();
    } else if (Screen == 3) {
      Button = 0;
      btn0();
    }


  } else if (Button == 1 && Direction == "Down") {// Button 2
    if (Screen == 2) {
      Button = 3;
      btn3();
    } else if (Screen == 3) {
      Button = 2;
      btn2();
    }


  } else if (Button == 2 && Direction == "Down") {// Button 3
    if (Screen == 2) {
      Button = 3;
      btn3();
    } else if (Screen == 3) {
      return;
    }


  } else if (Button == 2 && Direction == "Up") {// Button 3
    if (Screen == 2) {
      Button = 0;
      btn0();
    } else if (Screen == 3) {
      Button = 0;
      btn0();
    }


  } else if (Button == 2 && Direction == "Right") {// Button 3
    if (Screen == 2) {
      Button = 1;
      btn1();
    } else if (Screen == 3) {
      Button = 1;
      btn1();
    }


  } else if (Button == 3 && Direction == "Right") {// Button 4
    if (Screen == 2) {
      Button = 1;
      btn1();
    } else if (Screen == 3) {
      return;
    }


  } else if (Button == 3 && Direction == "Left") {// Button 4
    if (Screen == 2) {
      Button = 2;
      btn2();
    } else if (Screen == 3) {
      return;
    }


  } else if (Button == 3 && Direction == "Up") {// Button 4
    if (Screen == 2) {
      Button = 0;
      btn0();
    } else if (Screen == 3) {
      return;
    }


  } else {

    return;
  }




}

void getinfofromserver(String Key) {
  delaysr = (delaysr + 150);
  if (3000 < delaysr) {
    if (!client.connect(ServerIp, ServerPort)) {
      return;
    } else {

      client.print(Key); // Send String to server
      delaysr = 0; 
      int maxloops = 0;

      while (!client.available() && maxloops < 500) { // Wait for reply
        maxloops++;
      }
      if (client.available() > 0) { // Print message from server

        String cache = client.readString(); // Cache for the info from the server
        if (Screen == 5) {
          if (!Usedmem.equals(cache)) {
            Usedmem = cache;
            tft.fillRect(0, 125, 320, 50, TFT_GREEN);
            tft.drawString(Usedmem, 31, 125);
          }  else {
            client.stop();

          }
        } else if (Screen == 6) {

          Usedmem = cache;
          tft.fillRect(0, 125, 320, 50, TFT_GREEN);
          tft.drawString("Usage: " + Usedmem, 31, 125);

        }

      } else {
      }

      client.stop(); // Disconnecting from server

    }
  } else {
    return;

  }

}

void btn0() { // Button 1
  if (Screen == 2) {
    tft.fillRect(0, 50, 320, 300, TFT_GREEN);
    tft.fillRect(0, 0, 320, 50, TFT_DARKGREEN);
    tft.fillRoundRect(170 - 18, 50, 21, 30, 3, TFT_LIGHTGREY); // 0
    tft.fillRoundRect(255 - 18, 80, 57, 30, 3, TFT_DARKGREY); // REM
    tft.fillRoundRect(26, 80, 80, 30, 3, TFT_DARKGREY); // Back
    tft.fillRoundRect(110, 170, 100, 30, 3, TFT_DARKGREY); // Login
  } else if (Screen == 3) {
    tft.fillScreen(TFT_GREEN);
    tft.fillRoundRect(26, 180, 80, 30, 3, TFT_DARKGREY); // Back
    tft.fillRoundRect(26, 50, 60, 30, 3, TFT_LIGHTGREY); // Ram
    tft.fillRoundRect(234, 50, 60, 30, 3, TFT_DARKGREY); // Console
  } else if (Screen == 4) {
    tft.fillScreen(TFT_GREEN);
    tft.fillRoundRect(26, 180, 80, 30, 3, TFT_LIGHTGREY); // Back
  } else if (Screen == 5) {
    tft.fillScreen(TFT_GREEN);
    tft.fillRoundRect(26, 180, 80, 30, 3, TFT_LIGHTGREY); // Back
    tft.fillRect(0, 125, 320, 50, TFT_GREEN);
    tft.drawString("MB: 0.0", 31, 125);
  } else if (Screen == 6) {
    tft.fillScreen(TFT_GREEN);
    tft.fillRoundRect(26, 180, 80, 30, 3, TFT_LIGHTGREY); // Back
    tft.fillRect(0, 125, 320, 50, TFT_GREEN);
    tft.drawString("Usage: 0%", 31, 125);
  }
  lbtn();
}
void btn1() {
  if (Screen == 2) {// Button 2
    tft.fillRect(0, 50, 320, 300, TFT_GREEN);
    tft.fillRect(0, 0, 320, 50, TFT_DARKGREEN);
    tft.fillRoundRect(170 - 18, 50, 21, 30, 3, TFT_DARKGREY); // 0
    tft.fillRoundRect(255 - 18, 80, 57, 30, 3, TFT_LIGHTGREY); // REM
    tft.fillRoundRect(26, 80, 80, 30, 3, TFT_DARKGREY); // Back
    tft.fillRoundRect(110, 170, 100, 30, 3, TFT_DARKGREY); // Login
  } else if (Screen == 3) {
    tft.fillScreen(TFT_GREEN);
    tft.fillRoundRect(26, 180, 80, 30, 3, TFT_DARKGREY); // Back
    tft.fillRoundRect(26, 50, 60, 30, 3, TFT_DARKGREY); // Ram
    tft.fillRoundRect(234, 50, 60, 30, 3, TFT_LIGHTGREY); // Console
  }
  lbtn();
}
void btn2() { // Button 3
  if (Screen == 2) {
    tft.fillRect(0, 50, 320, 300, TFT_GREEN);
    tft.fillRect(0, 0, 320, 50, TFT_DARKGREEN);
    tft.fillRoundRect(170 - 18, 50, 21, 30, 3, TFT_DARKGREY); // 0
    tft.fillRoundRect(255 - 18, 80, 57, 30, 3, TFT_DARKGREY); // REM
    tft.fillRoundRect(26, 80, 80, 30, 3, TFT_LIGHTGREY); // Back
    tft.fillRoundRect(110, 170, 100, 30, 3, TFT_DARKGREY); // Login
  } else if (Screen == 3) {
    tft.fillScreen(TFT_GREEN);
    tft.fillRoundRect(26, 180, 80, 30, 3, TFT_LIGHTGREY); // Back
    tft.fillRoundRect(26, 50, 60, 30, 3, TFT_DARKGREY); // Ram
    tft.fillRoundRect(234, 50, 60, 30, 3, TFT_DARKGREY); // Console
  }
  lbtn();
}
void btn3() {// Button 4
  if (Screen == 2) {
    tft.fillRect(0, 50, 320, 300, TFT_GREEN);
    tft.fillRect(0, 0, 320, 50, TFT_DARKGREEN);
    tft.fillRoundRect(170 - 18, 50, 21, 30, 3, TFT_DARKGREY); // 0
    tft.fillRoundRect(255 - 18, 80, 57, 30, 3, TFT_DARKGREY); // REM
    tft.fillRoundRect(26, 80, 80, 30, 3, TFT_DARKGREY); // Back
    tft.fillRoundRect(110, 170, 100, 30, 3, TFT_LIGHTGREY); // Login
  } else if (Screen == 3) {

  }
  lbtn();
}

void lbtn() { // Add to Login
  if (Screen == 2) {
    tft.drawString(Dysplay, 5, 17);
    tft.drawString("Login", 115, 175);
    tft.drawString("Back", 31, 85);
    tft.drawString("0", 170 - 15, 55);
    tft.drawString("REM", 255 - 15 , 85);
  } else if (Screen == 1) {

    tft.drawString("Back", 40, 40);
    tft.drawString("Login", 200, 40);

  } else if (Screen == 3) {
    tft.drawString("Back", 31, 185);
    tft.drawString("Ram", 31, 55);
    tft.drawString("CPU", 239, 55);

  } else if (Screen == 4) {
    tft.drawString("exit", 31, 185);
    tft.setTextSize(2);
    tft.drawString("Wait for Internet", 31, 55);
    tft.setTextSize(3);
  } else if (Screen == 5) {
    tft.drawString("Back", 31, 185);
  } else if (Screen == 6) {
    tft.drawString("Back", 31, 185);
  }


}

void button(int button1) { // Button control
  if (Screen == 1) {
    if (button1 == 0) {
      screen(0);
    } else if (button1 == 1) {
      screen(2);
    }
  } else if (Screen == 2) {
    if (button1 == 1) {
      if (!Dysplay.equals("Error")) {
        tft.fillRect(0, 0, 320, 50, TFT_DARKGREEN);
        Dysplay.remove(Dysplay.length() - 1, 1);
        tft.drawString(Dysplay, 5, 17);
      } else {
        tft.fillRect(0, 0, 320, 50, TFT_DARKGREEN);
        Dysplay = "";
      }

    } else if (button1 == 2) {
      screen(1);
    } else if (button1 == 3) {
      if (Dysplay == Password) {
        screen(3);
      } else {
        tft.fillRect(0, 0, 320, 50, TFT_DARKGREEN);
        Dysplay = "Error";
        tft.drawString(Dysplay, 5, 17);
      }
    }
  } else if (Screen == 3) {
    if (button1 == 0) {
      screen(5);
    } else if (button1 == 1) {
      screen(6);
    } else if (button1 == 2) {
      screen(2);
    }
  }
}
