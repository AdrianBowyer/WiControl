
#include "Control.h"

char data[DATA_LENGTH];
int8_t dataPointer;

byte pins[SWITCHES] = OUT_PINS;

Switch* switches[SWITCHES];

long resetTime;

Communicator* communicator = 0;
CommandBuffer* serialInput;
CommandBuffer* wirelessInput;


void blink()
{
	switches[SWITCHES-1]->On(0.0);
	switches[SWITCHES-1]->Off(0.1);
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

float SetOutput(int pin, int val)
{
	digitalWrite(pin, val);
}


void setup()
{
	  wdt_disable();
	  resetTime = millis();
	  Serial.begin(BAUD_RATE);
	  Serial.print((int)MY_ADDRESS);
	  Serial.println(" starting");
	  pinMode(LEDOUTPUT, OUTPUT);
	  digitalWrite(LEDOUTPUT, LOW);
	  pinMode(TEMP_SENSE_PIN, INPUT);

	  for(byte s = 0; s < SWITCHES; s++)
		  switches[s] = new Switch(pins[s]);

#ifdef USE_LEDS
	  pinMode(RED_PWM, OUTPUT);
	  pinMode(GREEN_PWM, OUTPUT);
	  pinMode(BLUE_PWM, OUTPUT);
	  analogWrite(RED_PWM, 0);
	  analogWrite(GREEN_PWM, 0);
	  analogWrite(BLUE_PWM, 0);
#endif

	  communicator = new Communicator(MY_ADDRESS);
	  communicator->SetDebug(false);
	  dataPointer = 0;
	  serialInput = new CommandBuffer();
	  wirelessInput = new CommandBuffer();
#ifdef WATCHDOG
	  wdt_enable(WDTO_8S);
	  wdt_reset();
#endif
}

void Message(char* m)
{
	Serial.print(m);
}

void Message(int i)
{
	Serial.print(i);
}

void Interpret(CommandBuffer* cb, int address)
{
	float t;
	int sendAddress;

	if(address == MY_ADDRESS)
	{
		Message("Talking to myself!\n");
		return;
	}

	if(cb->Seen('C')) // Command
	{
		switch(cb->GetIValue())
		{
		case 0: // Some sort of sleep or reset

			break;

			// Turn an output on or off

		case 1:
			if(cb->Seen('H'))
			{
				int value = cb->GetIValue();
				if(cb->Seen('P'))
				{
					int s = cb->GetIValue();
					float delay = 0.0;
					if(cb->Seen('D'))
						delay = cb->GetFValue();
					if(value)
						switches[s]->On(delay);
					else
						switches[s]->Off(delay);
				}
			}
			break;

			// Print the temperature, and return it

		case 2:
			t = GetTemperature();
			Message("Temp: ");
			Message(t);
			Message("\n");
			if(address >= 0)
			{
				snprintf(data, DATA_LENGTH, "R2 T%d\n", 10*(int)t);
				communicator->Send(address, data);
			}
			break;

			// Send a command to another device

		case 3:
			if(cb->Seen('A'))
			{
				sendAddress = cb->GetIValue();
				if(cb->Seen('S'))
					communicator->Send(sendAddress, cb->GetString());
			}
			break;

			// Say who I am

		case 4:
			snprintf(data, DATA_LENGTH, "%s, ver: %s, date: %s, addr: %d\n",
					NAME, VERSION, DATE, MY_ADDRESS);
			Message("\n");
			Message(data);
			Message("\n");
			if(address >= 0)
			{
				snprintf(data, DATA_LENGTH, "R4 S%s, ver: %s, date: %s, addr: %d\n",
						NAME, VERSION, DATE, MY_ADDRESS);
				communicator->Send(address, data);
			}
			break;

			// Low level write to a pin for test/debug

		case 5:
			if(cb->Seen('P'))
			{
				int pin = cb->GetIValue();
				if(cb->Seen('H'))
				{
					pinMode(pin, OUTPUT);
					digitalWrite(pin,cb->GetIValue());
				}
			}
			break;

			// Set LED rgb values
#ifdef USE_LEDS
		case 6:
		{
			float c;
			if(cb->Seen('r'))
			{
				c = cb->GetFValue();
				analogWrite(RED_PWM, (unsigned char)(c*255.0));
			}
			if(cb->Seen('g'))
			{
				c = cb->GetFValue();
				analogWrite(GREEN_PWM, (unsigned char)(c*255.0));
			}
			if(cb->Seen('b'))
			{
				c = cb->GetFValue();
				analogWrite(BLUE_PWM, (unsigned char)(c*255.0));
			}

		}
		break;
#endif

		default:
			Message("Unknown command: ");
			Message(cb->Buffer());
			Message("\n");
		}
	}

	if(cb->Seen('R')) // Reply
	{
		switch(cb->GetIValue())
		{
		// Reply from a request for a temperature

		case 2:
			if(cb->Seen('T'))
			{
				Message("Temp returned from ");
				Message(address);
				Message(" was ");
				Message(0.1*(float)cb->GetIValue());
				Message("\n");
			}
			break;

		case 4:
			if(cb->Seen('S'))
			{
				Message("Identifier from: ");
				Message(address);
				Message(" was ");
				Message(cb->GetString());
				Message("\n");
			}
			break;

		default:
			Message("Unknown reply: ");
			Message(cb->Buffer());
			Message("\n");
		}
	}
}


void loop()
{

	byte address;
	unsigned long time = millis();

#ifdef WATCHDOG
	if(time - resetTime < ((long)RESET_SECONDS - (long)8)*(long)1000)
		wdt_reset();
#endif

	for(byte s = 0; s < SWITCHES; s++)
		switches[s]->Spin(time);

	char* input = communicator->Receive(address);
	if(input)
	{
//		Serial.print("From ");
//		Serial.print(address);
//		Serial.print(": ");
//		Serial.println(input);
		wirelessInput->Fill(input);
		communicator->FreeReadData();
		Interpret(wirelessInput, address);
		// If something has talked to us, we are working OK; reset the reset timer
		resetTime = time;
	}

	if(!Serial.available())
		return;

	char b = (char)Serial.read();
	if(serialInput->Put(b))
		Interpret(serialInput, -1);

	//Serial.print(b);
}
