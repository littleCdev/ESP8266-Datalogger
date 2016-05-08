#include "lcEeprom.h"

// WiFi Name / Password / Enabled
int lcEeprom::ClientNetworkName 	= 0;
int lcEeprom::ClientNetworkPassword = 100;
int lcEeprom::ClientEnabled 		= 201;

// Accesspoint Name / Password / Enabled
int lcEeprom::ApName 				= 300;
int lcEeprom::ApPw 					= 400;
int lcEeprom::ApEnabled 			= 501;

// If the config was ever saved bevore~
int lcEeprom::SavedBefore 			= 502;

// thingspeak config
int lcEeprom::ThingSpeakKey			= 503; // string [50]
int lcEeprom::ThingSpeakField		= 553; // string [1]
int lcEeprom::ThingSpeakIntervall	= 554; // string [3]

int lcEeprom::DeepSleepActive		= 557;



// Dummy
//int lcEeprom:: = ;

void lcEeprom::init(){
	EEPROM.begin(EEPROM_MAX_SIZE); // 4096 is the max
}

bool lcEeprom::read(int iAddress){  
	return EEPROM.read(iAddress) ? true:false;
}

bool lcEeprom::read(int iAddress,bool *buffer){
	buffer[0] = EEPROM.read(iAddress)==1 ? true:false;

	return buffer[0];
}

void lcEeprom::read(int iAddress,char *buffer){
	read(iAddress,buffer,100);
}

void lcEeprom::read(int iAddress,char *buffer,int iSize){
	int i = 0;
	for(i=0;i<iSize;i++){
      buffer[i]   = EEPROM.read(iAddress+i);
	  if(buffer[i] == 255)
	  	buffer[i] = 0;
	}
}

void lcEeprom::write(int iAddress, bool buffer){
	EEPROM.write(iAddress,buffer?1:0);
	EEPROM.commit();
}

void lcEeprom::write(int iAddress, char *buffer){
	lcEeprom::write(iAddress,buffer,100);
	EEPROM.commit();
}

void lcEeprom::write(int iAddress, char *buffer, int iSize){
	int i = 0;
	for (int i = 0; i < iSize; ++i)
		EEPROM.write(iAddress+i,buffer[i]);  

	EEPROM.commit();
}