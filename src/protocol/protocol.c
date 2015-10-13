#include "heads.h"

#define CMD_HEADER              0xF1
#define ACK_HEADER              0xF2
#define END                     0x7E

#define CMD_GET_SYS_STA         0x01
#define CMD_POWER_CTR           0x02
#define CMD_FAN_CTR             0x03
#define CMD_FIX_CTR             0x04
#define CMD_CLEAN_CTR           0x05
#define CMD_AION_CTR            0x06
#define CMD_UV_CTR              0x07
#define CMD_TIMER_CTR           0x08
#define CMD_GET_PM_DATAS        0x09
#define CMD_GET_ENE_DATAS       0x0A
#define CMD_RT_SET              0x0B
#define CMD_MEM_SET             0x0C
#define CMD_REST                0x0D
#define CMD_GET_DEVICE_INFO     0xF7
#define CMD_WIFI_STA_CHANGE     0XFF


#define CMD_GET_SYS_STA_LEN     0x01
#define CMD_POWER_CTR_LEN       0x01
#define CMD_FAN_CTR_LEN         0x01
#define CMD_FIX_CTR_LEN         0x01
#define CMD_CLEAN_CTR_LEN       0x01
#define CMD_AION_CTR_LEN        0x01
#define CMD_UV_CTR_LEN          0x01
#define CMD_TIMER_CTR_LEN       0x0C
#define CMD_GET_PM_DATAS_LEN    0x01
#define CMD_GET_ENE_DATAS_LEN   0x01
#define CMD_RT_SET_LEN          0x06
#define CMD_MEM_SET_LEN         0x01
#define CMD_REST_LEN            0x01
#define CMD_GET_DEVICE_INFO_LEN 0x01
#define CMD_WIFI_STA_CHANGE_LEN 0X06


static void Protocol_StaResp(uint8_t cmd)
{
    uint8_t sum;

    sum = 0;

    Enqueue(Tx_Queue, 0xf2);
    Enqueue(Tx_Queue, 0xf2);

    sum += Enqueue(Tx_Queue, cmd);
    sum += Enqueue(Tx_Queue, 0x25);   /* len */

    sum += Enqueue(Tx_Queue, Sys_PowerSta);   /* 开关机状态 1：开机 2：关机 */

    sum += Enqueue(Tx_Queue,H(Dust_Data));           /* PM2.5 */
    sum += Enqueue(Tx_Queue,L(Dust_Data));

    sum += Enqueue(Tx_Queue,H(Sys_AmountOfWipedDust));     /* 去除的粉尘量 */
    sum += Enqueue(Tx_Queue,L(Sys_AmountOfWipedDust));

    sum += Enqueue(Tx_Queue,H(Sys_TotalWorkedTime));        /* 运行时间 */
    sum += Enqueue(Tx_Queue,L(Sys_TotalWorkedTime));

    sum += Enqueue(Tx_Queue,H(Sys_LeftTime));           /* 剩余使用时间 */
    sum += Enqueue(Tx_Queue,L(Sys_LeftTime));

    sum += Enqueue(Tx_Queue, Humidity_CurrentT);        /* 温度 */
    sum += Enqueue(Tx_Queue, Humidity_CurrentH);        /* 湿度 */
                     
    if (Sys_IsAutoMode)                                 /* 档位 */
    {
        sum += Enqueue(Tx_Queue, 0);
    }
    else
    {
        sum += Enqueue(Tx_Queue, Sys_SpOption);
    }

    sum += Enqueue(Tx_Queue,Sys_FixMode == FM_FIXING ? 1 : 0);      /* 维护 */
    sum += Enqueue(Tx_Queue,Sys_CleanMode == CM_CLEANING ? 1 : 0);  /* 清洗 */

    sum += Enqueue(Tx_Queue,Sys_AionSta);                   /* 负离子 */
    sum += Enqueue(Tx_Queue,Sys_UVLedSta);                  /* UV灯 */

    sum += Enqueue(Tx_Queue,Sys_PowerOnPoint1.bytes[0]);
    sum += Enqueue(Tx_Queue,Sys_PowerOnPoint1.bytes[1]);
    sum += Enqueue(Tx_Queue,Sys_PowerOffPoint1.bytes[0]);
    sum += Enqueue(Tx_Queue,Sys_PowerOffPoint1.bytes[1]);

    sum += Enqueue(Tx_Queue,Sys_PowerOnPoint2.bytes[0]);
    sum += Enqueue(Tx_Queue,Sys_PowerOnPoint2.bytes[1]);
    sum += Enqueue(Tx_Queue,Sys_PowerOffPoint2.bytes[0]);
    sum += Enqueue(Tx_Queue,Sys_PowerOffPoint2.bytes[1]);

    sum += Enqueue(Tx_Queue,Sys_PowerOnPoint3.bytes[0]);
    sum += Enqueue(Tx_Queue,Sys_PowerOnPoint3.bytes[1]);
    sum += Enqueue(Tx_Queue,Sys_PowerOffPoint3.bytes[0]);
    sum += Enqueue(Tx_Queue,Sys_PowerOffPoint3.bytes[1]);

    sum += Enqueue(Tx_Queue,0);     /* 总能耗：整数 */
    sum += Enqueue(Tx_Queue, 0);
    /* 总能耗：小数字 */
    sum += Enqueue(Tx_Queue, 0);

    sum += Enqueue(Tx_Queue, 0);     /* 月能耗：整数 */
    sum += Enqueue(Tx_Queue, 0);

    sum += Enqueue(Tx_Queue, 0);     /* 月能耗：小数字 */

    sum += Enqueue(Tx_Queue, 0);     /* 故障：38 */

    sum += Enqueue(Tx_Queue, 0);     /* 预留 */
    sum += Enqueue(Tx_Queue, 0);

    
    Enqueue(Tx_Queue, sum);
    Enqueue(Tx_Queue, 0x7e);

    Uart_SendStartup();
}


