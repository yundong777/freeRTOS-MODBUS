/*
*********************************************************************************************************
*
*   ģ������ : MODEBUS ͨ��ģ�� (��������
*   �ļ����� : modbus_host.h
*   ��    �� : V1.0
*   ˵    �� : ͷ�ļ�
*
*********************************************************************************************************
*/
#ifndef __MOSBUS_HOST_H
#define __MOSBUS_HOST_H
#include "stdint.h"
#define SlaveAddr       0x01            // �����Ϊʱ���������ӻ� 
#define HBAUD485        UART3_BAUD
// RTU Ӧ�����
#define RSP_OK              0       // �ɹ�  
#define RSP_ERR_CMD         0x01    // ��֧�ֵĹ�����  
#define RSP_ERR_REG_ADDR    0x02    // �Ĵ�����ַ����  
#define RSP_ERR_VALUE       0x03    // ����ֵ�����  
#define RSP_ERR_WRITE       0x04    // д��ʧ��  
#define H_RX_BUF_SIZE       64
#define H_TX_BUF_SIZE       128
typedef struct
{
    uint8_t RxBuf[H_RX_BUF_SIZE];
    uint8_t RxCount;
    uint8_t RxStatus;
    uint8_t RxNewFlag;
    uint8_t RspCode;
    uint8_t TxBuf[H_TX_BUF_SIZE];
    uint8_t TxCount;
    uint16_t Reg01H;        // �����������͵ļĴ����׵�ַ
    uint16_t Reg02H;
    uint16_t Reg03H;
    uint16_t Reg04H;
    uint8_t RegNum;         // �Ĵ�������
    // Ӧ�������־ 0 ��ʾִ��ʧ�� 1��ʾִ�гɹ�
    uint8_t fAck03H;
    uint8_t fAck06H;
} MODH_T;
typedef struct
{
    // 03H 06H ��д���ּĴ���
    uint16_t P01;
    uint16_t P02;
} VAR_T;
#define QUEUE_SIZE 20
#define QUEUE_BUF  4
typedef struct
{
    uint16_t queueBuf[QUEUE_SIZE][QUEUE_BUF];
    uint8_t front;
    uint8_t rear;
} QUEUE_STR;
extern QUEUE_STR queueStr;
int enqueue(uint16_t data[]);   //���
int dequeue(uint16_t data[]);   //����
typedef struct
{
    uint16_t modousData[4];
    uint16_t address;
    uint8_t writable;
} ALL_PARAMETER_SET;
extern ALL_PARAMETER_SET allParameterSet;
extern uint8_t modbusError;
uint8_t MODH_Poll(void);
uint8_t MODH_ReadParam_03H(uint16_t _reg, uint16_t _num,uint16_t _address);
uint8_t MODH_WriteParam_06H(uint16_t _reg, uint16_t _value,uint16_t address);
extern MODH_T g_tModH;
#endif
/******************************************************************************************/
