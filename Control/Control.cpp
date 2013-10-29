// Do not remove the include below
#include "Control.h"

#define MY_ADDRESS 4
#define THEIR_ADDRESS 5

char data[60];
int8_t dataPointer;

Communicator* communicator = 0;

volatile bool blinkQueued = false;

void blink()
{
	blinkQueued = true;
}

void blinkInLoop()
{
	if(!blinkQueued)
		return;
	digitalWrite(LEDOUTPUT, !digitalRead(LEDOUTPUT));
	delay(100);
	digitalWrite(LEDOUTPUT, !digitalRead(LEDOUTPUT));
	delay(100);
	blinkQueued = false;
}

void IncommingInterrupt()
{
	blink();
	if(communicator)
		communicator->Interrupt();
}

float GetTemperature()
{
  float r = (float)analogRead(TEMP_SENSE_PIN);
  return ABS_ZERO + THERMISTOR_BETA/log( (r*THERMISTOR_SERIES_R/(AD_RANGE - r))/
		  ( THERMISTOR_25_R*exp(-THERMISTOR_BETA/(25.0 - ABS_ZERO)) ) );
}

float SetHeater(int val)
{
	digitalWrite(HEAT_ON_PIN, val);
}


void setup()
{
	  Serial.begin(38400);
	  Serial.println("start");
	  pinMode(LEDOUTPUT, OUTPUT);
	  digitalWrite(LEDOUTPUT, LOW);
	  pinMode(TEMP_SENSE_PIN, INPUT);
	  pinMode(HEAT_ON_PIN, OUTPUT);
	  digitalWrite(HEAT_ON_PIN, 0);
	  communicator = new Communicator(MY_ADDRESS);
	  communicator->SetDebug(true);
	  dataPointer = 0;
	  blinkQueued = false;
}

void talk()
{
	byte address;

	char* input = communicator->Receive(address);
	if(input)
	{
		Serial.print("From ");
		Serial.print(address);
		Serial.print(": ");
		Serial.print(input);
		communicator->FreeReadData();
	}

	if(!Serial.available())
		return;

	data[dataPointer] = Serial.read();
	if(data[dataPointer] == '\n')
	{
		dataPointer++;
		data[dataPointer] = 0;
		communicator->Send(THEIR_ADDRESS, data);
		dataPointer = 0;
		return;
	}
	dataPointer++;
}


void loop()
{
	blinkInLoop();
	talk();
}
