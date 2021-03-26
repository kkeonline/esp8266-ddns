#include <ESP8266mDNS.h>
#include <ESP8266WiFiMulti.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <EasyDDNS.h>

#define delayTime 10 // Delay between loop
#define wifiTimeout 15000

/*
    For DDNS Providers where you get a token:
      Use this: EasyDDNS.client("domain", "token");
    
    For DDNS Providers where you get username and password: ( Leave the password field empty "" if not required )
      Use this: EasyDDNS.client("domain", "username", "password");
*/
const char* ddnsdomain = "ddns.host.name";
//const char* ddnstoken = "ddnstoken" ;
const char* ddnsuser = "ddnsuser";
const char* ddnspass = "ddnspass" ;

/*
    List of supported DDNS providers:
    - "duckdns"
    - "noip"
    - "dyndns"
    - "dynu"
    - "enom"
    - "all-inkl"
    - "selfhost.de"
    - "dyndns.it"
    - "strato"
    - "freemyip"
    - "afraid.org"
*/
const char* ddnsprovider = "dynu";

// set SSID as many as you may have
const char* ssid1 = "SSID1";
const char* password1 = "PASSWD1";
const char* ssid2 = "SSID2";
const char* password2 = "PASSWD2";
//const char* ssid3 = "SSID3";
//const char* password3 = "PASSWD3";
//const char* ssid4 = "SSID4";
//const char* password4 = "PASSWD4";

const char* OTAname = "ESP01s";
const char* OTApasswd = "OTAPASSWD";

unsigned long nextMillis;        // will store last time LED was updated
int interval = 1000;           // interval at which to blink (milliseconds)

const int ledPin =  LED_BUILTIN;// the number of the LED pin
int led = HIGH;             // ledState used to set the LED

bool wifiConnected = true;

WiFiUDP Udp;
ESP8266WiFiMulti wifiMulti;

void Blink(){
  if(led){
    //digitalWrite(ledPin, LOW); // full bright LED
    analogWrite(ledPin, 880); // Dim LED
  }else{
    digitalWrite(ledPin, HIGH);
  }
  led=!led;
}

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, HIGH);

  wifiMulti.addAP(ssid1, password1);
  wifiMulti.addAP(ssid2, password2);
//  wifiMulti.addAP(ssid3, password3);
//  wifiMulti.addAP(ssid4, password4);

  Serial.print("Connecting to wifi ");
  unsigned long Millis = millis();
  while (wifiMulti.run() != WL_CONNECTED) { // Wait for the Wi-Fi to connect
    if (millis() - Millis >= wifiTimeout) {
      wifiConnected=false;
      Serial.println(" Fail!");
      break;  
    }
    Serial.print(".");
    delay(100);
    Blink();
  }
  
  if(wifiConnected) {
    Serial.println(" Successful");
  }
 
  ArduinoOTA.setHostname(OTAname);
  ArduinoOTA.setPassword(OTApasswd);
  ArduinoOTA.onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH) {
      type = "sketch";
    } else { // U_FS
      type = "filesystem";
    }
  });
  ArduinoOTA.begin();

  EasyDDNS.service(ddnsprovider);

  /*
    For DDNS Providers where you get a token:
      Use this: EasyDDNS.client("domain", "token");
    
    For DDNS Providers where you get username and password: ( Leave the password field empty "" if not required )
      Use this: EasyDDNS.client("domain", "username", "password");
  */
  //EasyDDNS.client(ddnsdomain, ddnstoken); // Enter your DDNS Domain & Token
  EasyDDNS.client(ddnsdomain, ddnsuser, ddnspass); // Enter your DDNS Domain & user passwd

  // Get Notified when your IP changes
  EasyDDNS.onUpdate([&](const char* oldIP, const char* newIP){
    Serial.print("EasyDDNS - IP Change Detected: ");
    Serial.println(newIP);
  });

  nextMillis = millis()+interval;
  Serial.println();
}

void loop(){
    if (wifiMulti.run() == WL_CONNECTED) {
      wifiConnected=true;
      interval=1000;
    } else {
      wifiConnected=false;
      interval=500;
    }
    if(wifiConnected) {
      ArduinoOTA.handle();
      EasyDDNS.update(15000);
    }

    if (millis() >= nextMillis) {
        while(nextMillis <= millis() ) {
            nextMillis+=interval;
        }
        Blink();
    } 
  delay(delayTime);
}
