#include <M5StickC.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <BLEDevice.h>                 // To turn BLE off for powersafing
#include <OSCBundle.h>                 // for receiving OSC messages
#include <WiFiUdp.h>                   //UDP

const char* ssid = "THJ-Inspi";  //Credentials to register network defined by the SSID (Service Set IDentifier)
const char* password = "{,ulaS:HXV}m1*%VxG-mhje"; //and the second one a password if you wish to use it.
const char* hostid = "CUE2"; //Name it before you blame it
//IPAddress local_IP(192, 168, 100, 230);
//IPAddress gateway(192, 168, 100, 5);
//IPAddress subnet(255, 255, 255, 0);
//IPAddress primaryDNS(192, 168, 100, 5);   // optional
//IPAddress secondaryDNS(8, 8, 4, 4); // optional
//Pinger pinger;

//UDP
WiFiUDP Udp;                           // A UDP instance to let us send and receive packets over UDP
const unsigned int localPort = 8000;   // local port to listen for UDP packets(another device must send OSC messages to this port)
const unsigned int destPort = 9000;    // remote port of the target device where we send OSC to

const int ledPin = 10;    // LED-Pin
int OSCueLightID = 2;     // OSCueLightID
String pathOSC = "/oscuelight/"; //OSC-String
char hierlang[42];    // buffer for OSC-String + ID

void setup(){
  setCpuFrequencyMhz(80);
  esp_bt_controller_disable();
  //adc_power_release();
  M5.begin();
  pinMode (ledPin, OUTPUT);
//  if (!WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS)) {
//   Serial.println("STA Failed to configure");
//  }
  WiFi.mode(WIFI_STA);
  WiFi.hostname(hostid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
  } 
  Udp.begin(localPort);
  delay(1000);
  M5.Lcd.setRotation(1);
  M5.Lcd.fillScreen(GREEN);
  M5.Lcd.setCursor(10, 10, 2);
  M5.Lcd.print(WiFi.localIP());
  delay(1000);
  M5.Lcd.fillScreen(BLUE);
  M5.Lcd.setCursor(10, 10, 2);
  M5.Lcd.print(WiFi.localIP());
  delay(1000);
  M5.Lcd.fillScreen(RED);
  M5.Lcd.setCursor(10, 10, 2);
  M5.Lcd.print(WiFi.localIP());//NETWORK
   //INSERT SETUP MENUE HIER
  pathOSC = pathOSC + String(OSCueLightID);
  M5.Lcd.setCursor(10, 30, 2);
  M5.Lcd.print(pathOSC);
  digitalWrite (ledPin, HIGH);
  M5.Axp.ScreenBreath(7);
  screenoff ();
}


void loop() {
  OSCMessage msgIN;
  int size;
  if ((size = Udp.parsePacket()) > 0) {
    while (size--)
      msgIN.fill(Udp.read());
    if (!msgIN.hasError()) {
      pathOSC.toCharArray(hierlang,42);
      msgIN.dispatch(strcat(hierlang, "/color"), colorscreen);
      pathOSC.toCharArray(hierlang,42);
      msgIN.dispatch(strcat(hierlang, "/write"), write);
      pathOSC.toCharArray(hierlang,42);
      msgIN.dispatch(strcat(hierlang, "/led"), led);
      pathOSC.toCharArray(hierlang,42); 
      msgIN.dispatch(strcat(hierlang, "/clear"), clear);
      pathOSC.toCharArray(hierlang,42);
      msgIN.dispatch(strcat(hierlang, "/achtung"), achtung);
      pathOSC.toCharArray(hierlang,42);
      msgIN.dispatch(strcat(hierlang, "/go"), go);
      pathOSC.toCharArray(hierlang,42);
      msgIN.dispatch(strcat(hierlang, "/countdown"), countdown);
    }
  }
m5.update();
}

void colorscreen(OSCMessage &msg) {
  screenon ();
  M5.Axp.ScreenBreath(15);
  char screencolor [16];
  msg.getString(0, screencolor, 8);
  if (strcmp(screencolor, "blue")==0){
    M5.Lcd.fillScreen(BLUE);   
    M5.Lcd.setTextColor(WHITE, BLUE); 
  }
  if (strcmp(screencolor, "red")==0){
    M5.Lcd.fillScreen(RED);
    M5.Lcd.setTextColor(WHITE, RED); 
  }
  if (strcmp(screencolor, "green")==0){
    M5.Lcd.fillScreen(GREEN);
    M5.Lcd.setTextColor(WHITE, GREEN); 
  }
}

