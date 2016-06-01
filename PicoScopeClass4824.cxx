
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
			buffer2[i] = buffer[i];
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
		if( isEnabled )
			isEnabled_int = TRUE;
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
		status = ps4000aSetDataBuffer( handle,
					       channel,
					       buffer,
					       bufferLth,
					       0,
	//				       PS4000A_RATIO_MODE_NONE);
					       PS4000A_RATIO_MODE_AVERAGE);
		printf("Statuscode %x\n", status);
	}


	int PicoScope4000::streaming(char *filename, int numberOfSamples, int timeIntervalInNS){
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
		fprintf( fileHandle, "adc\n");
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
					fprintf(fileHandle, "%i\n", buffer2[i]);
				}
			}
		}
		fclose( fileHandle );
		ps4000aStop( handle );
		printf("Statuscode %x\n", status);
	}




