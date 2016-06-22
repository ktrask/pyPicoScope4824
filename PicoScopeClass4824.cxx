
#include <stdio.h>
#include <unistd.h>
#include <libps4000a-1.0/ps4000aApi.h>

#define TRUE  1
#define FALSE 0

#include "PicoScopeClass4824.h"






/* Callbackfunction to copy the received data and get it back to the object*/
static void outerCallbackFunction( 
	int16_t		handle,
	int32_t		noOfSamples,
	uint32_t	startIndex,
	int16_t		overflow,
	uint32_t	triggerAt,
	int16_t		triggered,
	int16_t		autoStop,
	void		*pParameter) 
{
	PicoScope4000 *pointer;
	pointer = static_cast<PicoScope4000 *>(pParameter);
	pointer->callbackFunction(handle, noOfSamples, startIndex, overflow, triggerAt, triggered, autoStop, pParameter);
}
void  PicoScope4000::callbackFunction( 
	int16_t		handle,
	int32_t		noOfSamples,
	uint32_t	startIndex,
	int16_t		overflow,
	uint32_t	triggerAt,
	int16_t		triggered,
	int16_t		autoStop,
	void		*pParameter) 
{
	startIndx = startIndex;
	samples = noOfSamples;
	//printf("Callback...\nstartIndex %i\nnoOfSamples %i\n",startIndx, samples);
	for( int i = startIndex; i < (startIndex + noOfSamples); i++) {
		bufferCpA[i] = bufferA[i];
		bufferCpB[i] = bufferB[i];
		bufferCpC[i] = bufferC[i];
		bufferCpD[i] = bufferD[i];
		bufferCpE[i] = bufferE[i];
		bufferCpF[i] = bufferF[i];
		bufferCpG[i] = bufferG[i];
		bufferCpH[i] = bufferH[i];
	}
	if( autoStop )
		isStreaming = FALSE;
	else
		isNewData = TRUE;
}

int PicoScope4000::setChannel(){
	status = setChannel( 
	                           PS4000A_CHANNEL_A, 
				   true, 
				   PS4000A_AC, 
				   PS4000A_50MV,
				   0);
	return(status);
}
int PicoScope4000::setChannel(
        PS4000A_CHANNEL         channel,
        bool                 isEnabled,
        PS4000A_COUPLING        type,
        PS4000A_RANGE           range,
        float                   analogOffset
        ) {
	int16_t isEnabled_int = FALSE;
	if( isEnabled ){
		isEnabled_int = TRUE;
		setDataBuffer( channel );
	}
	status = ps4000aSetChannel(handle, 
	                           channel, 
				   isEnabled_int, 
				   type, 
				   range,
				   analogOffset);
	printf("Statuscode %x\n", status);
	return(status);
}

int PicoScope4000::setTriggerConditions(){
	status = ps4000aSetTriggerChannelConditions( handle,
					    NULL,
			    		    0,
					    PS4000A_CLEAR);
	printf("Statuscode %x\n", status);
}

int PicoScope4000::getTimeBase(){
	float timeIntervalNanoseconds;
	int32_t maxSamples;
	status = ps4000aGetTimebase2( handle, 
	                     16,
			     1024,  //number of required samples
			     &timeIntervalNanoseconds,
			     &maxSamples,
			     0);
	printf("Statuscode %x\n", status);
	printf("timeIntervalNanoseconds: %f\n", timeIntervalNanoseconds);
	printf("maxSamples: %i\n", maxSamples);
}

