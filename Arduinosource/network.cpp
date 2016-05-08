

#define NetworkConfigExt

#include <ESP8266WiFi.h>
extern "C" {
  #include "user_interface.h"
  #include <string.h>
}
#include "network.h"
#include "httpRequest.h"
#include "templates.h"
#include "lcWiFi.h"
#include "lcEeprom.h"

void lcNetwork::loadDefaultApName(){
  byte mac[6] = {0};
  WiFi.macAddress(mac);

  snprintf(sApName,100,"%s-%02x%02x%02x%02x%02x%02x",APDEFAULTNAME,mac[0],mac[1],mac[2],mac[3],mac[4],mac[5]);
}

bool lcNetwork::initNetwork(){
  /*
    typedef enum WiFiMode {
    WIFI_OFF = 0, WIFI_STA = 1, WIFI_AP = 2, WIFI_AP_STA = 3
    } WiFiMode_t;
  */
  apIP = APDEFAULTIP;
  lcEeprom::init();
  loadFromEerpom();
  setWiFiMode();

  if(bClientEnabled){
    WiFi.begin(sClientNetworkName, sClientNetworkPassword);  
  }else{
    WiFi.disconnect();
  }
  
  if(bApEnabled){
    WiFi.softAPConfig(apIP, apIP, APDEFAULTSUBNETMASK);
    WiFi.softAP(sApName, sApPassword);
  }

  return true;  
}

String lcNetwork::scanNetworks() {

  String sResponse = "";
  String sTmp = "";

  if(String(sClientNetworkName).length()>0){
    sResponse += sHtmlNetworkDisconnect;
    sResponse.replace("<!--SSID-->",sClientNetworkName);
  }
  
  // WiFi.scanNetworks will return the number of networks found
  int n = WiFi.scanNetworks();
  if (n == 0)
    Serial.println("no networks found");
  else{
    Serial.print(n);
    Serial.println(" networks found");
    for (int i = 0; i < (n>5?5:n); i++){
      Serial.println((WiFi.encryptionType(i) == ENC_TYPE_NONE) ? " " : "*");
      
      sTmp = sHtmlNetworkScan;

      sTmp.replace("<!--SSID-->",WiFi.SSID(i));
      sTmp.replace("<!--CONNECTED-->",(WiFi.SSID(i)==String(sClientNetworkName)?"connected":""));
      sResponse += sTmp;

      delay(25);
    }
  }
  return sResponse;
}

String lcNetwork::HTMLnetworkStatus(){
  String sRet = sHtmlNetworkOverView;
    sRet.replace("<!--APIP-->",String(WiFi.softAPIP()[0]) +"."+String(WiFi.softAPIP()[1]) +"."+String(WiFi.softAPIP()[2]) +"."+String(WiFi.softAPIP()[3]));
    sRet.replace("<!--APACTIVE-->",bApEnabled?"Active":"Disabled");
    sRet.replace("<!--APNAME-->",sApName);
    sRet.replace("<!--APPW-->",sApPassword);
    sRet.replace("<!--WIFIIP-->",String(WiFi.localIP()[0]) +"."+String(WiFi.localIP()[1]) +"."+String(WiFi.localIP()[2]) +"."+String(WiFi.localIP()[3]));
    sRet.replace("<!--WIFINAME-->",sClientNetworkName);
    sRet.replace("<!--WIFIPW-->",sClientNetworkPassword);
    switch(WiFi.status()){
      case WL_NO_SHIELD:        sRet.replace("<!--WIFISTATE-->","WL_NO_SHIELD");      break;
      case WL_IDLE_STATUS:      sRet.replace("<!--WIFISTATE-->","WL_IDLE_STATUS");    break;
      case WL_NO_SSID_AVAIL:    sRet.replace("<!--WIFISTATE-->","WL_NO_SSID_AVAIL");  break;
      case WL_SCAN_COMPLETED:   sRet.replace("<!--WIFISTATE-->","WL_SCAN_COMPLETED"); break;
      case WL_CONNECTED:        sRet.replace("<!--WIFISTATE-->","WL_CONNECTED");      break;
      case WL_CONNECTION_LOST:  sRet.replace("<!--WIFISTATE-->","WL_CONNECTION_LOST"); break;
      case WL_DISCONNECTED:     sRet.replace("<!--WIFISTATE-->","WL_DISCONNECTED");   break;
      case WL_CONNECT_FAILED:     sRet.replace("<!--WIFISTATE-->","WL_CONNECT_FAILED");   break;
    }

    byte mac[6] = {0};
    WiFi.macAddress(mac);
    char sMac[20] = {0};
    snprintf(sMac,100,"%02x:%02x:%02x:%02x:%02x:%02x",mac[0],mac[1],mac[2],mac[3],mac[4],mac[5]);
    sRet.replace("<!--MAC-->",String(sMac));

    return sRet;
}