static void CMD_DUMMY_DEAL(void)
{
   
}

static void CMD_GET_SYS_STA_DEAL(void)
{
    Protocol_StaResp(CMD_GET_SYS_STA);
}
static void CMD_POWER_CTR_DEAL(void)
{
    Sys_PowerSta = (PowerSta_t)Rx_Buf[2];
    if (Sys_PowerSta == POWER_ON)
    {
        State_TransitionTo(&State_Work, TRUE, FALSE);
    }
    else
    {
        State_TransitionTo(&State_Standby, TRUE, FALSE);
    }
    Buzz_Set(1, 10, 15);
    Protocol_StaResp(CMD_POWER_CTR);
}
static void CMD_FAN_CTR_DEAL(void)
{
    if (Sys_PowerSta == POWER_ON)
    {
        if (Rx_Buf[2] == 0)
        {
            Sys_IsAutoMode = TRUE;
        }
        else
        {
            Sys_IsAutoMode = FALSE;
            Sys_SpOption = (SpOptions_t)Rx_Buf[2];
        }

        /* 一档状态，UV,Aion灯自动关闭 */
        if (Sys_SpOption == SP_SLEEP)
        {
            Sys_UVLedSta = UV_LED_OFF;
            Sys_AionSta = AION_OFF;
        }
        /* 2 - 5 档自动开启UV灯 */
        else if (Sys_SpOption > SP_SLEEP)
        {
            Sys_UVLedSta = UV_LED_ON;
            Sys_AionSta = AION_ON;
        }

        Buzz_Set(1, 10, 15);
    }

    Protocol_StaResp(CMD_FAN_CTR);
}
static void CMD_FIX_CTR_DEAL(void)
{
    Sys_FixMode = (FixMode_t)Rx_Buf[2];
    Buzz_Set(1, 10, 15);
    Protocol_StaResp(CMD_FIX_CTR);
}
static void CMD_CLEAN_CTR_DEAL(void)
{
    Sys_CleanMode = (CleanMode_t)Rx_Buf[2];
    Buzz_Set(1, 10, 15);
    Protocol_StaResp(CMD_CLEAN_CTR);
}
static void CMD_AION_CTR_DEAL(void)
{
    if (Sys_PowerSta == POWER_ON)
    {
        Sys_AionSta = (AionSta_t)Rx_Buf[2];
        Buzz_Set(1, 10, 15);
    }

    Protocol_StaResp(CMD_AION_CTR);
}
static void CMD_UV_CTR_DEAL(void)
{
    if (Sys_PowerSta == POWER_ON)
    {
        Sys_UVLedSta = (UVLedSta_t)Rx_Buf[2];
        Buzz_Set(1, 10, 15);
    }

    Protocol_StaResp(CMD_UV_CTR);
}
static void CMD_TIMER_CTR_DEAL(void)
{
    Sys_PowerOnPoint1.bytes[0] = Rx_Buf[2];
    Sys_PowerOnPoint1.bytes[1] = Rx_Buf[3];
    Sys_PowerOffPoint1.bytes[0] = Rx_Buf[4];
    Sys_PowerOffPoint1.bytes[1] = Rx_Buf[5];

    Sys_PowerOnPoint2.bytes[0] = Rx_Buf[6];
    Sys_PowerOnPoint2.bytes[1] = Rx_Buf[7];
    Sys_PowerOffPoint2.bytes[0] = Rx_Buf[8];
    Sys_PowerOffPoint2.bytes[1] = Rx_Buf[9];

    Sys_PowerOnPoint3.bytes[0] = Rx_Buf[10];
    Sys_PowerOnPoint3.bytes[1] = Rx_Buf[11];
    Sys_PowerOffPoint3.bytes[0] = Rx_Buf[12];
    Sys_PowerOffPoint3.bytes[1] = Rx_Buf[13];

    Sys_TimerFunEn = TRUE;
    Sys_RemoteTimerCtr.funEn = FALSE;

    Sys_UpdateTimerOnIndex();

    Buzz_Set(1, 10, 15);
    Protocol_StaResp(CMD_TIMER_CTR);
}
static void CMD_GET_PM_DATAS_DEAL(void)
{
    uint8_t sum;
    uint8_t i;
    DustRecord_t* ptr;

    if (Rx_Buf[2] == 1)
    {
        ptr = &Sys_Pm25Records_10;
    }
    else
    {
        ptr = &Sys_Pm25Records_30;
    }

    Enqueue(Tx_Queue, 0xf2);
    Enqueue(Tx_Queue, 0xf2);

    sum = Enqueue(Tx_Queue, CMD_GET_PM_DATAS);
    sum += Enqueue(Tx_Queue, 0x1a);   /* len */

                           
    sum += Enqueue(Tx_Queue, ptr->hour);
    sum += Enqueue(Tx_Queue, ptr->min);

    for (i = 0; i < 12;i++)
    {
        sum += Enqueue(Tx_Queue, H(ptr->records[i]));
        sum += Enqueue(Tx_Queue, L(ptr->records[i]));
    }

    Enqueue(Tx_Queue, sum);
    Enqueue(Tx_Queue, 0x7e);

    Buzz_Set(1, 10, 15);

    Uart_SendStartup();
}
static void CMD_GET_ENE_DATAS_DEAL(void)
{
    uint8_t sum;
    uint8_t i;

    Enqueue(Tx_Queue, 0xf2);
    Enqueue(Tx_Queue, 0xf2);

    sum = Enqueue(Tx_Queue, CMD_GET_ENE_DATAS);
    sum += Enqueue(Tx_Queue, 0x14);   /* len */

    for (i = 0; i < 20;i++)
    {
        sum += Enqueue(Tx_Queue, Sys_UsedTimeRecord.bytes[i]);
    }
    
    Enqueue(Tx_Queue, sum);
    Enqueue(Tx_Queue, 0x7e);

    Buzz_Set(1, 10, 15);

    Uart_SendStartup();
}
static void CMD_RT_SET_DEAL(void)
{
    Sys_ClockTime.year =
        (uint16_t)((Rx_Buf[2] << 8) + Rx_Buf[3]);
    Sys_ClockTime.month = Rx_Buf[4];
    Sys_ClockTime.day = Rx_Buf[5];
    Sys_ClockTime.hour = Rx_Buf[6];
    Sys_ClockTime.min = Rx_Buf[7];

    Buzz_Set(1, 10, 15);

    Protocol_StaResp(CMD_RT_SET);
}
static void CMD_MEM_SET_DEAL(void)
{
    /* 执行一键记忆功能 */

    Sys_MemoryDataExist = TRUE;
    State_TransitionTo(&State_Standby, TRUE, FALSE);

    Buzz_Set(1, 10, 15);
    Protocol_StaResp(CMD_MEM_SET);
}
static void CMD_REST_DEAL(void)
{
    uint8_t i;
    /* 执行恢复设备状态功能 */
    Sys_IsAutoMode = FALSE;
    Sys_SpOption = SP_MID;

    Sys_PowerOnPoint1.bytes[0] = 0;
    Sys_PowerOffPoint1.bytes[0] = 0;
    Sys_PowerOnPoint2.bytes[0] = 0;
    Sys_PowerOffPoint2.bytes[0] = 0;
    Sys_PowerOnPoint3.bytes[0] = 0;
    Sys_PowerOffPoint3.bytes[0] = 0;

    Sys_PowerOnPoint1.bytes[1] = 0;
    Sys_PowerOffPoint1.bytes[1] = 0;
    Sys_PowerOnPoint2.bytes[1] = 0;
    Sys_PowerOffPoint2.bytes[1] = 0;
    Sys_PowerOnPoint3.bytes[1] = 0;
    Sys_PowerOffPoint3.bytes[1] = 0;

    for (i = 0; i < 20;i++)
    {
        Sys_UsedTimeRecord.bytes[i] = 0;
    }

    Sys_TimerFunEn = FALSE;
    Sys_RemoteTimerCtr.funEn = FALSE;
    Sys_RemoteTimerCtr.segmentIndex = 0;

    Sys_TimerOnIndex = 0;
    Sys_TimerStartedIndex = 0;

    Sys_NHVModeEn = FALSE;

    Sys_EWorkedMin = 0;
    Sys_EDispEn = FALSE;

    Sys_MemoryDataExist = FALSE;
    Sys_TotalWorkedTime = 0;

    State_TransitionTo(&State_Standby, TRUE, FALSE);
    Buzz_Set(1, 10, 15);
    Protocol_StaResp(CMD_REST);
}

