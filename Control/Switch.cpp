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
}

bool Switch::Spin(unsigned long time)
{
	if(onWaiting)
	{
		if(time >= onTime)
		{
			onWaiting = false;
			digitalWrite(pin, HIGH);
		}
	}

	if(offWaiting)
	{
		if(time >= offTime)
		{
			offWaiting = false;
			digitalWrite(pin, LOW);
		}
	}
}




