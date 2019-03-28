/*
*********************************************************************************************************
*
*   ģ������ : MODSBUSͨ�ų��� ��������
*   �ļ����� : modbus_host.c
*   ��    �� : V1.0
*   ˵    �� : ͨ��Э�����MODBUS
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
                                            �궨��
**********************************************************************************************************
*/
#define TIMEOUT     100     /* �������ʱʱ��, ��λms */
#define NUM         3       /* ѭ�����ʹ��� */
/*
**********************************************************************************************************
                                            ����
**********************************************************************************************************
*/
extern SemaphoreHandle_t  queueMutex;   //���л�����
int enqueue(uint16_t data[])    //���
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
int dequeue(uint16_t data[])        //����
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
                                            �����ͺ���
**********************************************************************************************************
*/
/* ����ÿ���ӻ��ļ�����ֵ */
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
*   �� �� ��: MODH_SendPacket
*   ����˵��: �������ݰ� COM1��
*   ��    ��: _buf : ���ݻ�����
*             _len : ���ݳ���
*   �� �� ֵ: ��
*********************************************************************************************************
*/
void MODH_SendPacket(uint8_t *_buf, uint16_t _len)
{
    RS485_SendBuf(_buf, _len);
}
/*
*********************************************************************************************************
*   �� �� ��: MODH_SendAckWithCRC
*   ����˵��: ����Ӧ��,�Զ���CRC.
*   ��    ��: �ޡ����������� g_tModH.TxBuf[], [g_tModH.TxCount
*   �� �� ֵ: ��
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
*   �� �� ��: MODH_AnalyzeApp
*   ����˵��: ����Ӧ�ò�Э�顣����Ӧ��
*   ��    ��: ��
*   �� �� ֵ: ��
*********************************************************************************************************
*/
static void MODH_AnalyzeApp(void)
{
    switch (g_tModH.RxBuf[1])           // ��2���ֽ� ������
    {
    case 0x03:  // ��ȡ���ּĴ��� ��һ���������ּĴ�����ȡ�õ�ǰ�Ķ�����ֵ
        MODH_Read_03H();
        break;
    case 0x06:  // д�����Ĵ���
    case 0x09:  // д�����Ĵ���
        MODH_Read_06H();
        break;
    default:
        break;
    }
}
/*
*********************************************************************************************************
*   �� �� ��: MODH_Send03H
*   ����˵��: ����03Hָ���ѯ1���������ּĴ���
*   ��    ��: _addr : ��վ��ַ
*             _reg : �Ĵ������
*             _num : �Ĵ�������
*   �� �� ֵ: ��
*********************************************************************************************************
*/
void MODH_Send03H(uint8_t _addr, uint16_t _reg, uint16_t _num)
{
    g_tModH.TxCount = 0;
    g_tModH.TxBuf[g_tModH.TxCount++] = _addr;       // ��վ��ַ
    g_tModH.TxBuf[g_tModH.TxCount++] = 0x03;        // ������
    g_tModH.TxBuf[g_tModH.TxCount++] = _reg >> 8;   // �Ĵ������ ���ֽ�
    g_tModH.TxBuf[g_tModH.TxCount++] = _reg;        // �Ĵ������ ���ֽ�
    g_tModH.TxBuf[g_tModH.TxCount++] = _num >> 8;   // �Ĵ������� ���ֽ�
    g_tModH.TxBuf[g_tModH.TxCount++] = _num;        // �Ĵ������� ���ֽ�
    MODH_SendAckWithCRC();      // �������ݣ��Զ���CRC
    g_tModH.fAck03H = 0;        // ����ձ�־
    g_tModH.RegNum = _num;      // �Ĵ�������
    g_tModH.Reg03H = _reg;      // ����03Hָ���еļĴ�����ַ�������Ӧ�����ݽ��з���
}
/*
*********************************************************************************************************
*   �� �� ��: MODH_Send06H
*   ����˵��: ����06Hָ�д1�����ּĴ���
*   ��    ��: _addr : ��վ��ַ
*             _reg : �Ĵ������
*             _value : �Ĵ���ֵ,2�ֽ�
*   �� �� ֵ: ��
*********************************************************************************************************
*/
void MODH_Send06H(uint8_t _addr, uint16_t _reg, uint16_t _value)
{
    g_tModH.TxCount = 0;
    g_tModH.TxBuf[g_tModH.TxCount++] = _addr;           // ��վ��ַ
    if(_addr == funcCode.code.address / 100)
    {
        g_tModH.TxBuf[g_tModH.TxCount++] = 0x09;    // ������
    }
    else
    {
        g_tModH.TxBuf[g_tModH.TxCount++] = 0x06;    // ������
    }
    g_tModH.TxBuf[g_tModH.TxCount++] = _reg >> 8;       // �Ĵ������ ���ֽ�
    g_tModH.TxBuf[g_tModH.TxCount++] = _reg;            // �Ĵ������ ���ֽ�
    g_tModH.TxBuf[g_tModH.TxCount++] = _value >> 8;     // �Ĵ���ֵ ���ֽ�
    g_tModH.TxBuf[g_tModH.TxCount++] = _value;          // �Ĵ���ֵ ���ֽ�
    MODH_SendAckWithCRC();      // �������ݣ��Զ���CRC
    g_tModH.fAck06H = 0;        // ����յ��ӻ���Ӧ���������־����Ϊ1
}
/*
*********************************************************************************************************
*   �� �� ��: MODH_ReciveNew
*   ����˵��: ���ڽ����жϷ���������ñ����������յ�һ���ֽ�ʱ��ִ��һ�α�������
*   ��    ��:
*   �� �� ֵ: 1 ��ʾ������
*********************************************************************************************************
*/
void MODH_ReciveNew(uint8_t _data)
{
    /*
        3.5���ַ���ʱ������ֻ������RTUģʽ���棬��ΪRTUģʽû�п�ʼ���ͽ�������
        �������ݰ�֮��ֻ�ܿ�ʱ���������֣�Modbus�����ڲ�ͬ�Ĳ������£����ʱ���ǲ�һ���ģ�
        ���Ծ���3.5���ַ���ʱ�䣬�����ʸߣ����ʱ������С�������ʵͣ����ʱ������Ӧ�ʹ�
        4800  = 7.297ms
        9600  = 3.646ms
        19200  = 1.771ms
        38400  = 0.885ms
    */
    uint32_t timeout;
    timeout = 35000000 / HBAUD485;      // ���㳬ʱʱ�䣬��λus 35000000
    /* Ӳ����ʱ�жϣ���ʱ����us Ӳ����ʱ��2����MODBUS�ӻ�, ��ʱ��3����MODBUS�ӻ����� */
    bsp_StartHardTimer(3, timeout, (void *)MODH_RxTimeOut);
    if (g_tModH.RxCount < H_RX_BUF_SIZE)
    {
        g_tModH.RxBuf[g_tModH.RxCount++] = _data;
    }
}
/*
*********************************************************************************************************
*   �� �� ��: MODH_RxTimeOut
*   ����˵��: ����3.5���ַ�ʱ���ִ�б������� ����ȫ�ֱ��� g_rtu_timeout = 1; ֪ͨ������ʼ���롣
*   ��    ��: ��
*   �� �� ֵ: ��
*********************************************************************************************************
*/
static void MODH_RxTimeOut(void)
{
    BaseType_t xResult;
    BaseType_t xHigherPriorityTaskWoken = pdFALSE;
    // ������vTaskMsgPro�����¼���־
    xResult = xEventGroupSetBitsFromISR(xCreatedEventGroup, // �¼���־����
                                        1 ,             // ����bit0
                                        &xHigherPriorityTaskWoken );
    // ��Ϣ���ɹ�����
    if( xResult != pdFAIL )
    {
        portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
    }
}
/*
*********************************************************************************************************
*   �� �� ��: MODH_Poll
*   ����˵��: ���տ�����ָ��. 1ms ��Ӧʱ�䡣
*   ��    ��: ��
*   �� �� ֵ: 0 ��ʾ��ʱ 1��ʾ�յ�����
*********************************************************************************************************
*/
uint8_t MODH_Poll(void)
{
    EventBits_t uxBits;
    const TickType_t xTicksToWait = 50 / portTICK_PERIOD_MS; // ����ӳ� 50ms
    uint16_t crc1;
    uint8_t ret = 0;
    //�ȴ����ڽ����ն���bit0��λ�ź�
    uxBits = xEventGroupWaitBits(xCreatedEventGroup,    // �¼���־����
                                 BIT_0,              // �ȴ� bit0 ������
                                 pdTRUE,             // �˳�ǰ bit0������������� bit0 �������òű�ʾ���˳���
                                 pdTRUE,             // ����Ϊ pdTRUE��ʾ�ȴ� bit0 ��������
                                 xTicksToWait);      // �ȴ��ӳ�ʱ��
    // ����3.5���ַ�ʱ���ִ��MODH_RxTimeOut()������
    if((uxBits & BIT_0) != BIT_0)
    {
        return 0;
    }
    if (g_tModH.RxCount < 4)
    {
        ret = 1;
        goto err_ret;
    }
    // ����CRCУ���
    crc1 = CRC16_Modbus(g_tModH.RxBuf, g_tModH.RxCount);
    if (crc1 != 0)
    {
        ret = 2;
        goto err_ret;
    }
    // ����Ӧ�ò�Э��
    MODH_AnalyzeApp();
err_ret:
    g_tModH.RxCount = 0;    // ��������������������´�֡ͬ��
    if((ret != 1)&&(ret != 2))
    {
        ret = 3;
    }
    return ret;
}
/*
*********************************************************************************************************
*   �� �� ��: MODH_Read_06H
*   ����˵��: ����06Hָ���Ӧ������
*   ��    ��: ��
*   �� �� ֵ: ��
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
            g_tModH.fAck06H = 1;        // ���յ�Ӧ��
        }
    }
}
/*
*********************************************************************************************************
*   �� �� ��: MODH_Read_03H
*   ����˵��: ����03Hָ���Ӧ������
*   ��    ��: ��
*   �� �� ֵ: ��
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
    if (g_tModH.RxCount > 0)    //�������ݴ洢
    {
        if(g_tModH.RxBuf[0] == (funcCode.code.address / 100))//����
        {
            bytes = g_tModH.RxBuf[2];   // ���ݳ��� �ֽ���
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
        else if(g_tModH.RxBuf[0] == (funcCode.code.fanAddress / 100))//���
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
*   �� �� ��: MODH_ReadParam_03H
*   ����˵��: ��������. ͨ������03Hָ��ʵ�֣�����֮�󣬵ȴ��ӻ�Ӧ��
*   ��    ��: ��
*   �� �� ֵ: 1 ��ʾ�ɹ���0 ��ʾʧ�ܣ�ͨ�ų�ʱ�򱻾ܾ���
*********************************************************************************************************
*/
uint8_t MODH_ReadParam_03H(uint16_t _reg, uint16_t _num,uint16_t _address)
{
    uint8_t i, res;
    for (i = 0; i < NUM; i++)
    {
        MODH_Send03H (_address, _reg, _num);          // ��������
        while (1)               // �ȴ�Ӧ��,��ʱ����յ�Ӧ����break
        {
            res = MODH_Poll();
            if (res  == 0)
            {
                break;      // ͨ�ų�ʱ��
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
        //�ϵ��Ƶ������ȫ����ȡ����
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
        return 1;   // д��03H�����ɹ�
    }
}
/*
*********************************************************************************************************
*   �� �� ��: MODH_WriteParam_06H
*   ����˵��: ��������. ͨ������06Hָ��ʵ�֣�����֮�󣬵ȴ��ӻ�Ӧ��ѭ��NUM��д����
*   ��    ��: ��
*   �� �� ֵ: 1 ��ʾ�ɹ���0 ��ʾʧ�ܣ�ͨ�ų�ʱ�򱻾ܾ���
*********************************************************************************************************
*/
uint8_t MODH_WriteParam_06H(uint16_t _reg, uint16_t _value,uint16_t _address)
{
    uint8_t i, res;
    for (i = 0; i < NUM; i++)
    {
        MODH_Send06H (_address, _reg, _value);
        while (1)               // �ȴ�Ӧ��,��ʱ����յ�Ӧ����break
        {
            res = MODH_Poll();
            if (res  == 0)
            {
                break;      // ͨ�ų�ʱ��
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
        return 1;   // д��06H�����ɹ�
    }
}
/*******************************************************************************************************/