/* 获取主板设备信息命令 - 应答 */
static void CMD_GET_DEVICE_INFO_DEAL(void)
{
    uint8_t sum;

    sum = 0;

    Enqueue(Tx_Queue, 0xf2);
    Enqueue(Tx_Queue, 0xf2);

    sum += Enqueue(Tx_Queue, CMD_GET_DEVICE_INFO);

    sum += Enqueue(Tx_Queue, 0x0e);


    sum += Enqueue(Tx_Queue, 0x01);   /* 厂家名称 */
    sum += Enqueue(Tx_Queue, 0x3f);

    sum += Enqueue(Tx_Queue,0x01);   /* 应用厂家 */
    sum += Enqueue(Tx_Queue,0x3f);

    sum += Enqueue(Tx_Queue,0x01);   /* 设备类型 */
    sum += Enqueue(Tx_Queue,0x00);

    sum += Enqueue(Tx_Queue,0x00);  /* 设备型号 */
    sum += Enqueue(Tx_Queue,0x00);

    sum += Enqueue(Tx_Queue,0x00);  /* 硬件版本 */
    sum += Enqueue(Tx_Queue,0x00);

    sum += Enqueue(Tx_Queue,0x00);  /* 软件版本 */
    sum += Enqueue(Tx_Queue,0x00);

    sum += Enqueue(Tx_Queue,0x00);  /* 生产日期 */
    sum += Enqueue(Tx_Queue,0x00);

    Enqueue(Tx_Queue, sum);

    Enqueue(Tx_Queue, 0x7e);

    Uart_SendStartup();
}
static void CMD_WIFI_STA_CHANGE_DEAL(void)
{
    /* 信号强度 */
    Sys_WifiStrength = Rx_Buf[2];
    /* 路由器连接状态 */
    Sys_RouteConnSta = (ConnSta_t)Rx_Buf[3];
    /* 平台连接状态 */
    Sys_PlanformConnSta = (ConnSta_t)Rx_Buf[4];
}

