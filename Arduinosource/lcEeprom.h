#ifndef lcEeprom_H
	#define lcEeprom_H

	#include <EEPROM.h>

	#define EEPROM_MAX_SIZE 4096

	class lcEeprom
	{
	public:
		// just calls .begin
		static void init();

		static void write(int iAddress, bool buffer);
		static void write(int iAddress, char *buffer);
		static void write(int iAddress, char *buffer, int iSize);
		
		static bool read(int iAddress);
		static bool read(int iAddress,bool *buffer);
		static void read(int iAddress,char *buffer);
		static void read(int iAddress,char *buffer,int iSize);

		static int ClientNetworkName    ;
		static int ClientNetworkPassword;
		static int ClientEnabled        ;
		static int ApName               ;
		static int ApPw                 ;
		static int ApEnabled            ;
		static int SavedBefore          ; // not the first time, so we saved something in the eeprom	



		static int ThingSpeakKey		; // string [50]
		static int ThingSpeakField		; // string [1]
		static int ThingSpeakIntervall	; // string [3]
		static int DeepSleepActive		; // int
	};

#endif