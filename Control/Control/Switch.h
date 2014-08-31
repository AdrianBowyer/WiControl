/*
 * Switch.h
 *
 *  Created on: 24 Aug 2014
 *      Author: ensab
 */

#ifndef SWITCH_H_
#define SWITCH_H_

class Switch
{
public:
	Switch(int p);
	void On(float delay);
	void Off(float delay);
	bool Spin(unsigned long time);
	int Pin();

private:
	int pin;
	bool onWaiting;
	bool offWaiting;
	unsigned long onTime;
	unsigned long offTime;
};

inline void Switch::On(float delay)
{
	if(!onWaiting)
	{
		onWaiting = true;
		onTime = millis() + (unsigned long)(delay*1000.0);
	}
}

inline void Switch::Off(float delay)
{
	if(!offWaiting)
	{
		offWaiting = true;
		offTime = millis() + (unsigned long)(delay*1000.0);
	}
}

inline int Switch::Pin()
{
	return pin;
}


#endif /* SWITCH_H_ */