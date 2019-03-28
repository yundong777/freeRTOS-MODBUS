/*
*********************************************************************************************************
*
*   模块名称 : MODSBUS通信程序 （主机）
*   文件名称 : modbus_host.c
*   版    本 : V1.0
*   说    明 : 通信协议基于MODBUS
*
*********************************************************************************************************
*/
#include "includes.h"
#include "MainTask.h"
extern EventGroupHandle_t xCreatedEventGroup;
QUEUE_STR queueStr;
ALL_PARAMETER_SET allParameterSet = {0,0xA000,0,0};
uint8_t modbusError = 0;
/*
**********************************************************************************************************
                                            宏定义
**********************************************************************************************************
*/
#define TIMEOUT     100     /* 接收命令超时时间, 单位ms */
#define NUM         3       /* 循环发送次数 */
/*
**********************************************************************************************************
                                            队列
**********************************************************************************************************
*/
extern SemaphoreHandle_t  queueMutex;   //队列互斥锁
int enqueue(uint16_t data[])    //入队
{
    uint8_t i;
    xSemaphoreTake(queueMutex, portMAX_DELAY);
    if ((queueStr.rear+1) % QUEUE_SIZE == queueStr.front)
    {
        xSemaphoreGive(queueMutex);
        return 0;
    }
    for(i = 0; i < QUEUE_BUF; i++)
    {
        queueStr.queueBuf[queueStr.rear][i] = data[i];
    }
    queueStr.rear = (queueStr.rear+1) % QUEUE_SIZE;
    xSemaphoreGive(queueMutex);
    return 1;
}
int dequeue(uint16_t data[])        //出队
{
    uint8_t i = 0;
    xSemaphoreTake(queueMutex, portMAX_DELAY);
    if (queueStr.rear == queueStr.front)
    {
        xSemaphoreGive(queueMutex);
        return 0;
    }
    for(i = 0; i < QUEUE_BUF; i++)
    {
        data[i] = queueStr.queueBuf[queueStr.front][i];
    }
    queueStr.front = (queueStr.front+1)%QUEUE_SIZE;
    xSemaphoreGive(queueMutex);
    return 1;
}
/*
**********************************************************************************************************
                                            变量和函数
**********************************************************************************************************
*/
/* 保存每个从机的计数器值 */
MODH_T g_tModH;
VAR_T g_tVar;
static void MODH_RxTimeOut(void);
static void MODH_AnalyzeApp(void);
static void MODH_Read_01H(void);
static void MODH_Read_02H(void);
static void MODH_Read_03H(void);
static void MODH_Read_04H(void);
static void MODH_Read_05H(void);
static void MODH_Read_06H(void);
static void MODH_Read_10H(void);
#define ModbusTaskID  HandleTaskModbus
/*
*********************************************************************************************************
*   函 数 名: MODH_SendPacket
*   功能说明: 发送数据包 COM1口
*   形    参: _buf : 数据缓冲区
*             _len : 数据长度
*   返 回 值: 无
*********************************************************************************************************
*/
void MODH_SendPacket(uint8_t *_buf, uint16_t _len)
{
    RS485_SendBuf(_buf, _len);
}
/*
*********************************************************************************************************
*   函 数 名: MODH_SendAckWithCRC
*   功能说明: 发送应答,自动加CRC.
*   形    参: 无。发送数据在 g_tModH.TxBuf[], [g_tModH.TxCount
*   返 回 值: 无
*********************************************************************************************************
*/
static void MODH_SendAckWithCRC(void)
{
    uint16_t crc;
    crc = CRC16_Modbus(g_tModH.TxBuf, g_tModH.TxCount);
    g_tModH.TxBuf[g_tModH.TxCount++] = crc >> 8;
    g_tModH.TxBuf[g_tModH.TxCount++] = crc;
    MODH_SendPacket(g_tModH.TxBuf, g_tModH.TxCount);
}
/*
*********************************************************************************************************
*   函 数 名: MODH_AnalyzeApp
*   功能说明: 分析应用层协议。处理应答。
*   形    参: 无
*   返 回 值: 无
*********************************************************************************************************
*/
static void MODH_AnalyzeApp(void)
{
    switch (g_tModH.RxBuf[1])           // 第2个字节 功能码
    {
    case 0x03:  // 读取保持寄存器 在一个或多个保持寄存器中取得当前的二进制值
        MODH_Read_03H();
        break;
    case 0x06:  // 写单个寄存器
    case 0x09:  // 写单个寄存器
        MODH_Read_06H();
        break;
    default:
        break;
    }
}
/*
*********************************************************************************************************
*   函 数 名: MODH_Send03H
*   功能说明: 发送03H指令，查询1个或多个保持寄存器
*   形    参: _addr : 从站地址
*             _reg : 寄存器编号
*             _num : 寄存器个数
*   返 回 值: 无
*********************************************************************************************************
*/
void MODH_Send03H(uint8_t _addr, uint16_t _reg, uint16_t _num)
{
    g_tModH.TxCount = 0;
    g_tModH.TxBuf[g_tModH.TxCount++] = _addr;       // 从站地址
    g_tModH.TxBuf[g_tModH.TxCount++] = 0x03;        // 功能码
    g_tModH.TxBuf[g_tModH.TxCount++] = _reg >> 8;   // 寄存器编号 高字节
    g_tModH.TxBuf[g_tModH.TxCount++] = _reg;        // 寄存器编号 低字节
    g_tModH.TxBuf[g_tModH.TxCount++] = _num >> 8;   // 寄存器个数 高字节
    g_tModH.TxBuf[g_tModH.TxCount++] = _num;        // 寄存器个数 低字节
    MODH_SendAckWithCRC();      // 发送数据，自动加CRC
    g_tModH.fAck03H = 0;        // 清接收标志
    g_tModH.RegNum = _num;      // 寄存器个数
    g_tModH.Reg03H = _reg;      // 保存03H指令中的寄存器地址，方便对应答数据进行分类
}
/*
*********************************************************************************************************
*   函 数 名: MODH_Send06H
*   功能说明: 发送06H指令，写1个保持寄存器
*   形    参: _addr : 从站地址
*             _reg : 寄存器编号
*             _value : 寄存器值,2字节
*   返 回 值: 无
*********************************************************************************************************
*/
void MODH_Send06H(uint8_t _addr, uint16_t _reg, uint16_t _value)
{
    g_tModH.TxCount = 0;
    g_tModH.TxBuf[g_tModH.TxCount++] = _addr;           // 从站地址
    if(_addr == funcCode.code.address / 100)
    {
        g_tModH.TxBuf[g_tModH.TxCount++] = 0x09;    // 功能码
    }
    else
    {
        g_tModH.TxBuf[g_tModH.TxCount++] = 0x06;    // 功能码
    }
    g_tModH.TxBuf[g_tModH.TxCount++] = _reg >> 8;       // 寄存器编号 高字节
    g_tModH.TxBuf[g_tModH.TxCount++] = _reg;            // 寄存器编号 低字节
    g_tModH.TxBuf[g_tModH.TxCount++] = _value >> 8;     // 寄存器值 高字节
    g_tModH.TxBuf[g_tModH.TxCount++] = _value;          // 寄存器值 低字节
    MODH_SendAckWithCRC();      // 发送数据，自动加CRC
    g_tModH.fAck06H = 0;        // 如果收到从机的应答，则这个标志会设为1
}
/*
*********************************************************************************************************
*   函 数 名: MODH_ReciveNew
*   功能说明: 串口接收中断服务程序会调用本函数。当收到一个字节时，执行一次本函数。
*   形    参:
*   返 回 值: 1 表示有数据
*********************************************************************************************************
*/
void MODH_ReciveNew(uint8_t _data)
{
    /*
        3.5个字符的时间间隔，只是用在RTU模式下面，因为RTU模式没有开始符和结束符，
        两个数据包之间只能靠时间间隔来区分，Modbus定义在不同的波特率下，间隔时间是不一样的，
        所以就是3.5个字符的时间，波特率高，这个时间间隔就小，波特率低，这个时间间隔相应就大
        4800  = 7.297ms
        9600  = 3.646ms
        19200  = 1.771ms
        38400  = 0.885ms
    */
    uint32_t timeout;
    timeout = 35000000 / HBAUD485;      // 计算超时时间，单位us 35000000
    /* 硬件定时中断，定时精度us 硬件定时器2用于MODBUS从机, 定时器3用于MODBUS从机主机 */
    bsp_StartHardTimer(3, timeout, (void *)MODH_RxTimeOut);
    if (g_tModH.RxCount < H_RX_BUF_SIZE)
    {
        g_tModH.RxBuf[g_tModH.RxCount++] = _data;
    }
}
/*
*********************************************************************************************************
*   函 数 名: MODH_RxTimeOut
*   功能说明: 超过3.5个字符时间后执行本函数。 设置全局变量 g_rtu_timeout = 1; 通知主程序开始解码。
*   形    参: 无
*   返 回 值: 无
*********************************************************************************************************
*/
static void MODH_RxTimeOut(void)
{
    BaseType_t xResult;
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    // 向任务vTaskMsgPro发送事件标志
    xResult = xEventGroupSetBitsFromISR(xCreatedEventGroup, // 事件标志组句柄
                                        1 ,             // 设置bit0
                                        &xHigherPriorityTaskWoken );
    // 消息被成功发出
    if( xResult != pdFAIL )
    {
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    }
}
/*
*********************************************************************************************************
*   函 数 名: MODH_Poll
*   功能说明: 接收控制器指令. 1ms 响应时间。
*   形    参: 无
*   返 回 值: 0 表示超时 1表示收到数据
*********************************************************************************************************
*/
uint8_t MODH_Poll(void)
{
    EventBits_t uxBits;
    const TickType_t xTicksToWait = 50 / portTICK_PERIOD_MS; // 最大延迟 50ms
    uint16_t crc1;
    uint8_t ret = 0;
    //等待串口接收终端中bit0置位信号
    uxBits = xEventGroupWaitBits(xCreatedEventGroup,    // 事件标志组句柄
                                 BIT_0,              // 等待 bit0 被设置
                                 pdTRUE,             // 退出前 bit0被清除，这里是 bit0 都被设置才表示“退出”
                                 pdTRUE,             // 设置为 pdTRUE表示等待 bit0 都被设置
                                 xTicksToWait);      // 等待延迟时间
    // 超过3.5个字符时间后执行MODH_RxTimeOut()函数。
    if((uxBits & BIT_0) != BIT_0)
    {
        return 0;
    }
    if (g_tModH.RxCount < 4)
    {
        ret = 1;
        goto err_ret;
    }
    // 计算CRC校验和
    crc1 = CRC16_Modbus(g_tModH.RxBuf, g_tModH.RxCount);
    if (crc1 != 0)
    {
        ret = 2;
        goto err_ret;
    }
    // 分析应用层协议
    MODH_AnalyzeApp();
err_ret:
    g_tModH.RxCount = 0;    // 必须清零计数器，方便下次帧同步
    if((ret != 1)&&(ret != 2))
    {
        ret = 3;
    }
    return ret;
}
/*
*********************************************************************************************************
*   函 数 名: MODH_Read_06H
*   功能说明: 分析06H指令的应答数据
*   形    参: 无
*   返 回 值: 无
*********************************************************************************************************
*/
static void MODH_Read_06H(void)
{
    if(g_tModH.RxBuf[2] == 0x80 && g_tModH.RxBuf[3] == 0x01)
    {
        g_tModH.fAck06H = 1;
        modbusError =  g_tModH.RxBuf[5];
        return;
    }
    if (g_tModH.RxCount > 0)
    {
        if (g_tModH.RxBuf[0] == g_tModH.TxBuf[0])
        {
            g_tModH.fAck06H = 1;        // 接收到应答
        }
    }
}
/*
*********************************************************************************************************
*   函 数 名: MODH_Read_03H
*   功能说明: 分析03H指令的应答数据
*   形    参: 无
*   返 回 值: 无
*********************************************************************************************************
*/
void MODH_Read_03H(void)
{
    uint8_t bytes;
    uint8_t *p;
    uint8_t i = 0;
    if(g_tModH.RxBuf[2] == 0x80 && g_tModH.RxBuf[3] == 0x01)
    {
        g_tModH.fAck03H = 1;
        modbusError =  g_tModH.RxBuf[5];
        return;
    }
    if (g_tModH.RxCount > 0)    //接收数据存储
    {
        if(g_tModH.RxBuf[0] == (funcCode.code.address / 100))//主机
        {
            bytes = g_tModH.RxBuf[2];   // 数据长度 字节数
            switch (g_tModH.Reg03H >> 8)
            {
            case 0XA0:
                p = &g_tModH.RxBuf[3];
                for(i = 0; i < bytes/2; i ++)
                {
                    FunctionCodeVFD.group.a0[(uint8_t)g_tModH.Reg03H + i] = BEBufToUint16(p);
                    p += 2;
                }
                break;
            case 0xA1:
                p = &g_tModH.RxBuf[3];
                for(i = 0; i < bytes/2; i ++)
                {
                    FunctionCodeVFD.group.a1[(uint8_t)g_tModH.Reg03H + i] = BEBufToUint16(p);
                    p += 2;
                }
                break;
            case 0xA2:
                p = &g_tModH.RxBuf[3];
                for(i = 0; i < bytes/2; i ++)
                {
                    FunctionCodeVFD.group.a2[(uint8_t)g_tModH.Reg03H + i] = BEBufToUint16(p);
                    p += 2;
                }
                break;
            case 0xA3:
                p = &g_tModH.RxBuf[3];
                for(i = 0; i < bytes/2; i ++)
                {
                    FunctionCodeVFD.group.a3[(uint8_t)g_tModH.Reg03H + i] = BEBufToUint16(p);
                    p += 2;
                }
                break;
            case 0xA4:
                p = &g_tModH.RxBuf[3];
                for(i = 0; i < bytes/2; i ++)
                {
                    FunctionCodeVFD.group.a4[(uint8_t)g_tModH.Reg03H + i] = BEBufToUint16(p);
                    p += 2;
                }
                break;
            case 0xA5:
                p = &g_tModH.RxBuf[3];
                for(i = 0; i < bytes/2; i ++)
                {
                    FunctionCodeVFD.group.a5[(uint8_t)g_tModH.Reg03H + i] = BEBufToUint16(p);
                    p += 2;
                }
                break;
            case 0xAF:
                p = &g_tModH.RxBuf[3];
                for(i = 0; i < bytes/2; i ++)
                {
                    FunctionCodeVFD.group.af[(uint8_t)g_tModH.Reg03H + i] = BEBufToUint16(p);
                    p += 2;
                }
                break;
            case 0xB0:
                p = &g_tModH.RxBuf[3];
                for(i = 0; i < bytes/2; i ++)
                {
                    FunctionCodeVFD.group.b0[(uint8_t)g_tModH.Reg03H + i] = BEBufToUint16(p);
                    p += 2;
                }
                break;
            case 0xB1:
                p = &g_tModH.RxBuf[3];
                for(i = 0; i < bytes/2; i ++)
                {
                    FunctionCodeVFD.group.b1[(uint8_t)g_tModH.Reg03H + i] = BEBufToUint16(p);
                    p += 2;
                }
                break;
            case 0xB2:
                p = &g_tModH.RxBuf[3];
                for(i = 0; i < bytes/2; i ++)
                {
                    FunctionCodeVFD.group.b2[(uint8_t)g_tModH.Reg03H + i] = BEBufToUint16(p);
                    p += 2;
                }
                break;
            case 0xBF:
                p = &g_tModH.RxBuf[3];
                for(i = 0; i < bytes/2; i ++)
                {
                    FunctionCodeVFD.group.bf[(uint8_t)g_tModH.Reg03H + i] = BEBufToUint16(p);
                    p += 2;
                }
                break;
            case 0xE0:
                p = &g_tModH.RxBuf[3];
                for(i = 0; i < bytes/2; i ++)
                {
                    FunctionCodeVFD.group.u0[(uint8_t)g_tModH.Reg03H + i] = BEBufToUint16(p);
                    p += 2;
                }
                break;
            case 0xE1:
                p = &g_tModH.RxBuf[3];
                for(i = 0; i < bytes/2; i ++)
                {
                    FunctionCodeVFD.group.u1[(uint8_t)g_tModH.Reg03H + i] = BEBufToUint16(p);
                    p += 2;
                }
                break;
            case 0x30:
                if(g_tModH.Reg03H == 0x3000)
                {
                    converterState = g_tModH.RxBuf[4];
                }
                break;
            case 0x80:
                if(g_tModH.Reg03H == 0x8000)
                {
                    mainVFDerrorFlag =  g_tModH.RxBuf[3]<<8| g_tModH.RxBuf[4];
                }
                break;
            }
            g_tModH.fAck03H = 1;
        }
        else if(g_tModH.RxBuf[0] == (funcCode.code.fanAddress / 100))//风机
        {
            if(g_tModH.Reg03H ==  0x1001)
            {
                fanVFDvalue[1] = (uint16_t)g_tModH.RxBuf[3]<<8| g_tModH.RxBuf[4];//1001
                fanVFDvalue[3] =  (uint16_t)g_tModH.RxBuf[7]<<8| g_tModH.RxBuf[8];//1003
                fanVFDvalue[2] =  (uint16_t)g_tModH.RxBuf[9]<<8| g_tModH.RxBuf[10];//1004
                fanVFDvalue[4] =  (uint16_t)g_tModH.RxBuf[11]<<8| g_tModH.RxBuf[12];//1005
            }
            else if(g_tModH.Reg03H == 0x8000)
            {
                fanVFDerrorFlag = (uint16_t)g_tModH.RxBuf[3]<<8| g_tModH.RxBuf[4];
            }
            else if(g_tModH.Reg03H == 0xF80E)
            {
                fanVFDvalue[5]= (uint16_t)g_tModH.RxBuf[3]<<8| g_tModH.RxBuf[4];
            }
            g_tModH.fAck03H = 1;
        }
    }
}
/*
*********************************************************************************************************
*   函 数 名: MODH_ReadParam_03H
*   功能说明: 单个参数. 通过发送03H指令实现，发送之后，等待从机应答。
*   形    参: 无
*   返 回 值: 1 表示成功。0 表示失败（通信超时或被拒绝）
*********************************************************************************************************
*/
uint8_t MODH_ReadParam_03H(uint16_t _reg, uint16_t _num,uint16_t _address)
{
    uint8_t i, res;
    for (i = 0; i < NUM; i++)
    {
        MODH_Send03H (_address, _reg, _num);          // 发送命令
        while (1)               // 等待应答,超时或接收到应答则break
        {
            res = MODH_Poll();
            if (res  == 0)
            {
                break;      // 通信超时了
            }
            if (g_tModH.fAck03H > 0)
            {
                break;
            }
        }
        if (g_tModH.fAck03H > 0)
        {
            break;
        }
        //上电变频器参数全部读取过程
        if(functionCodeVFDInitFlag == 0)
        {
            i = 0;
        }
    }
    if (g_tModH.fAck03H == 0)
    {
        return 0;
    }
    else
    {
        return 1;   // 写入03H参数成功
    }
}
/*
*********************************************************************************************************
*   函 数 名: MODH_WriteParam_06H
*   功能说明: 单个参数. 通过发送06H指令实现，发送之后，等待从机应答。循环NUM次写命令
*   形    参: 无
*   返 回 值: 1 表示成功。0 表示失败（通信超时或被拒绝）
*********************************************************************************************************
*/
uint8_t MODH_WriteParam_06H(uint16_t _reg, uint16_t _value,uint16_t _address)
{
    uint8_t i, res;
    for (i = 0; i < NUM; i++)
    {
        MODH_Send06H (_address, _reg, _value);
        while (1)               // 等待应答,超时或接收到应答则break
        {
            res = MODH_Poll();
            if (res  == 0)
            {
                break;      // 通信超时了
            }
            if (g_tModH.fAck06H > 0)
            {
                break;
            }
        }
        if (g_tModH.fAck06H > 0)
        {
            break;
        }
    }
    if (g_tModH.fAck06H == 0)
    {
        return 0;
    }
    else
    {
        return 1;   // 写入06H参数成功
    }
}
/*******************************************************************************************************/