int PicoScope4000::setDataBuffer(){
	status = setDataBuffer( PS4000A_CHANNEL_A );
	printf("Statuscode %x\n", status);
}
int PicoScope4000::setDataBuffer( PS4000A_CHANNEL channel){
	int16_t * buff = bufferA;
	switch( channel ) {
		case PS4000A_CHANNEL_A:
			buff = bufferA;
			break;
		case PS4000A_CHANNEL_B:
			buff = bufferB;
			break;
		case PS4000A_CHANNEL_C:
			buff = bufferC;
			break;
		case PS4000A_CHANNEL_D:
			buff = bufferD;
			break;
		case PS4000A_CHANNEL_E:
			buff = bufferE;
			break;
		case PS4000A_CHANNEL_F:
			buff = bufferF;
			break;
		case PS4000A_CHANNEL_G:
			buff = bufferG;
			break;
		case PS4000A_CHANNEL_H:
			buff = bufferH;
			break;
	}
	status = ps4000aSetDataBuffer( handle,
				       channel,
				       buff,
				       bufferLth,
				       0,
//				       PS4000A_RATIO_MODE_NONE);
				       PS4000A_RATIO_MODE_AVERAGE);
	printf("Statuscode %x\n", status);
}


int PicoScope4000::streaming(const char *filename, int numberOfSamples, int timeIntervalInNS){
	// Start Streaming
	isNewData = FALSE;
	isStreaming = TRUE;
	uint32_t sampleInterval = timeIntervalInNS / 10;
	status = ps4000aRunStreaming( handle,
				      &sampleInterval,
				      PS4000A_NS,
				      0, // no trigger
				      numberOfSamples*10,
				      TRUE,
				      10,
				      PS4000A_RATIO_MODE_AVERAGE,
				      bufferLth);
	printf("Statuscode %x\n", status);
	printf("sampleInterval %i\n", sampleInterval);

	// get Streaming Values
	status = ps4000aGetStreamingLatestValues( handle,
						  outerCallbackFunction,
						  this
						);
	printf("Statuscode %x\n", status);
	FILE * fileHandle = fopen( filename, "w" );
	fprintf( fileHandle, "#sampleInterval: %i\n", sampleInterval*10);
	fprintf( fileHandle, "adcA,\tadcB,\tadcC,\tadcD,\tadcE,\tadcF,\tadcG,\tadcH\n");
	while( isStreaming ) {

		usleep( 1e3 );
		status = ps4000aGetStreamingLatestValues( handle,
							  outerCallbackFunction,
							  this
							);
	//printf("Statuscode %x\n", status);
		if( isNewData ) {
			isNewData = FALSE;
			for( int i = startIndx; i < (startIndx + samples); i++) {
				//printf("%i\n", buffer2[i]);
				fprintf(fileHandle, "%i,\t%i,\t%i,\t%i,\t%i,\t%i,\t%i,\t%i,\t\n", bufferCpA[i]
							  , bufferCpB[i]
							  , bufferCpC[i]
							  , bufferCpD[i]
							  , bufferCpE[i]
							  , bufferCpF[i]
							  , bufferCpG[i]
							  , bufferCpH[i]
					);
			}
		}
	}
	fclose( fileHandle );
	ps4000aStop( handle );
	printf("Statuscode %x\n", status);
}





PicoScope4000::PicoScope4000(){
	bufferLth = 65536;
	//UNIT unit;
	status = ps4000aOpenUnit(&handle, NULL); // open the first found unit
	printf("Statuscode %x\n", status);

	status = ps4000aChangePowerSource(handle, PICO_POWER_SUPPLY_NOT_CONNECTED); //for usb2 connections
	printf("Statuscode %x\n", status);
	isNewData = FALSE;
	isStreaming = TRUE;

	for( int i = 0; i < 65536; i++) {
		bufferCpA[i] = 0;
		bufferCpB[i] = 0;
		bufferCpC[i] = 0;
		bufferCpD[i] = 0;
		bufferCpE[i] = 0;
		bufferCpF[i] = 0;
		bufferCpG[i] = 0;
		bufferCpH[i] = 0;
		bufferA[i] = 0;
		bufferB[i] = 0;
		bufferC[i] = 0;
		bufferD[i] = 0;
		bufferE[i] = 0;
		bufferF[i] = 0;
		bufferG[i] = 0;
		bufferH[i] = 0;
	}
}
PicoScope4000::~PicoScope4000(){
	ps4000aCloseUnit(handle);
}