String lcNetwork::HTMLApConfig(){
  String sRet = sHtmlNetworkApConfig;

  sRet.replace("<!--APNAME-->",sApName);
  sRet.replace("<!--APPW-->",sApPassword);
  sRet.replace("<!--APACTIVECHECKED-->",bApEnabled?"checked":"");
  sRet.replace("<!--APEA-->",bApEnabled?"dis":"en");

  return sRet;
}

void lcNetwork::manageNetworks(lcWiFiClient *oWiFi){
  
  String sHeader = sHtmlHttpHeader;
  sHeader.replace("<!--TYPE-->","text/html");
  
  String  sRet = sHtmlMenu;
          sRet += sHtmlSubMenuNetwork;
  String  sUri = oWiFi->Request.URI();
 
  if(sUri.startsWith("Network/connect/") && oWiFi->Request.GET((char *)"action").equals("connect")){
    oWiFi->Request.GET((char*)"ssid").toCharArray(sClientNetworkName, 100);
    oWiFi->Request.GET((char*)"pass").toCharArray(sClientNetworkPassword,100);;
    
    configureClient();

    sRet += HTMLnetworkStatus();
  }else if(sUri.startsWith("Network/connect/") && oWiFi->Request.GET((char *)"action").equals("disconnect")){
    memset(sClientNetworkName,'\0',100);
    memset(sClientNetworkPassword,'\0',100);
    bClientEnabled          = false;   
    saveToEeprom();
    WiFi.disconnect();
    sRet = scanNetworks();
  }else if(sUri.startsWith("Network/WiFi/")){
    // scan networks
    sRet += scanNetworks();
  }else if(sUri == "Network/wifi/disconnect/"){
    // disconnect from Wifi
    WiFi.disconnect();
  }else if(sUri == "Network/Accesspoint/" && oWiFi->Request.GET((char *)"action").equals("configure")){
    // configure accesspoint
    oWiFi->Request.GET((char *)"ssid").toCharArray(sApName,100);
    oWiFi->Request.GET((char *)"pw").toCharArray(sApPassword,100);
    configureAP();

    sRet += HTMLApConfig();
  }else if(sUri == "Network/Accesspoint/" && oWiFi->Request.GET((char *)"action").equals("ed")){
    // enable or disable accesspoint  
    String sState = oWiFi->Request.GET((char *)"state");
    if(sState == "enable"){
      bApEnabled = true;
      setWiFiMode();
      saveToEeprom();
    }else if(sState == "disable"){
      bApEnabled = false;
      setWiFiMode();
      saveToEeprom();
    }else {
      sRet += "<h4>invalid state?</h4>";
    }
    sRet += HTMLApConfig();              
  }else if(sUri == "Network/Reset/"){
    resetEeprom();  
    ESP.restart();
  }else if(sUri == "Network/Accesspoint/"){
    // statuspage
    sRet += HTMLApConfig();        
  }else{
    sRet += HTMLnetworkStatus();
  }

  sRet += sHtmlEnd;

  sHeader.replace("<!--LENGTH-->",String(sRet.length()));
  oWiFi->sendString(&sHeader);
  oWiFi->sendString(&sRet);
  oWiFi->stop();
}

