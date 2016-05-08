#ifndef lcWiFiClient_H
  #define lcWiFiClient_H
  
  #include <ESP8266WiFi.h>
  #include "templates.h"
  #include "httpRequest.h"
  
  class lcWiFiClient{
    private:
      WiFiClient *_client;
      String _sRequest;
      
    public:
      lcHttpRequest Request;

      lcWiFiClient(WiFiClient *client);
      void send404();
      void sendString(String *sText);
      void sendString(const String *sText);
      void sendCss();
      bool clientOK();
      void stop();
  };

#endif