struct _protocolTab
{
    uint8_t cmd;
    uint8_t len;
    task_t cmdDeal;
};

code struct _protocolTab ProtocolTab[]=
{
    { CMD_GET_SYS_STA,      CMD_GET_SYS_STA_LEN,        CMD_GET_SYS_STA_DEAL        },
    { CMD_POWER_CTR,        CMD_POWER_CTR_LEN,          CMD_POWER_CTR_DEAL          },
    { CMD_FAN_CTR,          CMD_FAN_CTR_LEN,            CMD_FAN_CTR_DEAL            },
    { CMD_FIX_CTR,          CMD_FIX_CTR_LEN,            CMD_FIX_CTR_DEAL            },
    { CMD_CLEAN_CTR,        CMD_CLEAN_CTR_LEN,          CMD_CLEAN_CTR_DEAL          },
    { CMD_AION_CTR,         CMD_AION_CTR_LEN,           CMD_AION_CTR_DEAL           },
    { CMD_UV_CTR,           CMD_UV_CTR_LEN,             CMD_UV_CTR_DEAL             },
    { CMD_TIMER_CTR,        CMD_TIMER_CTR_LEN,          CMD_TIMER_CTR_DEAL          },
    { CMD_GET_PM_DATAS,     CMD_GET_PM_DATAS_LEN,       CMD_GET_PM_DATAS_DEAL       },
    { CMD_GET_ENE_DATAS,    CMD_GET_ENE_DATAS_LEN,      CMD_GET_ENE_DATAS_DEAL      },
    { CMD_RT_SET,           CMD_RT_SET_LEN,             CMD_RT_SET_DEAL             },
    { CMD_MEM_SET,          CMD_MEM_SET_LEN,            CMD_MEM_SET_DEAL            },
    { CMD_REST,             CMD_REST_LEN,               CMD_REST_DEAL               },
    { CMD_GET_DEVICE_INFO,  CMD_GET_DEVICE_INFO_LEN,    CMD_GET_DEVICE_INFO_DEAL    },
    { CMD_WIFI_STA_CHANGE,  CMD_WIFI_STA_CHANGE_LEN,    CMD_WIFI_STA_CHANGE_DEAL    },
};


