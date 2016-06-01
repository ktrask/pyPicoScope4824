
#include <stdio.h>
#include <unistd.h>
#include <libps4000a-1.0/ps4000aApi.h>

#define TRUE  1
#define FALSE 0

class PicoScope4000 {


	int isNewData;
	int isStreaming;
	uint32_t startIndx;
	int32_t samples;
	PICO_STATUS status;
	int32_t bufferLth;

	int16_t handle;
	int16_t buffer[65536];
	int16_t buffer2[65536];


	public:
		PS4000A_CHANNEL channelA;
		PS4000A_CHANNEL channelB;
		PS4000A_CHANNEL channelC;
		PS4000A_CHANNEL channelD;
		PS4000A_CHANNEL channelE;
		PS4000A_CHANNEL channelF;
		PS4000A_CHANNEL channelG;
		PS4000A_CHANNEL channelH;
		PS4000A_RANGE range10mV;
		PS4000A_RANGE range20mV;
		PS4000A_RANGE range50mV;
		PS4000A_RANGE range100mV;
		PS4000A_RANGE range200mV;
		PS4000A_RANGE range500mV;
		PS4000A_RANGE range1V;
		PS4000A_RANGE range2V;
		PS4000A_RANGE range5V;
		PS4000A_RANGE range10V;
		PS4000A_RANGE range20V;
		PS4000A_RANGE range100V;
		PS4000A_RANGE range200V;
		PS4000A_COUPLING ac;
	        PS4000A_COUPLING dc;
	PicoScope4000(){
		bufferLth = 65536;
		//UNIT unit;
		status = ps4000aOpenUnit(&handle, NULL); // open the first found unit
		printf("Statuscode %x\n", status);
	
		status = ps4000aChangePowerSource(handle, PICO_POWER_SUPPLY_NOT_CONNECTED); //for usb2 connections
		printf("Statuscode %x\n", status);
		isNewData = FALSE;
		isStreaming = TRUE;
	
		for( int i = 0; i < 65536; i++) {
			buffer2[i] = 0;
			buffer[i] = 0;
		}
		channelA = PS4000A_CHANNEL_A;
		channelB = PS4000A_CHANNEL_B;
		channelC = PS4000A_CHANNEL_C;
		channelD = PS4000A_CHANNEL_D;
		channelE = PS4000A_CHANNEL_E;
		channelF = PS4000A_CHANNEL_F;
		channelG = PS4000A_CHANNEL_G;
		channelH = PS4000A_CHANNEL_H;
		range10mV = PS4000A_10MV;
		range20mV = PS4000A_20MV;
		range50mV = PS4000A_50MV;
		range100mV= PS4000A_100MV;
		range200mV= PS4000A_200MV;
		range500mV= PS4000A_500MV;
		range1V   = PS4000A_1V;
		range2V   = PS4000A_2V;
		range5V   = PS4000A_5V;
		range10V  = PS4000A_10V;
		range20V  = PS4000A_50V;
		range100V = PS4000A_100V;
		range200V = PS4000A_200V;
		ac = PS4000A_AC;
		dc = PS4000A_DC;
	}
	~PicoScope4000(){
		ps4000aCloseUnit(handle);
	}
	void  callbackFunction( 
		int16_t		handle,
		int32_t		noOfSamples,
		uint32_t	startIndex,
		int16_t		overflow,
		uint32_t	triggerAt,
		int16_t		triggered,
		int16_t		autoStop,
		void		*pParameter); 
	int setChannel();
	int setChannel(
		PS4000A_CHANNEL		channel,
		bool			isEnabled,
		PS4000A_COUPLING	type,
		PS4000A_RANGE		range,
		float			analogOffset = 0
		);
	int setTriggerConditions();
	int getTimeBase();
	int setDataBuffer();
	int setDataBuffer( PS4000A_CHANNEL channel);
	int streaming( char *filename = "stream.csv", int numberOfSamples = 31*66553, int timeIntervalInNS = 5000);
};


