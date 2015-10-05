#ifndef __DUST_H__
#define __DUST_H__

#define DUST_POWER  P43
#define DUST_SIGNAL P21

extern bool_t Dust_IsReady;

extern volatile uint16_t Dust_RTData;
extern uint16_t Dust_Data;
extern uint8_t Dust_Level;
extern uint8_t Dust_LevelForFan;
extern volatile bool_t Dust_DataValid;

void Dust_DeviceInit(void);
void Dust_DataUpdate(void);
void Dust_LevelUpdate(void);

#endif


