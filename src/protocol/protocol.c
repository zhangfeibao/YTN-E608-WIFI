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

static void CMD_DUMMY_DEAL(void)
{
    uint8_t sum;

    sum += Enqueue(Tx_Queue,0x01);
    sum += Enqueue(Tx_Queue,0x02);

    Uart_SendStartup();
}

static void CMD_GET_SYS_STA_DEAL(void)
{

}
static void CMD_POWER_CTR_DEAL(void)
{

}
static void CMD_FAN_CTR_DEAL(void)
{

}
static void CMD_FIX_CTR_DEAL(void)
{

}
static void CMD_CLEAN_CTR_DEAL(void)
{

}
static void CMD_AION_CTR_DEAL(void)
{

}
static void CMD_UV_CTR_DEAL(void)
{

}
static void CMD_TIMER_CTR_DEAL(void)
{

}
static void CMD_GET_PM_DATAS_DEAL(void)
{

}
static void CMD_GET_ENE_DATAS_DEAL(void)
{

}
static void CMD_RT_SET_DEAL(void)
{

}
static void CMD_MEM_SET_DEAL(void)
{

}
static void CMD_REST_DEAL(void)
{

}
static void CMD_GET_DEVICE_INFO_DEAL(void)
{

}
static void CMD_WIFI_STA_CHANGE_DEAL(void)
{

}

struct _protocolTab
{
    uint8_t cmd;
    uint8_t len;
    task_t cmdDeal;
};

struct _protocolTab ProtocolTab[]=
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


void Protocol_Task(void)
{
	uint8_t dat;
    task_t cmdDeal;

	static uint8_t checksum;

	static uint8_t len;
	static uint8_t cmd;

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