void lcNetwork::setWiFiMode(){
  // WIFI_OFF = 0, WIFI_STA = 1, WIFI_AP = 2, WIFI_AP_STA = 3 
  //WiFi.disconnect();
        if(bClientEnabled && !bApEnabled){Serial.println("setting mode:WIFI_STA");WiFi.mode(WIFI_STA);}
 else if(!bClientEnabled && bApEnabled){Serial.println("setting mode:WIFI_AP");WiFi.mode(WIFI_AP);}
 else if(bClientEnabled && bApEnabled){Serial.println("setting mode:WIFI_AP_STA");WiFi.mode(WIFI_AP_STA);}
 else if(!bClientEnabled && !bApEnabled){WiFi.mode(WIFI_OFF);Serial.println("setting mode:WIFI_OFF");}

  delay(1000);
}

void lcNetwork::configureAP(){
  if(bApEnabled){

    //restart AP
    bApEnabled = false;
    setWiFiMode();
    bApEnabled = true;
    setWiFiMode();
    
    saveToEeprom();
    WiFi.softAPConfig(apIP, apIP, APDEFAULTSUBNETMASK);
    WiFi.softAP(sApName, sApPassword);
  }else{
    // just save
    saveToEeprom();
    setWiFiMode();
  }
}

bool lcNetwork::ClientActive(){
  return WiFi.status() == WL_CONNECTED;
}

bool lcNetwork::saveToEeprom(){
  Serial.println("saving config to eeprom");
  
  lcEeprom::write(lcEeprom::ClientNetworkName,sClientNetworkName);
  lcEeprom::write(lcEeprom::ClientNetworkPassword,sClientNetworkPassword);
  lcEeprom::write(lcEeprom::ClientEnabled,bClientEnabled);  

  lcEeprom::write(lcEeprom::ApName,sApName);
  lcEeprom::write(lcEeprom::ApPw,sApPassword);
  lcEeprom::write(lcEeprom::ApEnabled,bApEnabled);

  lcEeprom::write(lcEeprom::SavedBefore,true);
  

  return true;
}

void lcNetwork::dump(){
  Serial.println("sClientNetworkName ->"+String(sClientNetworkName));
  Serial.println("sClientNetworkPassword ->"+String(sClientNetworkPassword));
  Serial.println("bClientEnabled ->"+String(bClientEnabled));
  Serial.println("sApName ->"+String(sApName));
  Serial.println("sApPassword ->"+String(sApPassword));
  Serial.println("bApEnabled ->"+String(bApEnabled));
}

bool lcNetwork::loadFromEerpom(){
  if(lcEeprom::read(lcEeprom::SavedBefore)){
    Serial.println("loading from eeprom");
    lcEeprom::read(lcEeprom::ClientNetworkName,sClientNetworkName);
    lcEeprom::read(lcEeprom::ClientNetworkPassword,sClientNetworkPassword);
    bClientEnabled = lcEeprom::read(lcEeprom::ClientEnabled);

    lcEeprom::read(lcEeprom::ApName,sApName);
    lcEeprom::read(lcEeprom::ApPw,sApPassword);
    bApEnabled = lcEeprom::read(lcEeprom::ApEnabled);    

    // create defaultname 
    if(strlen(sApName) == 0){
      loadDefaultApName();
    }
  }else{
    resetEeprom();
  }

  return true;
}

/**
  clear EEPROM and save default password/name for accesspoint
*/
bool lcNetwork::resetEeprom(){
  Serial.println("earsing eeprom");
  
  for (int i = 0 ; i < EEPROM_MAX_SIZE; i++) 
    EEPROM.write(i, 0);

  strcpy(sClientNetworkName,"");
  strcpy(sClientNetworkPassword,"");
  bClientEnabled          = false;

  // TODO remove function maybe..
  loadDefaultApName();
  strcpy(sApPassword,APDEFAULTPW);
  bApEnabled              = true;
  
  saveToEeprom();

  return true;
}

bool lcNetwork::configureClient(){
  bClientEnabled = true;
  WiFi.disconnect();
  Serial.println("trying to connect");
  WiFi.begin(sClientNetworkName, sClientNetworkPassword);

  delay(1000);
  saveToEeprom();
  
  return true;
}

bool lcNetwork::ApActive(){
  return bApEnabled; 
}

