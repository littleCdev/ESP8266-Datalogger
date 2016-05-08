#define lcSensor_EXT

#include "lcSensor.h"

lcSensor *SENSOR;

OneWire _onewire;
DallasTemperature _oneWireSensors;


void DS18B20::init(){
	_onewire.init(2);
	_oneWireSensors = DallasTemperature(&_onewire);
}

float DS18B20::getValue(){
	float fRet = 0.0;

	_oneWireSensors.requestTemperatures();
	fRet = _oneWireSensors.getTempCByIndex(0);

	return fRet;
}

String DS18B20::getType(){
	return "DS18B20";
}

String DS18B20::getUnity(){
	char buff[10] = {0};
	sprintf(buff,"%cC",GRAD_C);

	return String(buff);
}