void write(OSCMessage &msg) {
  if (msg.isInt(0)){
    M5.Lcd.setRotation(0);
    M5.Lcd.setCursor(5,5,2);
    M5.Lcd.setTextSize(2);
    M5.Lcd.print("CUE:");
    M5.Lcd.setCursor(4,40,2);
    M5.Lcd.setTextSize(4);  
    M5.Lcd.print(msg.getInt(0));
  }
  if (msg.isString(0)){
    char payload [128];
    M5.Lcd.setRotation(1);
    M5.Lcd.setCursor(5,2,2);
    M5.Lcd.setTextSize(1);
    M5.Lcd.print("NACHRICHT:");
    M5.Lcd.setCursor(2,20,2);
    M5.Lcd.setTextSize(3);
    msg.getString(0,payload,128);
    M5.Lcd.print(payload);
  } 
}


void led(OSCMessage &msg) {
  
}

void clear(OSCMessage &msg) {
  M5.Lcd.fillScreen(BLACK);
  M5.Axp.ScreenBreath(7);
  digitalWrite (ledPin, HIGH);
  screenoff ();
}

void achtung(OSCMessage &msg) {
  screenon ();
  M5.Axp.ScreenBreath(15);
  M5.Lcd.setTextColor(WHITE, GREEN);
  M5.Lcd.fillScreen(BLACK);
  delay(500);
  M5.Lcd.fillScreen(GREEN);
  delay(500);
  M5.Lcd.fillScreen(BLACK);
  delay(500);
  M5.Lcd.fillScreen(GREEN);
  delay(500);
  M5.Lcd.fillScreen(BLACK);
  delay(500);
  M5.Lcd.fillScreen(GREEN);
  delay(500);  
  digitalWrite (ledPin, LOW); 
  if (msg.isInt(0)){
    M5.Lcd.setRotation(0);
    M5.Lcd.setCursor(5,5,2);
    M5.Lcd.setTextSize(2);
    M5.Lcd.print("CUE:");
    M5.Lcd.setCursor(4,40,2);
    M5.Lcd.setTextSize(4);  
    M5.Lcd.print(msg.getInt(0));
  }
  if (msg.isString(0)){
    char payload [128];
    M5.Lcd.setRotation(1);
    M5.Lcd.setCursor(5,2,2);
    M5.Lcd.setTextSize(1);
    M5.Lcd.print("NACHRICHT:");
    M5.Lcd.setCursor(5,20,2);
    M5.Lcd.setTextSize(1);
    msg.getString(0,payload,128);
    M5.Lcd.print(payload);
  } 
}

void go(OSCMessage &msg) {
  M5.Lcd.setTextColor(WHITE, RED);
  digitalWrite (ledPin, HIGH);  
  M5.Lcd.fillScreen(BLACK);
  delay(500);
  M5.Lcd.fillScreen(RED);
  delay(500);
  M5.Lcd.fillScreen(BLACK);
  delay(500);
  M5.Lcd.fillScreen(RED);
  delay(500);
  M5.Lcd.fillScreen(BLACK);
  M5.Axp.ScreenBreath(7);
  screenoff ();
}

void countdown(OSCMessage &msg) {
  if (msg.isInt(0)){
    M5.Axp.ScreenBreath(15);
    screenon ();
    digitalWrite (ledPin, LOW); 
    int number = msg.getInt(0);
    M5.Lcd.setTextSize(6);
    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.setTextColor(WHITE, BLACK);
    M5.Lcd.setRotation(1);
    while (number > 0) {
      M5.Lcd.setCursor(20,0,2);
      M5.Lcd.fillScreen(BLACK);
      if ( (number % 2) == 0){
       M5.Lcd.invertDisplay(false);
       M5.Lcd.print(number);
      }
      else {
       M5.Lcd.invertDisplay(true);
       M5.Lcd.print(number); 
      }
     delay(1000);
      number--;
    }
    if (number == 0) {
  M5.Lcd.setTextColor(WHITE, RED);
  digitalWrite (ledPin, HIGH);  
  M5.Lcd.fillScreen(BLACK);
  delay(500);
  M5.Lcd.fillScreen(RED);
  delay(500);
  M5.Lcd.fillScreen(BLACK);
  delay(500);
  M5.Lcd.fillScreen(RED);
  delay(500);
  M5.Lcd.fillScreen(BLACK);
  M5.Axp.ScreenBreath(7);
  screenoff ();
    }
  }
}

void screenoff () {
  Wire1.beginTransmission(0x34);
  Wire1.write(0x12);
  Wire1.write(0b01001011);  // LDO2, aka OLED_VDD, off
  Wire1.endTransmission();
}

void screenon () {
  Wire1.beginTransmission(0x34);
  Wire1.write(0x12);
  Wire1.write(0x4d); // Enable LDO2, aka OLED_VDD
  Wire1.endTransmission();  
}