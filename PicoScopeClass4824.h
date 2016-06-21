
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
	int16_t bufferA[65536];
	int16_t bufferB[65536];
	int16_t bufferC[65536];
	int16_t bufferD[65536];
	int16_t bufferE[65536];
	int16_t bufferF[65536];
	int16_t bufferG[65536];
	int16_t bufferH[65536];
	int16_t bufferCpA[65536];
	int16_t bufferCpB[65536];
	int16_t bufferCpC[65536];
	int16_t bufferCpD[65536];
	int16_t bufferCpE[65536];
	int16_t bufferCpF[65536];
	int16_t bufferCpG[65536];
	int16_t bufferCpH[65536];


	public:
		const PS4000A_CHANNEL channelA = PS4000A_CHANNEL_A;
		const PS4000A_CHANNEL channelB = PS4000A_CHANNEL_B;
		const PS4000A_CHANNEL channelC = PS4000A_CHANNEL_C;
		const PS4000A_CHANNEL channelD = PS4000A_CHANNEL_D;
		const PS4000A_CHANNEL channelE = PS4000A_CHANNEL_E;
		const PS4000A_CHANNEL channelF = PS4000A_CHANNEL_F;
		const PS4000A_CHANNEL channelG = PS4000A_CHANNEL_G;
		const PS4000A_CHANNEL channelH = PS4000A_CHANNEL_H;
		const PS4000A_RANGE range10mV = PS4000A_10MV;
		const PS4000A_RANGE range20mV = PS4000A_20MV;
		const PS4000A_RANGE range50mV = PS4000A_50MV;
		const PS4000A_RANGE range100mV = PS4000A_100MV;
		const PS4000A_RANGE range200mV = PS4000A_200MV;
		const PS4000A_RANGE range500mV = PS4000A_500MV;
		const PS4000A_RANGE range1V = PS4000A_1V;
		const PS4000A_RANGE range2V = PS4000A_2V;
		const PS4000A_RANGE range5V = PS4000A_5V;
		const PS4000A_RANGE range10V = PS4000A_10V;
		const PS4000A_RANGE range20V = PS4000A_20V;
		const PS4000A_RANGE range100V = PS4000A_100V;
		const PS4000A_RANGE range200V = PS4000A_200V;
		const PS4000A_COUPLING ac = PS4000A_AC;
	        const PS4000A_COUPLING dc = PS4000A_DC;
	PicoScope4000();
	~PicoScope4000();
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
	int streaming( const char *filename = "stream.csv", int numberOfSamples = 31*66553, int timeIntervalInNS = 5000);
};


