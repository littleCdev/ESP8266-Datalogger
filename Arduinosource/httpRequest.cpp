#include "httpRequest.h"

void lcHttpRequest::init(String sRequest){
  _sRequest = sRequest;
}

String lcHttpRequest::URI() {
  // Syntax is e.g. GET /?pin=MOTOR1STOP HTTP/1.1
  String sGetstart = "GET ";
  int iStart = -1;
  int iEndSpace = -1;
  int iEndQuestionmark = -1;

  iStart = _sRequest.indexOf(sGetstart) + sGetstart.length() + 1;
  iEndSpace = _sRequest.indexOf(" ", iStart);
  iEndQuestionmark = _sRequest.indexOf("?", iStart); 

  int iEnd = iEndSpace > iEndQuestionmark && iEndQuestionmark > -1 ? iEndQuestionmark:iEndSpace;

  return _sRequest.substring(iStart, iEnd);
}


String lcHttpRequest::URN(){
  // Syntax is e.g. GET /?pin=MOTOR1STOP HTTP/1.1
  int iEndSpace = -1;
  int iEndQuestionmark = -1;

  iEndQuestionmark = _sRequest.indexOf("?"); 

  if(iEndQuestionmark == -1){
    return String("");
  }
  
  iEndSpace = _sRequest.indexOf(" ", iEndQuestionmark);
   
  return _sRequest.substring(iEndQuestionmark+1,iEndSpace);
}

String lcHttpRequest::GET(char *sKey) {
  char sURN[255] = {0};
  URN().toCharArray(sURN, 255);

  char *ptr = strtok(sURN, "=");
  while (ptr != NULL) {
    if (strcmp(sKey, ptr) == 0) {
      return String(strtok(NULL, "&"));
    }
    ptr = strtok(NULL, "&");
    ptr = strtok(NULL, "=");
  }

  return String("");
}
