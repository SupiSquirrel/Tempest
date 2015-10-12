/*
 * CFG_Config.h
 *
 * Created: 15.04.2015 21:23:06
 *  Author: Squirrel
 */ 


#ifndef CFG_CONFIG_H_
#define CFG_CONFIG_H_


void CFG_Config_Initialize(void);


uint32_t CFG_Config_GetUserSettingMaximumRofEyesOnTicks(void);
uint32_t CFG_Config_GetUserSettingMaximumRofEyesOffTicks(void);
uint32_t CFG_Config_GetUserSettingRampKickinRofTicks(void);
uint32_t CFG_Config_GetUserSettingDwellTicks(void);


#endif /* CFG_CONFIG_H_ */