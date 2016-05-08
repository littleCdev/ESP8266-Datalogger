#ifndef lcSensor_H
	#define lcSensor_H
	
	#include <ESP8266WiFi.h>
	#include "Sensors/OneWire.h"
	#include "Sensors/DS18B20/DallasTemperature.h"
	#include "Sensors/DHT11/DHT.h"

	#define GRAD_C 	176
	
	extern "C" {
		#include "user_interface.h"
	};

	class lcSensor
	{
		public:
			virtual void init() = 0;
			virtual float getValue() = 0;
			virtual String getUnity();
			virtual String getType();
		private:
	};

	class DS18B20: public lcSensor
	{
	public:
		float getValue();
		String getUnity();
		String getType();
		void init();
	};

	#ifndef lcSensor_EXT
		#define lcSensor_EXT
		extern lcSensor *SENSOR;
	#endif
#endif