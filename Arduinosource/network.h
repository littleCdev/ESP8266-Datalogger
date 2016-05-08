#ifndef lcNetwork_H
  #define lcNetwork_H
  
  #include <ESP8266WiFi.h>
  #include "httpRequest.h"
  #include "lcWiFi.h"
  #include "lcEeprom.h"

  #define APDEFAULTNAME       "LC-ESP8266"    // mac will be added to the end
  #define APDEFAULTPW         "12345678"
  #define APDEFAULTIP         IPAddress(192, 168, 0, 1)
  #define APDEFAULTSUBNETMASK IPAddress(255, 255, 255, 0)

  class lcNetwork{
    public:
      IPAddress apIP;

      bool initNetwork();
      
      void manageNetworks(lcWiFiClient *oWiFi);

      
      bool ClientActive();
      bool ApActive();
      
      void configureAP();
      bool WiFiConnected();
      bool configureClient();
      bool resetEeprom();

      void dump();

    private:
      char      sClientNetworkName[100]     = "";
      char      sClientNetworkPassword[100] = "";
      bool      bClientEnabled              = true;
      char      sApName[100]                = "";
      char      sApPassword[100]            = APDEFAULTPW;
      bool      bApEnabled                  = true;

      void loadDefaultApName();
      void setWiFiMode();
      String scanNetworks();
      
      String HTMLnetworkStatus();
      String HTMLApConfig();
      
      bool saveToEeprom();
      bool loadFromEerpom();
  };
#endif
