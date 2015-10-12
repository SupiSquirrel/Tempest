/*
 * TRG_Trigger.h
 *
 * Created: 13.04.2015 21:49:50
 *  Author: Squirrel
 */ 


#ifndef TRG_TRIGGER_H_
#define TRG_TRIGGER_H_


#define TRG_TRIGGER_PULLED 1
#define TRG_TRIGGER_RELEASED 2

#define TRG_TRIGGER_DEBOUNCE_TICKS (IO_SYSTICKTIMER_TICK_FREQUENCY/1000 * 10) // 10ms


#define TRG_TRIGGER_EDGE_NONE 0
#define TRG_TRIGGER_EDGE_PULL 1
#define TRG_TRIGGER_EDGE_RELEASE 2




void TRG_Trigger_Initialize(void);

void TRG_Trigger_Update(void);


void TRG_Trigger_ClearEdge(void);

uint32_t TRG_Trigger_GetEdge(void);

uint32_t TRG_Trigger_GetReleaseTimestampTicks(void);

uint32_t TRG_Trigger_GetPullTimestampTicks(void);

uint32_t TRG_Trigger_GetState(void);

#endif /* TRG_TRIGGER_H_ */