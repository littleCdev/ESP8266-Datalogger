#ifndef lcHttpRequest_H
  #define lcHttpRequest_H
  
  #include <ESP8266WiFi.h>
  
  class lcHttpRequest {
    public:
      void   init(String sRequest);
      String URN();
      String URI();
      String GET(char *sKey);
     
    private:
      String _sRequest;      
  };

#endif
