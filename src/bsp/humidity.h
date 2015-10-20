#ifndef HUMIDITY_H
#define HUMIDITY_H


#define HT_MIN  6
#define HT_MAX  44

extern int8_t idata Humidity_CurrentT;
extern uint8_t idata Humidity_CurrentH;

extern uint8_t idata Humidity_AdHBuf;

extern bool_t NTC_IsError;


uint8_t Utils_GetAdValue(uint8_t ch);

void Humidity_DeviceInit(void);
void Humidity_GetCurrentT(void);
void Humidity_GetCurrentH(void);


#endif