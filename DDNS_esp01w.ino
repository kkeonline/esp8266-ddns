#include <ArduinoOTA.h>
#include "ESP8266WiFi.h"
#include "ESP8266HTTPClient.h"
#include <EasyDDNS.h>

#define delayTime 50 // Delay between Frames
#define wifiTimeout 60000
#define bright 128 // led pwm brightness 0=On 255=Off(Esp8266 >= 3.0) 1023=Off(esp8266 < 3)

const char* ssid = "SSID";
const char* password = "PASSWD";
const char* ddnsdomain = "ddnsdomain";
const char* ddnsuser = "ddnsuser";
const char* ddnspass = "ddnspass" ;
//const char* ddnstoken = "ddnstoken" ;

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

const char* ddnsprovider = "dynu" ;
const int dnsInterval = 30000;     // DDNS check interval

const char* OTAname = "OTANAME";
const char* OTApasswd = "OTAPASSWD";

static const char ntpServerName[] = "th.pool.ntp.org";
const int timeZone = 7;     // Central European Time

unsigned long lastMillis;        // will store last time LED was updated
int LEDinterval = 1000;           // interval at which to blink (milliseconds)
const int intervalSlow = 1000;     // Wifi connected blink rate
const int intervalFast = 200;     // Wifi disconnected blink rate

const int ledPin =  LED_BUILTIN;// the number of the LED pin
int ledState;             // ledState used to set the LED
String myIP;

void Blink(){
  if(ledState){
    //digitalWrite(ledPin, LOW);
    analogWrite(ledPin, bright);
  }else{
    digitalWrite(ledPin, HIGH);
  }
  ledState=!ledState;
}

int wifiConnected() {
  return (WiFi.status() == WL_CONNECTED);
}

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, HIGH);
  ledState=HIGH;
  delay(100);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  Serial.print("Connecting to wifi ");
  unsigned long Millis = millis();
  while (!wifiConnected()) { // Wait for the Wi-Fi to connect
    if (millis() - Millis >= wifiTimeout) {
      Serial.println(" Timeout!");
      break;  
    }
    delay(intervalFast);
    Blink();
  }
  
  if(wifiConnected()) {
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
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
  EasyDDNS.client(ddnsdomain, ddnsuser, ddnspass); // Enter your DDNS Domain & user passwd
//  EasyDDNS.client(ddnsdomain, ddnstoken); // Enter your DDNS Domain & token

  // Get Notified when your IP changes
  EasyDDNS.onUpdate([&](const char* oldIP, const char* newIP){
    Serial.print("EasyDDNS - IP Change Detected: ");
    Serial.println(newIP);
  });

  lastMillis = millis();
  Serial.println();
  Serial.println("Running.");
}

void loop(){
    if (wifiConnected()) {
      LEDinterval=intervalSlow;
      ArduinoOTA.handle();
      EasyDDNS.update(dnsInterval);
    } else {
      LEDinterval=intervalFast;
    }

   unsigned long currentMillis = millis();
   if (currentMillis - lastMillis >= LEDinterval) {
        lastMillis = currentMillis;
        Blink();
    }

  delay(delayTime);
}
