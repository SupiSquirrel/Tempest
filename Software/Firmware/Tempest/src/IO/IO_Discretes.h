/*
 * IO_Discretes.h
 *
 * Created: 13.04.2015 20:57:01
 *  Author: Squirrel
 */ 


#ifndef IO_DISCRETES_H_
#define IO_DISCRETES_H_

#define IO_DISCRETES_CHANNEL_TRIGGER  0

#define IO_DISCRETES_CHANNEL_SOLENOID 1


void IO_Discretes_Initialize(void);

bool IO_Discretes_GetInputIsSet(uint32_t channel);

void IO_Discretes_SetOutputActive(uint32_t channel);
void IO_Discretes_SetOutputInactive(uint32_t channel);

#endif /* IO_DISCRETES_H_ */