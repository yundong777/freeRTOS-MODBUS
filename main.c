/*
*********************************************************************************************************
*
*   ģ������ : ������ģ�顣
*   �ļ����� : main.c
*   ��    �� : V1.0
*********************************************************************************************************
*/
#include "includes.h"
#include "MainTask.h"
/*
**********************************************************************************************************
                                            ��������
**********************************************************************************************************
*/
static void vTaskGUI(void *pvParameters);
static void vTaskAD(void *pvParameters);
static void vTaskAirCompress(void *pvParameters);
static void vTaskUSB(void *pvParameters);
static void vTaskStart(void *pvParameters);
static void AppTaskCreate (void);
static void AppObjCreate (void);
/*
**********************************************************************************************************
                                            ��������
**********************************************************************************************************
*/
static TaskHandle_t xHandleTaskAD = NULL;
static TaskHandle_t xHandleTaskAirCompress = NULL;
static TaskHandle_t xHandleTaskUSB = NULL;
static TaskHandle_t xHandleTaskStart = NULL;
static TaskHandle_t xHandleTaskModbus = NULL;
static TaskHandle_t xHandleTaskError = NULL;
SemaphoreHandle_t  xMutex = NULL;       //spi������
SemaphoreHandle_t  queueMutex = NULL;   //���л�����
EventGroupHandle_t xCreatedEventGroup = NULL;
uint32_t PARAM_VER = 0;     //�汾��
/*
*********************************************************************************************************
*   �� �� ��: main
*   ����˵��: ��׼c������ڡ�
*   ��    �Σ���
*   �� �� ֵ: ��
*********************************************************************************************************
*/
int main(void)
{
    __set_PRIMASK(1);
    /* Ӳ����ʼ�� */
    bsp_Init();
    vSetupSysInfoTest();
    /* �������� */
    AppTaskCreate();
    /* ��������ͨ�Ż��� */
    AppObjCreate();
    /* �������ȣ���ʼִ������ */
    vTaskStartScheduler();
    while(1)
    {
        ;
    }
}
/*
*********************************************************************************************************
*   �� �� ��: vTaskGUI
*   ����˵��: emWin����
*   ��    ��: pvParameters ���ڴ���������ʱ���ݵ��β�
*   �� �� ֵ: ��
*   �� �� ��: 1   (��ֵԽС���ȼ�Խ�ͣ������uCOS�෴)
*********************************************************************************************************
*/
static void vTaskGUI(void *pvParameters)
{
    while (1)
    {
        MainTask();
    }
}
/*
*********************************************************************************************************
*   �� �� ��: vTaskAD
*   ����˵��: ��Ƶ��������ʼ����AD�ɼ���RTCʱ�Ӹ���
*   ��    ��: pvParameters ���ڴ���������ʱ���ݵ��β�
*   �� �� ֵ: ��
*   �� �� ��: 2
*********************************************************************************************************
*/
static void vTaskAD(void *pvParameters)
{
    while(1)
    {
        if(functionCodeVFDInitFlag == 0)        //��Ƶ��������ʼ��
        {
            airCompressInit();
            functionCodeVFDInit();
            vTaskDelay(1);
        }
        else        //RTCʱ�Ӹ���
        {
            RTC_GetDate(RTC_Format_BIN, &RTC_DateStructure);
            RTC_GetTime(RTC_Format_BIN, &RTC_TimeStructure);
            RTC_CalcWeek(RTC_DateStructure.RTC_Year,RTC_DateStructure.RTC_Month,RTC_DateStructure.RTC_Date);
        }
        AD_Update();
        vTaskDelay(1);
    }
}
/*
*********************************************************************************************************
*   �� �� ��: vTaskAirCompress
*   ����˵��: ��ѹ���߼�����
*   ��    ��: pvParameters ���ڴ���������ʱ���ݵ��β�
*   �� �� ֵ: ��
*   �� �� ��: 3
*********************************************************************************************************
*/
static void vTaskAirCompress(void *pvParameters)
{
    while(1)
    {
        vTaskDelay(100);
        if(functionCodeVFDInitFlag == 1)
        {
            airCompressControl();
        }
    }
}
/*
*********************************************************************************************************
*   �� �� ��: vTaskUSB
*   ����˵��: U�����ļ���������
*   ��    ��: pvParameters ���ڴ���������ʱ���ݵ��β�
*   �� �� ֵ: ��
*   �� �� ��: 4
*********************************************************************************************************
*/
extern void USBFatFS(void);
static void vTaskUSB(void *pvParameters)
{
    while(1)
    {
        USBFatFS();
    }
}
/*
*********************************************************************************************************
*   �� �� ��: vTaskModbus
*   ����˵��: Modbus ����
*   ��    ��: pvParameters ���ڴ���������ʱ���ݵ��β�
*   �� �� ֵ: ��
*   �� �� ��: 6
*********************************************************************************************************
*/
static void vTaskModbus(void *pvParameters)
{
    uint16_t sendBuf[4];    //0:���� 1�����ݵ�ַ 2������ 3:�豸��ַ
    int ret,ret1 = 2;
    while(1)
    {
        ret = dequeue(sendBuf);     //����
        if(ret == 0 && functionCodeVFDInitFlag == 1)    //����ʧ��
        {
            sendBuf[3] = funcCode.code.address / 100;
            if(converterDebug == 0 && allParameterStart == 0)
            {
                if(runState == 1)
                {
                    //��ͣ����
                    sendBuf[0] = 6;
                    sendBuf[1] = 0x2000;
                    sendBuf[2] = 1;
                    enqueue(sendBuf);
                    //Ƶ�ʸ���
                    sendBuf[0] = 6;
                    sendBuf[1] = 0x1000;
                    sendBuf[2] = pid_pressure.Out / 10;
                    enqueue(sendBuf);
                }
                else
                {
                    //ͣ������
                    sendBuf[0] = 6;
                    sendBuf[1] = 0x2000;
                    sendBuf[2] = 5;
                    enqueue(sendBuf);
                }
                converterState = 0;
            }
            else
            {
                vTaskDelay(500);
                sendBuf[0] = 3;
                sendBuf[1] = 0xE127;
                sendBuf[2] = 1;
                enqueue(sendBuf);
            }
            //����
            sendBuf[0] = 3;
            sendBuf[1] = 0x8000;
            sendBuf[2] = 1;
            enqueue(sendBuf);
            //״̬
            sendBuf[0] = 3;
            sendBuf[1] = 0xE100;
            sendBuf[2] = 7;
            enqueue(sendBuf);
            if(funcCode.code.runMethod == 100)
            {
                sendBuf[3] = funcCode.code.fanAddress / 100;
                //��ͣ����
                if(fanState == 1)
                {
                    sendBuf[0] = 6;
                    sendBuf[1] = 0x2000;
                    sendBuf[2] = 1;
                    enqueue(sendBuf);
                    //Ƶ�ʸ���
                    sendBuf[0] = 6;
                    sendBuf[1] = 0x1000;
                    sendBuf[2] = pid_temperature.Out / 10;
                    enqueue(sendBuf);
                }
                else
                {
                    sendBuf[0] = 6;
                    sendBuf[1] = 0x2000;
                    sendBuf[2] = 5;
                    enqueue(sendBuf);
                }
                //����
                sendBuf[0] = 3;
                sendBuf[1] = 0x8000;
                sendBuf[2] = 1;
                enqueue(sendBuf);
                //״̬
                sendBuf[0] = 3;
                sendBuf[1] = 0x1001;
                sendBuf[2] = 7;
                enqueue(sendBuf);
            }
            ret = dequeue(sendBuf);
        }
        if(ret == 1)    //���ӳɹ�
        {
            switch (sendBuf[0])
            {
            case 3:
                ret1 = MODH_ReadParam_03H(sendBuf[1], sendBuf[2],sendBuf[3]);
                break;
            case 6:
                ret1 = MODH_WriteParam_06H(sendBuf[1], sendBuf[2],sendBuf[3]);
                break;
            }
        }
        if(ret1 == 1)   //ͨ�ųɹ�
        {
            if(sendBuf[3] == funcCode.code.address / 100)
            {
                if(communicationError > 0)
                {
                    communicationError --;
                }
            }
            else
            {
                if(fanCommunicationError > 0)
                {
                    fanCommunicationError --;
                }
            }
            if(sendBuf[0] == 6 && sendBuf[1] == 0xA403) //������ʼ�������¶�ȡ��Ƶ������
            {
                functionCodeVFDInitFlag = 0;
            }
        }
        else if(ret1 == 0)      //ͨ��ʧ��
        {
            if(sendBuf[3] == funcCode.code.address / 100)
            {
                communicationError ++;
                if(communicationError > 50)
                {
                    communicationError = 50;
                }
            }
            else
            {
                fanCommunicationError ++;
                if(fanCommunicationError > 50)
                {
                    fanCommunicationError = 50;
                }
            }
        }
        ret1 = 2;
        vTaskDelay(10);
    }
}
/*
*********************************************************************************************************
*   �� �� ��: vTaskError
*   ����˵��: ���ϼ��
*   ��    ��: pvParameters ���ڴ���������ʱ���ݵ��β�
*   �� �� ֵ: ��
*   �� �� ��: 7
*********************************************************************************************************
*/
static void vTaskError(void *pvParameters)
{
    while(1)
    {
        if(functionCodeVFDInitFlag == 0)
        {
            vTaskDelay(10);
        }
        else
        {
            vTaskDelay(1000);
            break;
        }
    }
    while(1)
    {
        if(functionCodeVFDInitFlag == 1)
        {
            //��ͣ���� �Զ���λ
            if(errorFlag == 11)
            {
                if(IsEmergencyON() == 1)
                {
                    vTaskDelay(5);
                    if(IsEmergencyON() == 1)
                    {
                        errorFlag = 0;
                    }
                }
            }
            if(errorHiddenFlag == 0)
            {
                if(errorFlag == 0)
                {
                    errorHandle();      //���ϴ���
                    warningHandle();    //��������
                }
            }
        }
        vTaskDelay(100);
    }
}
/*
*********************************************************************************************************
*   �� �� ��: vTaskStart
*   ����˵��: ��������Ҳ����������ȼ�������Ҫʵ�ְ������ʹ�����⡣
*   ��    ��: pvParameters ���ڴ���������ʱ���ݵ��β�
*   �� �� ֵ: ��
*   �� �� ��: 9
*********************************************************************************************************
*/
static void vTaskStart(void *pvParameters)
{
    uint8_t  ucCount = 0;
    uint8_t  ucCount1 = 0;
    while(1)
    {
        /* 1msһ�δ���ɨ�裬���败���� */
        if(g_tTP.Enable == 1)
        {
            TOUCH_Scan();
        }
        vTaskDelay(1);
        ucCount1++;
        if(ucCount1 == 10)
        {
            ucCount1 = 0;
            BEEP_Pro();     //��������ʱ
            guiSleepTime++;
            if(guiSleepTime > 18000 && guiSleepFlag == 1)
            {
                guiSleepFlag = 0;
                LCD_SetBackLight(0);     // �رձ���
            }
            if(guiSleepTime < 1000 && guiSleepFlag == 0)
            {
                guiSleepFlag = 1;
                LCD_SetBackLight(255*funcCode.code.backlight/500);   // �򿪱���
            }
        }
        if(functionCodeVFDInitFlag == 1)
        {
            phaseAndLoseTest();
        }
        if(startPressedTime > 0)
        {
            startPressedTime ++;
        }
        else
        {
            startPressedTime = 0;
        }
        WWDG_SetCounter( 80 );
    }
}
/*
*********************************************************************************************************
*   �� �� ��: AppTaskCreate
*   ����˵��: ����Ӧ������
*   ��    �Σ���
*   �� �� ֵ: ��
*********************************************************************************************************
*/
static void AppTaskCreate (void)
{
    xTaskCreate(  vTaskGUI,             /* ������  */
                  "vTaskGUI",           /* ������    */
                  1024,                 /* ����ջ��С����λword��Ҳ����4�ֽ� */
                  NULL,                 /* �������  */
                  1,                    /* �������ȼ�*/
                  NULL );               /* ������  */
    xTaskCreate( vTaskAD,               /* ������  */
                 "vTaskAD",             /* ������    */
                 512,                   /* ����ջ��С����λword��Ҳ����4�ֽ� */
                 NULL,                  /* �������  */
                 2,                     /* �������ȼ�*/
                 &xHandleTaskAD );      /* ������  */
    xTaskCreate( vTaskAirCompress,      /* ������  */
                 "vTaskAirCompress",    /* ������    */
                 512,                   /* stack��С����λword��Ҳ����4�ֽ� */
                 NULL,                  /* �������  */
                 3,                     /* �������ȼ�*/
                 &xHandleTaskAirCompress ); /* ������  */
    xTaskCreate( vTaskUSB,              /* ������  */
                 "vTaskUSB",            /* ������    */
                 2048,                  /* ����ջ��С����λword��Ҳ����4�ֽ� */
                 NULL,                  /* �������  */
                 4,                     /* �������ȼ�*/
                 &xHandleTaskUSB );     /* ������  */
    xTaskCreate( vTaskModbus,           /* ������  */
                 "vTaskModbus",         /* ������    */
                 512,                   /* ����ջ��С����λword��Ҳ����4�ֽ� */
                 NULL,                  /* �������  */
                 6,                     /* �������ȼ�*/
                 &xHandleTaskModbus );  /* ������  */
    xTaskCreate( vTaskError,            /* ������  */
                 "vTaskError",          /* ������    */
                 512,                   /* ����ջ��С����λword��Ҳ����4�ֽ� */
                 NULL,                  /* �������  */
                 7,                     /* �������ȼ�*/
                 &xHandleTaskError );   /* ������  */
    xTaskCreate( vTaskStart,            /* ������  */
                 "vTaskStart",          /* ������    */
                 512,                   /* ����ջ��С����λword��Ҳ����4�ֽ� */
                 NULL,                  /* �������  */
                 9,                     /* �������ȼ�*/
                 &xHandleTaskStart );   /* ������  */
}
/*
*********************************************************************************************************
*   �� �� ��: AppObjCreate
*   ����˵��: ��������ͨ�Ż���
*   ��    ��: ��
*   �� �� ֵ: ��
*********************************************************************************************************
*/
static void AppObjCreate (void)
{
    /* ���������ź��� */
    xMutex = xSemaphoreCreateMutex();
    if(xMutex == NULL)
    {
        errorFlag = 1000;
    }
    /* ���������ź��� */
    queueMutex = xSemaphoreCreateMutex();
    if(queueMutex == NULL)
    {
        errorFlag = 1001;
    }
    /* �����¼���־�� */
    xCreatedEventGroup = xEventGroupCreate();
    if(xCreatedEventGroup == NULL)
    {
        errorFlag = 1002;
    }
    {
    }
}
/********************************************************************************************************/
