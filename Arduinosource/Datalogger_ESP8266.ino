#include <ESP8266WiFi.h>
#include <DNSServer.h>
#include <ESP8266HTTPClient.h>

#include "network.h"
#include "templates.h"
#include "lcWiFi.h"
#include "lcSensor.h"
#include "lcEeprom.h"
/// http://arduino.esp8266.com/versions/1.6.5-1160-gef26c5f/doc/reference.html#file-system
// SPIFFS https://github.com/esp8266/arduino-esp8266fs-plugin/releases/
//  ^-- don't use it, it's sloooow

#define   RESETTIME             15  // time the button needs to be pressed until systemreset
#define   DEEPSLEEPWAKEUPTIME   3 // time user is able to wake me up after reset

bool      bDeepSleep = false;
char      sThingSpeakKey[50]     = "";
int       iThingSpeakField       = 1;
int       iThingSpeakIntervall   = 1;


lcNetwork oNetwork;     // my "networkmanager"
WiFiServer server(80);  // webserver on port 80

void setup()
{
  lcEeprom::init();
  
  // start serial
  Serial.begin(9600);
  Serial.println("Started\r\n");
  delay(1);

  // prepare GPIO2
  pinMode(2, OUTPUT);
  digitalWrite(2, 0);
  
  // check if i should disable deepsleep
  bDeepSleep = lcEeprom::read(lcEeprom::DeepSleepActive);
  if(bDeepSleep){
    Serial.println("bDeepSleep is active, waiting for switch");
		for(int i=0;i<DEEPSLEEPWAKEUPTIME*10;i++){
			if(digitalRead(0)==0){
				lcEeprom::write(lcEeprom::DeepSleepActive,false);
				bDeepSleep = false;
				Serial.println("bDeepSleep deactivated");
        i=30;
			}else
        delay(100);
		}
	}

  // if pin is still low for x seconds, reset network
  Serial.println("factoryreset?");
  delay(500);
  int iReset = 0;
  while(!digitalRead(0)){
    if(iReset == RESETTIME*10){
      oNetwork.resetEeprom();
      ESP.restart();
    }

    iReset++;
    delay(100);
  }

  
  server.begin();
  SENSOR = new DS18B20();

  SENSOR->init();
  oNetwork.initNetwork();
  oNetwork.dump();

  lcEeprom::read(lcEeprom::ThingSpeakKey,sThingSpeakKey,50);

  char sTmp[3] = {0};
  lcEeprom::read(lcEeprom::ThingSpeakField,sTmp,1);
  iThingSpeakField = String(sTmp).toInt();

  memset(sTmp,0,3);
  lcEeprom::read(lcEeprom::ThingSpeakIntervall,sTmp,3);
  iThingSpeakIntervall = String(sTmp).toInt();


  Serial.println("Field: "+String(iThingSpeakField));
  Serial.println("intervall: "+String(iThingSpeakIntervall));
  Serial.println(String("key:")+String(sThingSpeakKey));
}

void loop()
{

  if(bDeepSleep){
    Serial.println("bDeepSleep is active");
    HTTPClient http;
    String sUrl = String("http://api.thingspeak.com/update?key=")+String(sThingSpeakKey)+String("&field")+String(iThingSpeakField)+String("=")+String(SENSOR->getValue());
    Serial.println(sUrl);
    http.begin(sUrl); //HTTP
//    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    int httpCode = http.GET();
    if(httpCode) {
      Serial.printf("[HTTP] GET... code: %d\n", httpCode);
      // file found at server
      if(httpCode == 200) {
        String payload = http.getString();
        Serial.println(payload);
      }
    }

    Serial.println("going Deepsleep");
    Serial.flush();
    ESP.deepSleep(1000000*60*iThingSpeakIntervall, WAKE_RF_DEFAULT);

    return;
  }

  // Check if a client has connected
  WiFiClient client;
  
  if(!server.hasClient()){
    return;
  }
 
  client = server.available();
  lcWiFiClient WiFiClient(&client);       

  if(!WiFiClient.clientOK()){
    WiFiClient.stop();
    return;
  }
    
  String sCmd = WiFiClient.Request.GET((char *)"cmd");
  String sUrl = WiFiClient.Request.URI();
  Serial.println("got: url -> " + sUrl);
 
  if(sUrl == "css/skeleton.css"){
    WiFiClient.sendCss();

    return;
  } else if(sUrl == "favicon.ico"){
    WiFiClient.send404();
    return;
  }else if(sUrl.startsWith("Network/")){
    oNetwork.manageNetworks(&WiFiClient);
  }else if(sUrl.startsWith("Sensor/")){
    String sHeader = sHtmlHttpHeader;
    String sRet    = sHtmlMenu;
    sRet            += sHtmlSensorOverView;
    sRet            += sHtmlEnd;

    sRet.replace("<!--UNITY-->",SENSOR->getUnity());
    sRet.replace("<!--TYPE-->",SENSOR->getType());
    sRet.replace("<!--VALUE-->",String(SENSOR->getValue()));


    if(sUrl.endsWith("/Set")){
      Serial.println(WiFiClient.Request.GET((char *)"key"));
      Serial.println(WiFiClient.Request.GET((char *)"field"));
      Serial.println(WiFiClient.Request.GET((char *)"intervall"));

      String(WiFiClient.Request.GET((char *)"key")).toCharArray(sThingSpeakKey, 50); 
      iThingSpeakField = String(WiFiClient.Request.GET((char *)"field")).toInt();
      iThingSpeakIntervall = String(WiFiClient.Request.GET((char *)"intervall")).toInt();

      lcEeprom::write(lcEeprom::ThingSpeakKey,sThingSpeakKey,50);
      
      char sTmp[3] = {0};
      WiFiClient.Request.GET((char *)"field").toCharArray(sTmp, 3);
      lcEeprom::write(lcEeprom::ThingSpeakField,sTmp,1);

      memset(sTmp,0,3);
      WiFiClient.Request.GET((char *)"intervall").toCharArray(sTmp, 3);
      lcEeprom::write(lcEeprom::ThingSpeakIntervall,sTmp,3);
    }

    sRet.replace("<!--APIKEY-->",String(sThingSpeakKey));
    sRet.replace("<!--APIFIELD-->",String(iThingSpeakField));
    sRet.replace("<!--APIINTERVALL-->",String(iThingSpeakIntervall));


    WiFiClient.sendString(&sHeader);
    WiFiClient.sendString(&sRet);
  }else if(sUrl.startsWith("Deepsleep/")){
    lcEeprom::write(lcEeprom::DeepSleepActive,true);
    Serial.println(lcEeprom::read(lcEeprom::DeepSleepActive));
    bDeepSleep = true;
  }else if(sUrl.startsWith("Test/")){
    String sHeader  = sHtmlHttpHeader;
    String sRet     = sHtmlMenu;

    sRet            += "OK";
    sRet            += sHtmlEnd;

    sHeader.replace("<!--TYPE-->","text/html");
    sHeader.replace("<!--LENGTH-->",String(sRet.length()));

    WiFiClient.sendString(&sHeader);
    WiFiClient.sendString(&sRet);

  }else{
    String sHeader  = sHtmlHttpHeader;
    String sRet     = sHtmlMenu;
    sRet            += sHtmlEnd;

    sHeader.replace("<!--TYPE-->","text/html");
    sHeader.replace("<!--LENGTH-->",String(sRet.length()));

    WiFiClient.sendString(&sHeader);
    WiFiClient.sendString(&sRet);
  }
}


