/*
 * Switch.cpp
 *
 *  Created on: 24 Aug 2014
 *      Author: ensab
 */

#include "Control.h"

Switch::Switch(int p)
{
	pin = p;
	pinMode(pin, OUTPUT);
	digitalWrite(pin, LOW);
	onWaiting = false;
	offWaiting = false;
	iAmOn = false;
	analogue = false;
	value = 0;
}

bool Switch::Spin(time_t time)
{
	if(onWaiting)
	{
		if(time >= onTime)
		{
			onWaiting = false;
			iAmOn = true;
		}
	}

	if(offWaiting)
	{
		if(time >= offTime)
		{
			offWaiting = false;
			iAmOn = false;
		}
	}

	if(iAmOn)
	{
		if(analogue)
			analogWrite(pin, value);
		else
			digitalWrite(pin, HIGH);
	} else
	{
		digitalWrite(pin, LOW);
	}
}




