/*
*********************************************************************************************************
*
*   模块名称 : MODEBUS 通信模块 (主机程序）
*   文件名称 : modbus_host.h
*   版    本 : V1.0
*   说    明 : 头文件
*
*********************************************************************************************************
*/
#ifndef __MOSBUS_HOST_H
#define __MOSBUS_HOST_H
#include "stdint.h"
#define SlaveAddr       0x01            // 面板作为时，主板作从机 
#define HBAUD485        UART3_BAUD
// RTU 应答代码
#define RSP_OK              0       // 成功  
#define RSP_ERR_CMD         0x01    // 不支持的功能码  
#define RSP_ERR_REG_ADDR    0x02    // 寄存器地址错误  
#define RSP_ERR_VALUE       0x03    // 数据值域错误  
#define RSP_ERR_WRITE       0x04    // 写入失败  
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
    uint16_t Reg01H;        // 保存主机发送的寄存器首地址
    uint16_t Reg02H;
    uint16_t Reg03H;
    uint16_t Reg04H;
    uint8_t RegNum;         // 寄存器个数
    // 应答命令标志 0 表示执行失败 1表示执行成功
    uint8_t fAck03H;
    uint8_t fAck06H;
} MODH_T;
typedef struct
{
    // 03H 06H 读写保持寄存器
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
int enqueue(uint16_t data[]);   //入队
int dequeue(uint16_t data[]);   //出队
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