static bool_t isWaitingHeader;
static uint8_t rxIndex;

void Protocol_Init(void)
{
	isWaitingHeader = TRUE;
	rxIndex = 0;
}

void Protocol_EnterATmode(void)
{
    /* 进入AT指令模式 2b 2b 2b 41 74 43 6d 64 0d 0a */
    Enqueue(Tx_Queue, 0x2b);
    Enqueue(Tx_Queue,0x2b);
    Enqueue(Tx_Queue,0x2b);
    Enqueue(Tx_Queue,0x41);
    Enqueue(Tx_Queue,0x74);
    Enqueue(Tx_Queue,0x43);
    Enqueue(Tx_Queue,0x6d);
    Enqueue(Tx_Queue,0x64);
    Enqueue(Tx_Queue,0x0d);
    Enqueue(Tx_Queue,0x0a);

    Uart_SendStartup();
}

void Protocol_ResetWifiMode(void)
{
    /* 复位模组 41 54 2b 44 65 66 61 75 6c 74 3d 31 0d 0a */

    Enqueue(Tx_Queue,0x41);
    Enqueue(Tx_Queue,0x54);
    Enqueue(Tx_Queue,0x2b);
    Enqueue(Tx_Queue,0x44);
    Enqueue(Tx_Queue,0x65);
    Enqueue(Tx_Queue,0x66);
    Enqueue(Tx_Queue,0x61);
    Enqueue(Tx_Queue,0x75);
    Enqueue(Tx_Queue,0x6c);
    Enqueue(Tx_Queue,0x74);
    Enqueue(Tx_Queue,0x3d);
    Enqueue(Tx_Queue,0x31);
    Enqueue(Tx_Queue,0x0d);
    Enqueue(Tx_Queue,0x0a);

    Uart_SendStartup();
}

void Protocol_Task(void)
{
	uint8_t dat;
    task_t cmdDeal;

	static uint8_t checksum;

	static uint8_t len;
	static uint8_t cmd;

    cmdDeal = CMD_DUMMY_DEAL;
	//while (!QueueIsEmpty(Rx_Queue))
	while((Rx_Queue->head) != (Rx_Queue->tail))
	{
		//dat = Dequeue(Rx_Queue);
		dat = Rx_Queue->elements[Rx_Queue->head];
		Rx_Queue->head = (Rx_Queue->head + 1) % QueueArraySize;

		/* prototol head detect */
		if (isWaitingHeader)
		{
			if (dat == CMD_HEADER)
			{
				rxIndex++;
				if (rxIndex == 2)
				{
					isWaitingHeader = FALSE;
					rxIndex = 0;
					checksum = 0;
				}
			}
			else
			{
				rxIndex = 0;
			}
		}
		else
		{
			Rx_Buf[rxIndex] = dat;
			checksum += dat;

			rxIndex++;

            if (rxIndex == 1)
            {
                cmd = Rx_Buf[0];
            }
            else if (rxIndex == 2)
            {
                if (cmd > 0 && cmd <= 0x0d)
                {
                    len = ProtocolTab[cmd - 1].len;
                    cmdDeal = ProtocolTab[cmd - 1].cmdDeal;
                }
                else if (cmd == 0xf7)
                {
                    len = ProtocolTab[0x0d].len;
                    cmdDeal = ProtocolTab[0x0d].cmdDeal;
                }
                else if (cmd == 0xff)
                {
                    len = ProtocolTab[0x0e].len;
                    cmdDeal = ProtocolTab[0x0e].cmdDeal;
                }
                else
                {
                    /* Cmd unknow */
                    isWaitingHeader = TRUE;
                    rxIndex = 0;
                    return;
                }

                if (len != Rx_Buf[1])
                {
                    /* cmd's len is not correct */
                    isWaitingHeader = TRUE;
                    rxIndex = 0;
                    return;
                }
            }
            else if (rxIndex >= (len + 4))
            {

                if (checksum == (uint8_t)(Rx_Buf[len + 2] +
                    Rx_Buf[len + 2] +
                    Rx_Buf[len + 3]))
                {
                    /* checksum is ok */
                    cmdDeal();
                }

                isWaitingHeader = TRUE;
                rxIndex = 0;
            }
		}
	}
}

