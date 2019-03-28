/*
*********************************************************************************************************
*
*   模块名称 : 主程序模块。
*   文件名称 : main.c
*   版    本 : V1.0
*********************************************************************************************************
*/
#include "includes.h"
#include "MainTask.h"
/*
**********************************************************************************************************
                                            函数声明
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
                                            变量声明
**********************************************************************************************************
*/
static TaskHandle_t xHandleTaskAD = NULL;
static TaskHandle_t xHandleTaskAirCompress = NULL;
static TaskHandle_t xHandleTaskUSB = NULL;
static TaskHandle_t xHandleTaskStart = NULL;
static TaskHandle_t xHandleTaskModbus = NULL;
static TaskHandle_t xHandleTaskError = NULL;
SemaphoreHandle_t  xMutex = NULL;       //spi互斥锁
SemaphoreHandle_t  queueMutex = NULL;   //队列互斥锁
EventGroupHandle_t xCreatedEventGroup = NULL;
uint32_t PARAM_VER = 0;     //版本号
/*
*********************************************************************************************************
*   函 数 名: main
*   功能说明: 标准c程序入口。
*   形    参：无
*   返 回 值: 无
*********************************************************************************************************
*/
int main(void)
{
    __set_PRIMASK(1);
    /* 硬件初始化 */
    bsp_Init();
    vSetupSysInfoTest();
    /* 创建任务 */
    AppTaskCreate();
    /* 创建任务通信机制 */
    AppObjCreate();
    /* 启动调度，开始执行任务 */
    vTaskStartScheduler();
    while(1)
    {
        ;
    }
}
/*
*********************************************************************************************************
*   函 数 名: vTaskGUI
*   功能说明: emWin任务
*   形    参: pvParameters 是在创建该任务时传递的形参
*   返 回 值: 无
*   优 先 级: 1   (数值越小优先级越低，这个跟uCOS相反)
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
*   函 数 名: vTaskAD
*   功能说明: 变频器参数初始化、AD采集、RTC时钟更新
*   形    参: pvParameters 是在创建该任务时传递的形参
*   返 回 值: 无
*   优 先 级: 2
*********************************************************************************************************
*/
static void vTaskAD(void *pvParameters)
{
    while(1)
    {
        if(functionCodeVFDInitFlag == 0)        //变频器参数初始化
        {
            airCompressInit();
            functionCodeVFDInit();
            vTaskDelay(1);
        }
        else        //RTC时钟更新
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
*   函 数 名: vTaskAirCompress
*   功能说明: 空压机逻辑控制
*   形    参: pvParameters 是在创建该任务时传递的形参
*   返 回 值: 无
*   优 先 级: 3
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
*   函 数 名: vTaskUSB
*   功能说明: U盘中文件处理和浏览
*   形    参: pvParameters 是在创建该任务时传递的形参
*   返 回 值: 无
*   优 先 级: 4
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
*   函 数 名: vTaskModbus
*   功能说明: Modbus 发送
*   形    参: pvParameters 是在创建该任务时传递的形参
*   返 回 值: 无
*   优 先 级: 6
*********************************************************************************************************
*/
static void vTaskModbus(void *pvParameters)
{
    uint16_t sendBuf[4];    //0:命令 1：数据地址 2：数据 3:设备地址
    int ret,ret1 = 2;
    while(1)
    {
        ret = dequeue(sendBuf);     //出队
        if(ret == 0 && functionCodeVFDInitFlag == 1)    //出队失败
        {
            sendBuf[3] = funcCode.code.address / 100;
            if(converterDebug == 0 && allParameterStart == 0)
            {
                if(runState == 1)
                {
                    //启停命令
                    sendBuf[0] = 6;
                    sendBuf[1] = 0x2000;
                    sendBuf[2] = 1;
                    enqueue(sendBuf);
                    //频率给定
                    sendBuf[0] = 6;
                    sendBuf[1] = 0x1000;
                    sendBuf[2] = pid_pressure.Out / 10;
                    enqueue(sendBuf);
                }
                else
                {
                    //停机命令
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
            //故障
            sendBuf[0] = 3;
            sendBuf[1] = 0x8000;
            sendBuf[2] = 1;
            enqueue(sendBuf);
            //状态
            sendBuf[0] = 3;
            sendBuf[1] = 0xE100;
            sendBuf[2] = 7;
            enqueue(sendBuf);
            if(funcCode.code.runMethod == 100)
            {
                sendBuf[3] = funcCode.code.fanAddress / 100;
                //启停命令
                if(fanState == 1)
                {
                    sendBuf[0] = 6;
                    sendBuf[1] = 0x2000;
                    sendBuf[2] = 1;
                    enqueue(sendBuf);
                    //频率给定
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
                //故障
                sendBuf[0] = 3;
                sendBuf[1] = 0x8000;
                sendBuf[2] = 1;
                enqueue(sendBuf);
                //状态
                sendBuf[0] = 3;
                sendBuf[1] = 0x1001;
                sendBuf[2] = 7;
                enqueue(sendBuf);
            }
            ret = dequeue(sendBuf);
        }
        if(ret == 1)    //出队成功
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
        if(ret1 == 1)   //通信成功
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
            if(sendBuf[0] == 6 && sendBuf[1] == 0xA403) //参数初始化后重新读取变频器参数
            {
                functionCodeVFDInitFlag = 0;
            }
        }
        else if(ret1 == 0)      //通信失败
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
*   函 数 名: vTaskError
*   功能说明: 故障检测
*   形    参: pvParameters 是在创建该任务时传递的形参
*   返 回 值: 无
*   优 先 级: 7
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
            //急停故障 自动复位
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
                    errorHandle();      //故障处理
                    warningHandle();    //报警处理
                }
            }
        }
        vTaskDelay(100);
    }
}
/*
*********************************************************************************************************
*   函 数 名: vTaskStart
*   功能说明: 启动任务，也就是最高优先级任务。主要实现按键检测和触摸检测。
*   形    参: pvParameters 是在创建该任务时传递的形参
*   返 回 值: 无
*   优 先 级: 9
*********************************************************************************************************
*/
static void vTaskStart(void *pvParameters)
{
    uint8_t  ucCount = 0;
    uint8_t  ucCount1 = 0;
    while(1)
    {
        /* 1ms一次触摸扫描，电阻触摸屏 */
        if(g_tTP.Enable == 1)
        {
            TOUCH_Scan();
        }
        vTaskDelay(1);
        ucCount1++;
        if(ucCount1 == 10)
        {
            ucCount1 = 0;
            BEEP_Pro();     //蜂鸣器计时
            guiSleepTime++;
            if(guiSleepTime > 18000 && guiSleepFlag == 1)
            {
                guiSleepFlag = 0;
                LCD_SetBackLight(0);     // 关闭背光
            }
            if(guiSleepTime < 1000 && guiSleepFlag == 0)
            {
                guiSleepFlag = 1;
                LCD_SetBackLight(255*funcCode.code.backlight/500);   // 打开背光
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
*   函 数 名: AppTaskCreate
*   功能说明: 创建应用任务
*   形    参：无
*   返 回 值: 无
*********************************************************************************************************
*/
static void AppTaskCreate (void)
{
    xTaskCreate(  vTaskGUI,             /* 任务函数  */
                  "vTaskGUI",           /* 任务名    */
                  1024,                 /* 任务栈大小，单位word，也就是4字节 */
                  NULL,                 /* 任务参数  */
                  1,                    /* 任务优先级*/
                  NULL );               /* 任务句柄  */
    xTaskCreate( vTaskAD,               /* 任务函数  */
                 "vTaskAD",             /* 任务名    */
                 512,                   /* 任务栈大小，单位word，也就是4字节 */
                 NULL,                  /* 任务参数  */
                 2,                     /* 任务优先级*/
                 &xHandleTaskAD );      /* 任务句柄  */
    xTaskCreate( vTaskAirCompress,      /* 任务函数  */
                 "vTaskAirCompress",    /* 任务名    */
                 512,                   /* stack大小，单位word，也就是4字节 */
                 NULL,                  /* 任务参数  */
                 3,                     /* 任务优先级*/
                 &xHandleTaskAirCompress ); /* 任务句柄  */
    xTaskCreate( vTaskUSB,              /* 任务函数  */
                 "vTaskUSB",            /* 任务名    */
                 2048,                  /* 任务栈大小，单位word，也就是4字节 */
                 NULL,                  /* 任务参数  */
                 4,                     /* 任务优先级*/
                 &xHandleTaskUSB );     /* 任务句柄  */
    xTaskCreate( vTaskModbus,           /* 任务函数  */
                 "vTaskModbus",         /* 任务名    */
                 512,                   /* 任务栈大小，单位word，也就是4字节 */
                 NULL,                  /* 任务参数  */
                 6,                     /* 任务优先级*/
                 &xHandleTaskModbus );  /* 任务句柄  */
    xTaskCreate( vTaskError,            /* 任务函数  */
                 "vTaskError",          /* 任务名    */
                 512,                   /* 任务栈大小，单位word，也就是4字节 */
                 NULL,                  /* 任务参数  */
                 7,                     /* 任务优先级*/
                 &xHandleTaskError );   /* 任务句柄  */
    xTaskCreate( vTaskStart,            /* 任务函数  */
                 "vTaskStart",          /* 任务名    */
                 512,                   /* 任务栈大小，单位word，也就是4字节 */
                 NULL,                  /* 任务参数  */
                 9,                     /* 任务优先级*/
                 &xHandleTaskStart );   /* 任务句柄  */
}
/*
*********************************************************************************************************
*   函 数 名: AppObjCreate
*   功能说明: 创建任务通信机制
*   形    参: 无
*   返 回 值: 无
*********************************************************************************************************
*/
static void AppObjCreate (void)
{
    /* 创建互斥信号量 */
    xMutex = xSemaphoreCreateMutex();
    if(xMutex == NULL)
    {
        errorFlag = 1000;
    }
    /* 创建互斥信号量 */
    queueMutex = xSemaphoreCreateMutex();
    if(queueMutex == NULL)
    {
        errorFlag = 1001;
    }
    /* 创建事件标志组 */
    xCreatedEventGroup = xEventGroupCreate();
    if(xCreatedEventGroup == NULL)
    {
        errorFlag = 1002;
    }
    {
    }
}
/********************************************************************************************************/
