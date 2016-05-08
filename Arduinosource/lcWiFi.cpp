#include "lcWiFi.h"

lcWiFiClient::lcWiFiClient(WiFiClient *client){
  _client = client; 
  if(!_client)
    return;
    
  unsigned long ultimeout = millis() + 250;
  // Wait until the client sends some data
  while(!client->available() && millis() < ultimeout){
    delay(1);
  }

  if(millis()>ultimeout){
    Serial.println("client timeout");
    _sRequest = "";
    return;
  }
  
  _sRequest = _client->readStringUntil('\r');
  Request.init(_sRequest);
  
  client->flush();
}
bool lcWiFiClient::clientOK(){
  if (!_client || _sRequest == "")
    return false;
   return true;
}

void lcWiFiClient::sendCss(){
  Serial.println("seding css");
  
  String sHeader = sHtmlHttpHeader;
  sHeader.replace("<!--TYPE-->","text/css");
  sHeader.replace("<!--LENGTH-->",String(CSS.length()));

  sendString(&sHeader);
  sendString(&CSS);
  stop();
}

void lcWiFiClient::send404(){
  String sHeader = "";
  
  sHeader  = "HTTP/1.1 404 Not found\r\n";
  sHeader += "Content-Length: ";
  sHeader += sHtml404.length();
  sHeader += "\r\n";
  sHeader += "Content-Type: text/html\r\n";
  sHeader += "Connection: close\r\n";
  sHeader += "\r\n";
  
  sendString(&sHeader);
  sendString(&sHtml404);
  stop();
}
void lcWiFiClient::sendString(const String *sString){
   for (unsigned i = 0; i < sString->length(); i += 1024) {
    Serial.println("print:"+String(_client->print(sString->substring(i, i+1024))));
    delay(100);
  }
}

void lcWiFiClient::sendString(String *sString){
  for (unsigned i = 0; i < sString->length(); i += 1024) {
    Serial.println("print1:"+String(_client->print(sString->substring(i, i+1024))));
    delay(100);
  }
}

void lcWiFiClient::stop(){
  return;
  Serial.println("trying to stop");
  if(clientOK()){
    Serial.println("clientOK == True, stopping client");
    _client->stop();
  }
  Serial.println("returning client.stop()");
}

