/*
*********************************************************************************************************
*
*   模块名称 : 变频器参数
*   文件名称 : function_FunctionCode.h
*   版    本 : V1.0
*   说    明 : 头文件
*
*********************************************************************************************************
*/
#ifndef _F_FUNCTIONCODE_H_
#define _F_FUNCTIONCODE_H_
#include "stdint.h"
// EEPROM校验字
#define EEPROM_CHECK_1       (SOFTWARE_VERSION)              // EEPROM校验字1   与软件版本有关
#define EEPROM_CHECK_2       (FNUM_EEPROM)                   // EEPROM校验字2  所有要保存的功能码个数
// 功能码索引以及功能码组索引，得到的值为功能码的顺序值
#define CodeSerialNum(codeVFD)           ((uint16_t)((&(codeVFD)) - (&(FunctionCodeVFD.allVFD[0]))))
#define GradeSerialNum(group, grade)  (FunctionCodeGradeSum[group] + (grade))
//功能码的属性
// 单位bit3-5
#define ATTRIBUTE_UNIT_HZ_BIT           0
#define ATTRIBUTE_UNIT_A_BIT            1
#define ATTRIBUTE_UNIT_V_BIT            2
// 读写属性 bit 11-12
#define ATTRIBUTE_READ_AND_WRITE        0   // (任何时候)可写
#define ATTRIBUTE_READ_ONLY_WHEN_RUN    1   // 运行时只读
#define ATTRIBUTE_READ_ONLY_ANYTIME     2   // 只读
// 多个功能码的组合属性，跟上下限有关
#define ATTRIBUTE_MULTI_LIMIT_SINGLE    0   // 单独的功能码
#define ATTRIBUTE_MULTI_LIMIT_DEC       1   // 多个功能码，十进制
#define ATTRIBUTE_MULTI_LIMIT_HEX       2   // 多个功能码，十六进制
extern const uint16_t FunctionCodeGradeSum[];
struct  FUNC_ATTRIBUTE_BITS
{
    // bit    功能
    uint16_t point:3;         // 2-0    000：无小数点 001：1位小数，...，100：4位小数
    uint16_t unit:3;          // 5:3    000：无单位，001：hz,:010：A, 011：RPM, 100：V, 110-%;
    uint16_t displayBits:3;   // 8:6    000：显示0位，001：显示1位，...，101：显示5位
    uint16_t upperLimit:1;    // 9      1-参数由上限相关功能码限制，0-直接由上限限制
    uint16_t lowerLimit:1;    // 10     1-参数由下限相关功能码限制，0-直接由下限限制
    uint16_t writable:2;      // 12:11  00：可以读写, 01：运行中只读，10：参数只读
    uint16_t signal:1;        // 13     0：无符号; 1：有符号
    uint16_t multiLimit:2;    // 15:14  00-单独功能码(非组合);
    //        01-十进制,  多个功能码的组合;
    //        10-十六进制,多个功能码的组合;
};
union FUNC_ATTRIBUTE
{
    uint16_t                    allVFD;
    struct FUNC_ATTRIBUTE_BITS  bitVFD;
};
//=====================================================================
// 功能码属性表：上限、下限、出厂值、属性
//
typedef struct FUNCCODE_ATTRIBUTE_STRUCT_VFD
{
    uint16_t                  lower;          // 下限
    uint16_t                  upper;          // 上限
    uint16_t                  init;           // 出厂值
    union FUNC_ATTRIBUTE      attribute;      // 属性
} FUNCCODE_ATTRIBUTE_VFD;
extern const FUNCCODE_ATTRIBUTE_VFD funcCodeAttributeVFD[];
struct PLC_STRUCT           //PLC功能用的结构体
{
    uint16_t runTime;         // PLC第_段运行时间
    uint16_t accDecTimeSet;   // PLC第_段加减速时间选择
};
struct AI_SET_CURVE  // AI设定曲线
{
    uint16_t minIn;       // 曲线最小输入(电压值)
    uint16_t minInSet;    // 曲线最小输入对应设定(百分比)
    uint16_t maxIn;       // 曲线最大输入(电压值)
    uint16_t maxInSet;    // 曲线最大输入对应设定(百分比)
};
struct AI_JUMP    //模拟量跳跃
{
    uint16_t point;   // 设定跳跃点
    uint16_t arrange; // 设定跳跃幅度
};
struct ANALOG_CALIBRATE_CURVE  // 模拟量校正曲线，AIAO
{
    uint16_t before1;     // 校正前电压1
    uint16_t after1;      // 校正后电压1
    uint16_t before2;     // 校正前电压2
    uint16_t after2;      // 校正后电压2
};
typedef struct AO_PARA_STRUCT  //AO输出矫正
{
    uint16_t offset;          // AO零偏系数
    uint16_t gain;            // AO增益
} AO_PARA;
struct ERROR_SCENE_STRUCT
{
    uint16_t errorFrq;                    // 第三次(最近一次)故障时频率
    uint16_t errorCurrent;                // 第三次(最近一次)故障时电流
    uint16_t errorGeneratrixVoltage;      // 第三次(最近一次)故障时母线电压
    uint16_t errorDiStatus;               // 第三次(最近一次)故障时输入端子状态
    uint16_t errorTemp;                   // 第三次(最近一次)故障时变频器模块温度
    uint16_t errorInverterStatus;         // 第三次(最近一次)故障时变频器状态
    uint16_t errorTimeFromPowerUp;        // 第三次(最近一次)故障时时间（从本次上电开始计时）
    uint16_t errorTimeFromRun;            // 第三次(最近一次)故障时时间（从运行时开始计时）
};
union ERROR_SCENE
{
    uint16_t allVFD[sizeof(struct ERROR_SCENE_STRUCT)/2];
    struct ERROR_SCENE_STRUCT elem;
};
//=================================
#define MOTOR_TYPE_ACI_GENERAL  0   // 普通异步电机
#define MOTOR_TYPE_ACI_INV      1   // 变频异步电机
#define MOTOR_TYPE_PMSM         2   // 永磁同步电机
//------------------------------------------------
struct VC_PARA
{
    uint16_t vcSpdLoopKp1;            //   速度环比例增益1
    uint16_t vcSpdLoopTi1;            //   速度环积分时间1
    uint16_t vcSpdLoopChgFrq1;        //   切换频率1
    uint16_t vcSpdLoopKp2;            //   速度环比例增益2
    uint16_t vcSpdLoopTi2;            //   速度环积分时间2
    uint16_t vcSpdLoopChgFrq2;        //   切换频率2
    uint16_t vcSlipCompCoef;          //   转差补偿系数
    uint16_t vcSpdLoopFilterTime;     //  速度环滤波时间常数
    uint16_t vcOverMagGain;           //   矢量控制过励磁增益
    uint16_t spdCtrlDriveTorqueLimitSrc;  //   速度控制(驱动)转矩上限源
    uint16_t spdCtrlDriveTorqueLimit;     //   速度控制(驱动)转矩上限数字设定
    uint16_t spdCtrlBrakeTorqueLimitSrc;  //   速度控制(制动)转矩上限源
    uint16_t spdCtrlBrakeTorqueLimit;     //  速度控制(制动)转矩上限数字设定
    uint16_t mAcrKp;                  //   M轴电流环Kp
    uint16_t mAcrKi;                  //   M轴电流环Ki
    uint16_t tAcrKp;                  //   T轴电流环Kp
    uint16_t tAcrKi;                  //    T轴电流环Ki
    uint16_t spdLoopI;                //   速度环积分属性
    uint16_t weakFlusMode;            // 同步机弱磁模式
    uint16_t weakFlusCoef;            // 同步机弱磁系数
    uint16_t weakFlusCurMax;          //  最大弱磁电流
    uint16_t weakFlusAutoCoef;        // 弱磁自动调谐系数
    uint16_t weakFlusIntegrMul;       //  弱磁积分倍数
};
//------------------------------------------------
// 电机参数，包括电机参数、控制参数
struct MOTOR_FC
{
    // 电机基本参数
    uint16_t motorType;               //   电机类型选择
    uint16_t ratingPower;             //   电机额定功率
    uint16_t ratingVoltage;           //   电机额定电压
    uint16_t ratingCurrent;           //   电机额定电流
    uint16_t ratingFrq;               //   电机额定频率
    uint16_t ratingSpeed;             //   电机额定转速
    // 同步机调谐参数
    uint16_t pmsmRs;                  //   同步机定子电阻
    uint16_t pmsmLd;                  //   同步机d轴电感
    uint16_t pmsmLq;                  //   同步机q轴电感
    uint16_t pmsmInitPWMTs;            //   同步机转子位置检测脉宽
    uint16_t pmsmCoeff;               //  同步机反电动势系数
    uint16_t pmsmCheckTime;           //   同步机
    // PG卡参数
    uint16_t fvcPgSrc;                //     速度反馈PG卡选择, 0-QEP1,1-QEP2(扩展)
    uint16_t pgType;                  //     编码器类型
    uint16_t enCoderDir;              //     编码器相序/主方向
    uint16_t encoderPulse;            //     编码器脉冲线数
    uint16_t enCoderAngle;            //     编码器安装角
    uint16_t uvwSignDir;              //    UVW信号方向
    uint16_t uvwSignAngle;            //    UVW信号零点位置角
    uint16_t enCoderPole;             //   旋变极对数
    uint16_t rsvdF1_35;                //     UVW极对数
    uint16_t fvcPgLoseTime;           //    速度反馈PG断线检测时间
    uint16_t tuneCmd;                 // 调谐命令
    struct VC_PARA vcPara;          // VC控制参数
    uint16_t MotorCtrlMode;           // 电机控制方式
    uint16_t accDecTimeMotor;         // 电机加减速时间选择
};
//------------------------------------------------/
#define AI_NUMBER               3           // AI端子个数
#define AO_NUMBER               2           // AO端子个数
#define HDI_NUMBER              1           // HDI端子个数
#define HDO_NUMBER              1           // HDO端子个数
#define DI_NUMBER               10          // 物理DI端子
#define DI_FUNC_NUMBER          49          // DI的功能选择数量
#define DO_NUMBER               5           // 物理DO端子
#define PLC_STEP_MAX            16          // PLC、多段指令段数
//=====================================================================
#define EEPROM_USE_LENGTH        FNUM_EEPROM    //所有要保存的功能码的长度也是eeprom的使用长度
#define EEPROM_INDEX_USE_INDEX        4     // 功能参数使用EEPROM起始
// 预留给其它使用的功能码index
#define FUNCCODE_RSVD4ALL_INDEX     CodeSerialNum(FunctionCodeVFD.codeVFD.rsvd4All)
#define REM_P_OFF_MOTOR_NUMBER     5   // 性能掉电记忆的参数个数
// 功能码的组数
#define FUNCCODE_GROUP_NUMBER  41  // 包括预留的组
// EEPROM中顺序: EEPROM_CHK, 其他顺序保存
//每组的组号
#define FUNCCODE_GROUP_A0   0       // A0组
#define FUNCCODE_GROUP_AE   7       // AE组
#define FUNCCODE_GROUP_AF   8       // AF组
#define FUNCCODE_GROUP_B0   9       // B0组
#define FUNCCODE_GROUP_BF   17      // BF组
#define FUNCCODE_GROUP_C0   18      // C0组
#define FUNCCODE_GROUP_CF   26      // CF组
#define FUNCCODE_GROUP_D0   27      // D0组
#define FUNCCODE_GROUP_D5   32      // D5组
#define FUNCCODE_GROUP_DF   35      // DF组
#define FUNCCODE_GROUP_U0   36      // U0组   故障显示
//这里有 两个组，为eeprom校验和掉电保存
#define FUNCCODE_GROUP_U1   39       // H0组，显示
#define FUNCCODE_GROUP_U2   40      // U1组   性能显示
//以下为现在使用的功能码个数定义，不包括保留的
#define A0PATAMETERNUM           (26)     // A0
#define A1PATAMETERNUM           (25)     // A1
#define A2PATAMETERNUM           (21)     // A2
#define A3PATAMETERNUM           (36)     // A3
#define A4PATAMETERNUM           (11)     // A4
#define A5PATAMETERNUM           (13)     // A5
#define A6PATAMETERNUM           (0 )     // A6
#define AEPATAMETERNUM           (23)     // AE
#define AFPATAMETERNUM           (20)     // AF
#define B0PATAMETERNUM           (31)     // B0
#define B1PATAMETERNUM           (19)     // B1
#define B2PATAMETERNUM           (53)     // B2
#define B3PATAMETERNUM           (9 )     // B3
#define B4PATAMETERNUM           (0 )     // B4
#define B5PATAMETERNUM           (0 )     // B5
#define B6PATAMETERNUM           (0 )     // B6
#define BEPATAMETERNUM           (0 )     // BE
#define BFPATAMETERNUM           (30)     // BF
#define C0PATAMETERNUM           (27)     // C0
#define C1PATAMETERNUM           (44)     // C1
#define C2PATAMETERNUM           (23)     // C2
#define C3PATAMETERNUM           (0 )     // C3
#define C4PATAMETERNUM           (29)     // C4
#define C5PATAMETERNUM           (0 )     // C5
#define C6PATAMETERNUM           (0 )     // C6
#define CEPATAMETERNUM           (0 )     // CE
#define CFPATAMETERNUM           (0 )     // CF
#define D0PATAMETERNUM           (29)     // D0
#define D1PATAMETERNUM           (52)     // D1
#define D2PATAMETERNUM           (0 )     // D2
#define D3PATAMETERNUM           (0 )     // D3
#define D4PATAMETERNUM           (0 )     // D4
#define D5PATAMETERNUM           (30)     // D5
#define D6PATAMETERNUM           (0 )     // D6
#define DEPATAMETERNUM           (0 )     // DE
#define DFPATAMETERNUM           (0 )     // DF
#define U0PATAMETERNUM           (35)     // U0
#define A0RSVDNUM              (10 )   // A0保留
#define A1RSVDNUM              (10 )   // A1保留
#define A2RSVDNUM              (10 )   // A2保留
#define A3RSVDNUM              (10 )   // A3保留
#define A4RSVDNUM              (10 )   // A4保留
#define A5RSVDNUM              (10 )   // A5保留
#define A6RSVDNUM              (10 )   // A6保留
#define AERSVDNUM              (10 )   // AE保留
#define AFRSVDNUM              (14 )   // AF保留
#define B0RSVDNUM              (0 )   // B0保留
#define B1RSVDNUM              (10 )   // B1保留
#define B2RSVDNUM              (10 )   // B2保留
#define B3RSVDNUM              (10 )   // B3保留
#define B4RSVDNUM              (10 )   // B4保留
#define B5RSVDNUM              (10 )   // B5保留
#define B6RSVDNUM              (10 )   // B6保留
#define BERSVDNUM              (10 )   // BE保留
#define BFRSVDNUM              (10 )   // BF保留
#define C0RSVDNUM              (10 )   // C0保留
#define C1RSVDNUM              (10 )   // C1保留
#define C2RSVDNUM              (10 )   // C2保留
#define C3RSVDNUM              (30 )   // C3保留
#define C4RSVDNUM              (10 )   // C4保留
#define C5RSVDNUM              (10 )   // C5保留
#define C6RSVDNUM              (10 )   // C6保留
#define CERSVDNUM              (10 )   // CE保留
#define CFRSVDNUM              (10 )   // CF保留
#define D0RSVDNUM              (10 )   // D0保留
#define D1RSVDNUM              (10 )   // D1保留
#define D2RSVDNUM              (10 )   // D2保留
#define D3RSVDNUM              (10 )   // D3保留
#define D4RSVDNUM              (10 )   // D4保留
#define D5RSVDNUM              (10 )   // D5保留
#define D6RSVDNUM              (10 )   // D6保留
#define DERSVDNUM              (10 )   // DE保留
#define DFRSVDNUM              (10 )   // DF保留
#define U0RSVDNUM              (10 )   // U0保留
// 以下为每个组的功能码个数，包括保留的
// 除FF组之外，每组预留5个功能码。为了在增加功能码时，尽量不用恢复出厂参数。
#define A0NUM           (A0PATAMETERNUM+ A0RSVDNUM  )   // A0
#define A1NUM           (A1PATAMETERNUM+ A1RSVDNUM  )   // A1
#define A2NUM           (A2PATAMETERNUM+ A2RSVDNUM  )   // A2
#define A3NUM           (A3PATAMETERNUM+ A3RSVDNUM  )   // A3
#define A4NUM           (A4PATAMETERNUM+ A4RSVDNUM  )   // A4
#define A5NUM           (A5PATAMETERNUM+ A5RSVDNUM  )   // A5
#define A6NUM           (A6PATAMETERNUM+ A6RSVDNUM  )   // A6
#define AENUM           (AEPATAMETERNUM+ AERSVDNUM  )   // AE
#define AFNUM           (AFPATAMETERNUM+ AFRSVDNUM  )   // AF
#define B0NUM           (B0PATAMETERNUM+ B0RSVDNUM  )   // B0
#define B1NUM           (B1PATAMETERNUM+ B1RSVDNUM  )   // B1
#define B2NUM           (B2PATAMETERNUM+ B2RSVDNUM  )   // B2
#define B3NUM           (B3PATAMETERNUM+ B3RSVDNUM  )   // B3
#define B4NUM           (B4PATAMETERNUM+ B4RSVDNUM  )   // B4
#define B5NUM           (B5PATAMETERNUM+ B5RSVDNUM  )   // B5
#define B6NUM           (B6PATAMETERNUM+ B6RSVDNUM  )   // B6
#define BENUM           (BEPATAMETERNUM+ BERSVDNUM  )   // BE
#define BFNUM           (BFPATAMETERNUM+ BFRSVDNUM  )   // BF
#define C0NUM           (C0PATAMETERNUM + C0RSVDNUM )   // C0
#define C1NUM           (C1PATAMETERNUM + C1RSVDNUM )   // C1
#define C2NUM           (C2PATAMETERNUM + C2RSVDNUM )   // C2
#define C3NUM           (C3PATAMETERNUM + C3RSVDNUM )   // C3
#define C4NUM           (C4PATAMETERNUM + C4RSVDNUM )   // C4
#define C5NUM           (C5PATAMETERNUM + C5RSVDNUM )   // C5
#define C6NUM           (C6PATAMETERNUM + C6RSVDNUM )   // C6
#define CENUM           (CEPATAMETERNUM + CERSVDNUM )   // CE
#define CFNUM           (CFPATAMETERNUM + CFRSVDNUM )   // CF
#define D0NUM           (D0PATAMETERNUM + D0RSVDNUM )   // D0
#define D1NUM           (D1PATAMETERNUM + D1RSVDNUM )   // D1
#define D2NUM           (D2PATAMETERNUM + D2RSVDNUM )   // D2
#define D3NUM           (D3PATAMETERNUM + D3RSVDNUM )   // D3
#define D4NUM           (D4PATAMETERNUM + D4RSVDNUM )   // D4
#define D5NUM           (D5PATAMETERNUM + D5RSVDNUM )   // D5
#define D6NUM           (D6PATAMETERNUM + D6RSVDNUM )   // D6
#define DENUM           (DEPATAMETERNUM + DERSVDNUM )   // DE
#define DFNUM           (DFPATAMETERNUM + DFRSVDNUM )   // DF
#define U0NUM           (U0PATAMETERNUM + U0RSVDNUM )   // U0
#define CHK_NUM         (4)        //   eeprom检查标记个数
#define REM_NUM         (48)       //   所有掉电记忆保存的功能码个数
#define U1NUM           (50)      //   U1显示参数组个数，不进行eeprom保存
#define U2NUM           (40)      //   U2显示参数组个数，不进行eeprom保存
//所有的功能码个数（不包括显示的）
#define FNUM_PARA      (A0NUM + A1NUM + A2NUM + A3NUM +     \
                        A4NUM + A5NUM + A6NUM + AENUM +     \
                        AFNUM +                             \
                                                            \
                        B0NUM + B1NUM + B2NUM + B3NUM +     \
                        B4NUM + B5NUM + B6NUM + BENUM +     \
                        BFNUM +                             \
                                                            \
                        C0NUM + C1NUM + C2NUM + C3NUM +     \
                        C4NUM + C5NUM + C6NUM + CENUM +     \
                        CFNUM +                             \
                                                            \
                        D0NUM + D1NUM + D2NUM + D3NUM +     \
                        D4NUM + D5NUM + D6NUM + DENUM +     \
                        DFNUM +                             \
                                                            \
                        U0NUM                               \
                        )
//所有需要保存在eeprom中的功能码个数
#define FNUM_EEPROM    (FNUM_PARA + CHK_NUM + REM_NUM)
// 所有功能码、参数，包括显示
#define FNUM_ALL       (FNUM_EEPROM + U1NUM + U2NUM )
//==============================功能码组===================================//
struct FUNCCODE_GROUP
{
    uint16_t a0[A0NUM];               // A0
    uint16_t a1[A1NUM];               // A1
    uint16_t a2[A2NUM];               // A2
    uint16_t a3[A3NUM];               // A3
    uint16_t a4[A4NUM];               // A4
    uint16_t a5[A5NUM];               // A5
    uint16_t a6[A6NUM];               // A6
    uint16_t ae[AENUM];               // AE
    uint16_t af[AFNUM];               // AF
    uint16_t b0[B0NUM];               // B0
    uint16_t b1[B1NUM];               // B1
    uint16_t b2[B2NUM];               // B2
    uint16_t b3[B3NUM];               // B3
    uint16_t b4[B4NUM];               // B4
    uint16_t b5[B5NUM];               // B5
    uint16_t b6[B6NUM];               // B6
    uint16_t be[BENUM];               // BE
    uint16_t bf[BFNUM];               // BF
    uint16_t c0[C0NUM];               // C0
    uint16_t c1[C1NUM];               // C1
    uint16_t c2[C2NUM];               // C2
    uint16_t c3[C3NUM];               // C3
    uint16_t c4[C4NUM];               // C4
    uint16_t c5[C5NUM];               // C5
    uint16_t c6[C6NUM];               // C6
    uint16_t ce[CENUM];               // CE
    uint16_t cf[CFNUM];               // CF
    uint16_t d0[D0NUM];               // D0
    uint16_t d1[D1NUM];               // D1
    uint16_t d2[D2NUM];               // D2
    uint16_t d3[D3NUM];               // D3
    uint16_t d4[D4NUM];               // D4
    uint16_t d5[D5NUM];               // D5
    uint16_t d6[D6NUM];               // D6
    uint16_t de[DENUM];               // DE
    uint16_t df[DFNUM];               // DF
    uint16_t u0[U0NUM];               // u0
    uint16_t fChk[CHK_NUM];           //eeprom校验组
    uint16_t remember[REM_NUM];       // 掉电记忆组
    uint16_t u1[U1NUM];               // u1 显示组
    uint16_t u2[U2NUM];               // u2
};
//=============================功能码================================//
struct FUNCCODE_CODE_VFD
{
    // A0 基本功能组
    uint16_t MotorCtrlMode;           // A0-00  (电机)控制方式
    uint16_t RunCommandSelect;        // A0-01  命令源选择
    uint16_t FrqCalcSelect;           // A0-02  频率源选择
    uint16_t FrqXSelect;              // A0-03  主频率源X选择
    uint16_t FrqNumerical;            // A0-04  频率数字设定
    uint16_t FrqPoint;                // A0-05  频率指令小数点
    uint16_t FNumericalRecall;        // A0-06  数字设定频率记忆选择
    uint16_t FrqYSelect;              // A0-07  辅助频率源Y选择
    uint16_t FrqYRangeBase;           // A0-08  辅助频率源Y范围选择
    uint16_t FrqYRange;               // A0-09  辅助频率源Y范围
    uint16_t FrqYOffsetFc;            // A0-10  辅助频率源偏置的数字设定
    uint16_t MaxFrq;                  // A0-11  最大频率
    uint16_t UpperFrqSelect;          // A0-12  上限频率源
    uint16_t UpperFrqNumerical;       // A0-13  上限频率数值设定
    uint16_t lowerFrqNumerical;       // A0-14  下限频率数值设定
    uint16_t UpdnBenchmark;           // A0-15  运行时频率指令UP/DOWN基准
    uint16_t CarrierFrq;              // A0-16  载波频率
    uint16_t ChangeCarrierFrqByTem;   // A0-17  载波频率随温度调整
    uint16_t AccDecBenchmark;         // A0-18  加减速时间基准频率
    uint16_t AccDecTimeUnit;          // A0-19  加减速时间的单位
    uint16_t AccTime1;                // A0-20 加速时间1
    uint16_t DecTime1;                // A0-21  减速时间1
    uint16_t AccTime2;                // A0-22  加速时间2
    uint16_t DecTime2;                // A0-23  减速时间2
    uint16_t RunDirection;            // A0-24  运行方向
    uint16_t AntiReverseRun;          // A0-25  反转控制, 0-允许反转，1-禁止反转
    uint16_t rsvdA0[10];              // A0-27
    // A1 启停参数组
    uint16_t StartMode;               // A1-00  启动方式
    uint16_t SpeedTrackMode;          // A1-01  转速跟踪方式
    uint16_t SpeedTrackVelocity;      // A1-02  转速跟踪快慢
    uint16_t StartFrq;                // A1-03  启动频率
    uint16_t StartFrqTime;            // A1-04  启动频率保持时间
    uint16_t StartBrakeCurrent;       // A1-05  启动直流制动电流
    uint16_t StartBrakeTime;          // A1-06  启动直流制动时间
    uint16_t StartProtect;            // A1-07  启动保护选择..............
    uint16_t StopMode;                // A1-08  停机方式
    uint16_t StopBrakeFrq;            // A1-09  停机直流制动起始频率
    uint16_t StopBrakeWaitTime;       // A1-10  停机直流制动等待时间
    uint16_t StopBrakeCurrent;        // A1-11  停机直流制动电流
    uint16_t StopBrakeTime;           // A1-12  停机直流制动时间
    uint16_t BrakeDutyRatio;          // A1-13  制动使用率
    uint16_t BreakStartVolt;          // A1-14  制动起始电压...........
    uint16_t resdA1_15;               // A1-15  制动管允许开通时间
    uint16_t AccDecSpdCurve;          // A1-16  加减速方式
    uint16_t SCurveStartPhaseTime;    // A1-17  S曲线开始段时间比例
    uint16_t SCurveEndPhaseTime;      // A1-18  S曲线结束段时间比例
    uint16_t AccTimefrqChgValue;      // A1-19  加速时间1/2切换频率点
    uint16_t DecTimefrqChgValue;      // A1-20  减速时间1/2切换频率点
    uint16_t AccTime3;                // A1-21  加速时间3
    uint16_t DecTime3;                // A1-22  减速时间3
    uint16_t AccTime4;                // A1-23  加速时间4
    uint16_t DecTime4;                // A1-24  减速时间4
    uint16_t rsvdA1[10];              // A1-25保留
    // A2 辅助功能组
    uint16_t JogFrq;                  // A2-00  点动运行频率
    uint16_t JogAccTime;              // A2-01  点动加速时间
    uint16_t JogDecTime;              // A2-02  点动减速时间
    uint16_t JogWhenRun;              // A2-03  端子点动优先
    uint16_t JumpFrq1;                // A2-04  跳跃频率1
    uint16_t JumpFrq2;                // A2-05  跳跃频率2
    uint16_t JumpFrqRange;            // A2-06  跳跃频率幅度
    uint16_t JumpFrqMode;             // A2-07  加减速过程中跳跃频率是否有效
    uint16_t UpperFrqOffset;          // A2-08  上限频率偏置
    uint16_t LowerDeal;               // A2-09  频率低于下限频率运行动作
    uint16_t PowerUpTimeArriveSet;    // A2-10  设定上电到达时间
    uint16_t RunTimeArriveSet;        // A2-11  设定运行到达时间
    uint16_t PowerUpTimeAddup;        // A2-12 累计上电时间
    uint16_t RunTimeAddup;            // A2-13 累计运行时间, 单位: h
    uint16_t PowerAddup;              // A2-14 累计耗电量
    uint16_t FrqRunCmdBind;           // A2-15  命令源捆绑频率源
    uint16_t FanControl;              // A2-16  风扇控制
    uint16_t WakeUpFrq;               // A2-17  唤醒频率
    uint16_t WakeUpTime;              // A2-18  唤醒延迟时间
    uint16_t DormantFrq;              // A2-19  休眠频率
    uint16_t DormantTime;             // A2-20  休眠延迟时间
    uint16_t rsvdA2[10];              // A2-21保留
    // A3 保护参数组
    uint16_t ErrorAction[5];              // A3-00 -A3-04  故障时动作选择1-5
    uint16_t ErrorRunFrqSelect;           // A3-05  故障时继续运行频率选择
    uint16_t ErrorSecondFrq;              // A3-06  异常备用频率设定
    uint16_t ErrAutoRstNumMax;            // A3-07  故障自动复位次数
    uint16_t ErrAutoRstRelayAct;          // A3-08  故障自动复位期间故障继电器动作选择
    uint16_t ErrAutoRstSpaceTime;         // A3-09  故障自动复位间隔时间, 0.1s
    uint16_t OverloadMode;                // A3-10  电机过载保护选择
    uint16_t OverloadGain;                // A3-11 电机过载保护增益
    uint16_t ForeOverloadCoef;            // A3-12  电机过载预警系数
    uint16_t CbcEnable;                   // A3-13    逐波限流使能
    uint16_t OverVoltageGain;             // A3-14  过压失速增益
    uint16_t OverVoltagePoint;            // A3-15  过压失速保护电压
    uint16_t OverCurrentGain;             // A3-16  过流失速增益
    uint16_t OverCurrentPoint;            // A3-17  过流失速保护电流
    uint16_t ShortCheckMode;              // A3-18  上电对地短路保护功能
    uint16_t InPhaseLossProtect;          // A3-19  输入缺相保护选择
    uint16_t OutPhaseLossProtect;         // A3-20  输出缺相保护选择
    uint16_t MotorOtMode;               // A3-21   电机温度传感器类型
    uint16_t MotorOtProtect;              // A3-22  电机过热保护阈值
    uint16_t MotorOtCoef;                 // A3-23  电机过热预报警阈值
    uint16_t MotorOtResved;               // A3-24  电机温度相关保留
    uint16_t pOffTransitoryNoStop;        // A3-25  瞬停不停功能选择
    uint16_t pOffTransitoryFrqDecSlope;   // A3-26  瞬停动作暂停判断电压
    uint16_t pOffVolBackTime;             // A3-27  瞬停不停电压回升判断时间
    uint16_t pOffThresholdVol;            // A3-28  瞬停不停动作判断电压
    uint16_t LoseLoadProtectMode;         // A3-29  掉载保护选择
    uint16_t LoseLoadLevel;               // A3-30  掉载检出水平
    uint16_t LoseLoadTime;                // A3-31  掉载检出时间
    uint16_t OverSpeedChkValue;           // A3-32 过速度检测值
    uint16_t OverSpeedChkTime;            // A3-33 过速度检测时间
    uint16_t DevChkValue;                 // A3-34 速度偏差过大检测值
    uint16_t DevChkTime;                  // A3-35 速度偏差过大检测时间
    uint16_t rsvdA3[10];                  // A3-36保留
    // A4 用户参数组
    uint16_t UserPassword;            // A4-00  用户密码
    uint16_t MenuMode;                // A4-01  个性化参数模式选择
    uint16_t MfkKeyFunction;          // A4-02 MF.K键功能选择
    uint16_t ParaInitMode;            // A4-03  参数初始化
    uint16_t MenuReadOnly;          // A4-04 菜单保护
    uint16_t StopKeyFunction;         // A4-05  STOP键
    uint16_t LedDispParaRun1;         // A4-06  LED运行显示参数1
    uint16_t LedDispParaRun2;         // A4-07  LED运行显示参数2
    uint16_t LedDispParaStop;         // A4-08  LED停机显示参数
    uint16_t SpeedDispPointPos;       // A4-09      负载速度显示小数点位置
    uint16_t SpeedDispCoeff;          // A4-10  负载速度显示系数
    uint16_t rsvdA4[10];              // A4-11  保留
    // A5 通讯参数
    uint16_t CommBaudRate;            // A5-00  波特率
    uint16_t CommParity;              // A5-01  数据格式
    uint16_t CommSlaveAddress;        // A5-02  本机地址
    uint16_t CommDelay;               // A5-03  应答延迟
    uint16_t CommOverTime;            // A5-04  通讯超时时间
    uint16_t CommProtocol;            // A5-05  通讯数据传送格式选择
    uint16_t CommReadCurrentPoint;    // A5-06  通讯读取电流分辨率
    uint16_t rsvdA5[16];              // A5-07  保留
    // A6  保留
    uint16_t rsvdA6[10];              // A6-00 保留
    // AE  厂家参数1(模拟量矫正)
    //  AIAO校正
    struct ANALOG_CALIBRATE_CURVE AiCalibrateCurve[AI_NUMBER];  // AE-00, AI1/2/3校正曲线
    struct ANALOG_CALIBRATE_CURVE AoCalibrateCurve[AO_NUMBER];  // AE-12, AO1/AO2校正曲线
    uint16_t AI_DeadZone[3];                                      // AE-20, AI死区时间
    uint16_t rsvdAE_3[10];               //AE-24保留
    // AF  厂家参数2
    uint16_t FactoryPassword;         // AF-00  厂家密码
    uint16_t InverterType;            // AF-01  变频器机型
    uint16_t InverterGpType;            // AF-02 变频器GP机型
    uint16_t InverterPower;           // AF-03  变频器功率
    uint16_t InverterCurrentDisp;       // AF-04  变频器额定电流    大功率要考虑电流小数点问题
    uint16_t TempCurve;               // AF-05  温度曲线
    uint16_t UvGainWarp;              // AF-06  UV两相电流采样增益偏差
    uint16_t UwGainWarp;              // AF-07  Uw两相电流采样增益偏差
    uint16_t RealVol;                 // AF-08  实测母线电压
    uint16_t VolJudgeCoeff;           // AF-09  电压校正系数
    uint16_t CurJudgeCoeff;           // AF-10  电流校正系数
    uint16_t OverVoltagePointSet;     // AF-11  过压点设置
    uint16_t UvPoint;                 // AF-12  欠压点设置
    uint16_t CurSampleDelayComp;      // AF-13  电流检测延时补偿
    uint16_t FuncParaView;            // AF-14  功能参数模式属性  (性能调试参数组显示)
    uint16_t DeadTimeSet;             // AF-15  死区时间调整-1140V专用
    uint16_t DeadCompMode;            // AF-16    死区补偿模式选择
    uint16_t PwmModeSwitchLF;         // AF-17 DPWM切换上限频率
    uint16_t OvCurEN;                 // AF-18    过流中断清除限制使能
    uint16_t SoftVersion;             // AF-19   软件版本号
    uint16_t rsvdAF_3[4];             // AF-20--AF-23保留
    uint16_t rsvdAF_4[10];            // AF-24   保留
    // B0 电机参数
    // 电机基本参数
    uint16_t ratingPower;             // B0-00  电机额定功率
    uint16_t ratingVoltage;           // B0-01  电机额定电压
    uint16_t ratingCurrent;           // B0-02  电机额定电流
    uint16_t ratingFrq;               // B0-03  电机额定频率
    uint16_t ratingSpeed;             // B0-04  电机额定转速
    uint16_t pmsmRs;                  // B0-05  同步机定子电阻
    uint16_t pmsmLd;                  // B0-06  同步机d轴电感
    uint16_t pmsmLq;                  // B0-07  同步机q轴电感
    uint16_t pmsmInitPWMTs;            // B0-08 同步机转子位置检测脉宽
    uint16_t pmsmCoeff;               // B0-09  同步机反电动势系数
    // PG卡参数
    uint16_t fvcPgSrc;                // B0-10  速度反馈PG卡选择, 0-QEP1,1-QEP2(扩展)
    uint16_t pgType;                  // B0-11  编码器类型
    uint16_t enCoderDir;              // B0-12  编码器相序/主方向
    uint16_t encoderPulse;            // B0-13  编码器脉冲线数
    uint16_t enCoderAngle;            // B0-14  编码器安装角
    uint16_t uvwSignDir;              // B0-15  UVW信号方向
    uint16_t uvwSignAngle;            // B0-16  UVW信号零点位置角
    uint16_t enCoderPole;             // B0-17     旋变极对数
    uint16_t rsvdF1_35;               // B0-18  UVW极对数
    uint16_t fvcPgLoseTime;           // B0-19     速度反馈PG断线检测时间       这个参数标记电机参数的结束，修改时注意
    uint16_t motorType;                 // B0-20  电机类型选择
    uint16_t rsvdB0[9];                 // B0-21  保留
    uint16_t tuneCmd;                 // B0-30  调谐选择        放在这一个组的最后
    // B1  速度环参数
    uint16_t vcSpdLoopKp1;              // B1-00  速度环比例增益1
    uint16_t vcSpdLoopTi1;              // B1-01  速度环积分时间1
    uint16_t vcSpdLoopChgFrq1;          // B1-02  切换频率1
    uint16_t vcSpdLoopKp2;              // B1-03  速度环比例增益2
    uint16_t vcSpdLoopTi2;              // B1-04  速度环积分时间2
    uint16_t vcSpdLoopChgFrq2;          // B1-05  切换频率2
    uint16_t vcSlipCompCoef;                // B1-06  转差补偿系数
    uint16_t vcSpdLoopFilterTime;           // B1-07  速度环滤波时间常数
    uint16_t vcOverMagGain;             // B1-08  矢量控制过励磁增益
    uint16_t spdCtrlDriveTorqueLimitSrc;  // B1-09  速度控制(驱动)转矩上限源
    uint16_t spdCtrlDriveTorqueLimit;     // B1-10  速度控制(驱动)转矩上限数字设定
    uint16_t spdCtrlBrakeTorqueLimitSrc;  // B1-11  速度控制(制动)转矩上限源
    uint16_t spdCtrlBrakeTorqueLimit;     // B1-12  速度控制(制动)转矩上限数字设定
    uint16_t spdLoopI;                    // B1-13  速度环积分属性
    uint16_t rsvdB1_0[3];               // B1-14 -16 保留
    uint16_t posInitMode;                   // B1-17 转子位置辨识  0，单相辨识  1三相辨识
    uint16_t rsvdB1[11];                    // B1-18 保留
    // B2  电流环参数
    uint16_t mAcrKp;                  // B2-00  M轴电流环Kp
    uint16_t mAcrKi;                  // B2-01  M轴电流环Ki
    uint16_t tAcrKp;                  // B2-02  T轴电流环Kp
    uint16_t tAcrKi;                  // B2-03  T轴电流环Ki
    uint16_t pmsmCheckTime;         // B2-04       参数辨识获得的保留参数
    uint16_t resvedB2_1[13];          // B2-05----B2-17
    uint16_t weakFlusMode;            // B2-18 同步机弱磁模式
    uint16_t weakFlusCoef;            // B2-19 同步机弱磁系数
    uint16_t weakFlusCurMax;          // B2-20 最大弱磁电流
    uint16_t weakFlusAutoCoef;        // B2-21 弱磁自动调谐系数
    uint16_t weakFlusIntegrMul;       // B2-22 弱磁积分倍数
    uint16_t resvedB2_2[29];          //B2-23----B2-
    uint16_t resvedB2_3[11];          //B2-52----B2-
    // B3 转矩控制
    uint16_t TorqueCtrl;              // B3-00  转矩控制
    uint16_t DriveUpperTorqueSelect;  // B3-01  驱动转矩上限源
    uint16_t resvedB3_1;                // B3-02  保留
    uint16_t DriveUpperTorque;        // B3-03  驱动转矩上限
    uint16_t resvedB3_2;                // B3-04  保留
    uint16_t TorqueCtrlFwdMaxFrq;     // B3-05  转矩控制正向最大频率
    uint16_t TorqueCtrlRevMaxFrq;     // B3-06  转矩控制反向最大频率
    uint16_t TorqueCtrlAccTime;       // B3-07  转矩加速时间
    uint16_t TorqueCtrlDecTime;       // B3-08  转矩减速时间
    uint16_t resvedB3_4[10];          // B3-10
    //  B4 流量压力控制
    uint16_t resvedB4[10];           //暂时十个，以后加上更多100个
    // B5  保留
    uint16_t rsvdB5[10];               // 保留
    // B6  保留
    uint16_t rsvdB6[10];               // 保留
    // BE  保留
    uint16_t rsvdBE[10];               // 保留
    //  BF   性能调试组
    uint16_t resvedBF[40];            //暂时十个，以后加上更多100个
    //  C0  数字量输入参数组
    uint16_t DiFunc[DI_NUMBER];       // C0-00  --C0-09   DI1端子功能选择
    uint16_t DiFilterTime;            // C0-10  DI滤波时间
    uint16_t DiDelayTime[3];          // C0-11--C0-13  DI1---DI3  延迟时间
    uint16_t resvedC0_0[7];           // C0-14--C0-20  DI4---DI10 延迟时间
    uint16_t DiLogic[2];              // C0-21--C0-22  DI有效状态选择1,2
    uint16_t DiControlMode;           // C0-23  端子命令方式
    uint16_t DiUpDownSlope;           // C0-24  端子UP/DOWN速率
    uint16_t DiUptime;                // C0-25      //未显示
    uint16_t DiDntime;                // C0-26
    uint16_t resvedC0_1[10];          // C0-27
    //  C1 模拟量输入参数组
    uint16_t AiCurveSelect;           // C1-00  AI设定曲线选择
    uint16_t AiLimitSelect;           // C1-01  AI低于最小设定选择
    uint16_t Ai1FilterTime;           // C1-02  AI3滤波时间, 10ms
    uint16_t Ai2FilterTime;           // C1-03  AI1滤波时间, 10ms
    uint16_t Ai3FilterTime;           // C1-04  AI2滤波时间, 10ms
    uint16_t CurveSet2P1[4];          // C1-05,...,C1-08  曲线1，2点，最大值，最小值
    uint16_t CurveSet2P2[4];          // C1-09,...,C1-12  曲线2，2点
    uint16_t CurveSet2P3[4];          // C1-13,...,C1-16  曲线3，2点
    uint16_t CurveSet4P1[8];          // C1-17,...,C1-24  曲线4，4点，最大值，最小值，2个中间点
    uint16_t CurveSet4P2[8];          // C1-25,...,C1-32  曲线5，4点
    struct AI_JUMP AiJumpSet[AI_NUMBER]; // C1-33 --C1-38, AI1, AI2, AI3跳跃
    uint16_t CurveSet2P4[4];          // C1-39,...,C1-42  HDI曲线，2点
    uint16_t PulseInFilterTime;            // C1-43  PULSE滤波时间, 10ms
    uint16_t resvedC1[10];                 // C1-44
    // C2 数字量输出功能
    uint16_t FmOutSelect;             // C2-00  多功能端子输出选择
    uint16_t DoFunc[DO_NUMBER];         // C2-01  FMR输出选择
    // C2-02  控制板RELAY输出选择
    // C2-03  扩展卡RELAY输出选择
    // C2-04  DO1输出选择
    // C2-05  扩展卡DO2输出选择     保留
    uint16_t DoDelayTime[DO_NUMBER];    // C2-06  RELAY1输出延迟时间
    // C2-07  RELAY2输出延迟时间
    // C2-08  DO1输出延迟时间
    // C2-09  DO2输出延迟时间
    // C2-10  DO3输出延迟时间
    uint16_t DoLogic;                       // C2-11  DO有效状态选择
    uint16_t AoFunc[AO_NUMBER+HDO_NUMBER];    // C2-12  FMP输出选择
    // C2-13  AO1输出选择
    // C2-14  扩展卡AO2输出选择
    uint16_t FmpOutMaxFrq;                    // C2-15  FMP输出最大频率
    AO_PARA AoPara[AO_NUMBER];              // C2-16  AO1零偏系数
    // C2-17  AO1增益
    // C2-18  AO2零偏系数
    // C2-29  AO2增益
    uint16_t AoLpfTime[AO_NUMBER+HDO_NUMBER]; // C2-20  HDO,AO1,AO2输出滤波时间
    // C2-21  HDO,AO1,AO2输出滤波时间
    // C2-22  HDO,AO1,AO2输出滤波时间
    uint16_t resvedC2[10];                    // C2-23  保留
    // C3 虚拟端子功能
    uint16_t resvedC3[30];                   // C3 保留
    // C4 输出项目组
    uint16_t FrqFdtValue;             // C4-00  频率检测值(FDT电平)
    uint16_t FrqFdtLag;               // C4-01  频率检测滞后值
    uint16_t FrqArriveRange;          // C4-02  频率到达检出幅度
    uint16_t FrqFdt1Value;            // C4-03  频率检测值(FDT1电平)
    uint16_t FrqFdt1Lag;              // C4-04  频率检测1滞后值
    uint16_t FrqArriveValue1;         // C4-05  频率到达检测值1
    uint16_t FrqArriveRange1;         // C4-06  频率到达检出1幅度
    uint16_t FrqArriveValue2;         // C4-07  频率到达检测值2
    uint16_t FrqArriveRange2;         // C4-08  频率到达检出2幅度
    uint16_t ZeroCurrentChkValue;     // C4-09  零电流检测值
    uint16_t ZeroCurrentChkTime;      // C4-10  零电流检测延迟时间
    uint16_t SoftOCValue;             // C4-11  软件过流点
    uint16_t SoftOCDelay;             // C4-12  软件过流检测延迟时间
    uint16_t CurrentArriveValue1;     // C4-13  电流到达检测值1
    uint16_t CurrentArriveRange1;     // C4-14  电流到达检测1幅度
    uint16_t CurrentArriveValue2;     // C4-15  电流到达检测值1
    uint16_t CurrentArriveRange2;     // C4-16  电流到达检测1幅度
    uint16_t SetTimeMode;             // C4-17  定时功能选择
    uint16_t SetTimeSource;           // C4-18  定时时间设定选择
    uint16_t SetTimeValue;            // C4-19  设定运行时间
    uint16_t Ai1VoltageLimit;         // C4-20  AI1输入电压下限
    uint16_t Ai1VoltageUpper;         // C4-21  AI1输入电压上限
    uint16_t TemperatureArrive;       // C4-22  模块温度到达
    uint16_t SetTimeArrive;           // C4-23  当前运行到达时间
    uint16_t CounterSet;              // C4-24  设定计数值
    uint16_t CounterPoint;            // C4-25  指定计数值
    uint16_t LengthSet;               // C4-26  设定长度
    uint16_t LengthPulsePerMeter;     // C4-27 每米脉冲数，单位: 0.1
    uint16_t LengthCurrent;           // C4-28 实际长度
    uint16_t resvedC4[10];            // C4-29  保留
    // C5  保留
    uint16_t rsvdC5[10];               // 保留
    // C6  保留
    uint16_t rsvdC6[10];               // 保留
    // CE  保留
    uint16_t rsvdCE[10];               // 保留
    // CF  保留
    uint16_t rsvdCF[10];               // 保留
    // D0 PID功能组
    uint16_t PidSetSelect;            // D0-00  PID给定源
    uint16_t PidSet;                  // D0-01  PID数值给定, 0.1%
    uint16_t PidFdbSelect;            // D0-02  PID反馈源
    uint16_t PidDir;                  // D0-03  PID作用方向
    uint16_t PidDisp;                 // D0-04  PID给定反馈量程
    uint16_t PidKp;                   // D0-05  比例增益P
    uint16_t PidTi;                   // D0-06  积分时间I
    uint16_t PidTd;                   // D0-07  微分时间D
    uint16_t ReverseCutOffFrq;        // D0-08  PID反转截止频率
    uint16_t PidErrMin;               // D0-09  PID偏差极限
    uint16_t PidDLimit;               // D0-10  PID微分限幅
    uint16_t PidSetChangeTime;        // D0-11  PID给定变化时间
    uint16_t PidFdbLpfTime;           // D0-12  PID反馈滤波时间
    uint16_t PidOutLpfTime;           // D0-13  PID输出滤波时间
    uint16_t PidSampleTime;           // D0-14  PID采样周期(暂未做)
    uint16_t PidKp2;                  // D0-15  PID比例增益P2
    uint16_t PidTi2;                  // D0-16  PID积分时间I2
    uint16_t PidTd2;                  // D0-17  PID微分时间D2
    uint16_t PidParaChgCondition;     // D0-18  PID参数切换条件
    uint16_t PidParaChgDelta1;        // D0-19  PID参数切换偏差1
    uint16_t PidParaChgDelta2;        // D0-20  PID参数切换偏差2
    uint16_t PidInit;                 // D0-21  PID初值
    uint16_t PidInitTime;             // D0-22  PID初值保持时间
    uint16_t PidOutDeltaMax;          // D0-23  PID两次输出之间偏差的最大值
    uint16_t PidOutDeltaMin;          // D0-24  PID两次输出之间偏差的最小值
    uint16_t PidIAttribute;           // D0-25  PID积分属性
    uint16_t PidFdbLoseDetect;        // D0-26  PID反馈丢失检测值
    uint16_t PidFdbLoseDetectTime;    // D0-27  PID反馈丢失检测时间
    uint16_t PidCalcMode;             // D0-28  PID运算模式(停机是否运算). 供水模式下，停机时PID也计算.
    uint16_t resvedD0[10];
    // D1 多段速和PLC控制
    uint16_t PlcFrq[PLC_STEP_MAX];                      // D1-00  --D1-15   多段指令0-多段指令15
    uint16_t PlcRunMode;                            // D1-16  PLC运行方式
    uint16_t PlcPowerOffRemMode;                        // D1-17  PLC掉电记忆选择
    struct PLC_STRUCT PlcAttribute[PLC_STEP_MAX];   // D1-18  --D1-49   PLC运行时间，加减速时间选择
    uint16_t PlcTimeUnit;                           // D1-50  PLC运行时间单位
    uint16_t PlcFrq0Src;                            // D1-51  多令0给定方式
    uint16_t resvedD1[10];
    // D2  保留
    uint16_t rsvdD2[10];               // 保留
    // D3  保留
    uint16_t rsvdD3[10];               // 保留
    // D4  保留
    uint16_t rsvdD4[10];               // 保留
    // D5  保留
    uint16_t rsvdD5[40];               // 保留
    // D6  保留
    uint16_t rsvdD6[10];               // 保留
    // DE  保留
    uint16_t rsvdDE[10];               // 保留
    // DF  保留
    uint16_t rsvdDF[10];               // 保留
    //U0 故障记录组
    uint16_t ErrorLatest1;                // U0-00  第一次故障类型
    uint16_t ErrorLatest2;                // U0-01  第二次故障类型
    uint16_t ErrorLatest3;                // U0-02  第三次(最近一次)故障类型
    union ERROR_SCENE ErrorScene3;      // U0-03  第三次(最近一次)故障时频率
    // U0-04  第三次(最近一次)故障时电流
    // U0-05  第三次(最近一次)故障时母线电压
    // U0-06  第三次(最近一次)故障时输入端子状态
    // U0-07  第三次(最近一次)故障时变频器模块温度
    // U0-08  第三次(最近一次)故障时变频器状态
    // U0-09  第三次(最近一次)故障时时间（从本次上电开始计时）
    // U0-10  第三次(最近一次)故障时时间（从运行时开始计时）
    uint16_t rsvdF9_25[2];                // U0-11  U0-12
    union ERROR_SCENE errorScene2;      // U0-13  第二次故障现场
    uint16_t rsvdF9_35[2];                // U0-21  U0-22
    union ERROR_SCENE errorScene1;      // U0-23  第一次故障现场
    uint16_t rsvdF9_45[2];                // U0-31  U0-32
    uint16_t ErrorShow[2];                // U0-33  -U0-34  故障指示选择1,2
    uint16_t resvedU1[10];
    // EepromCheckWord
    uint16_t rsvd4All;                // 保留
    uint16_t EepromCheckWord1;        // EepromCheckWord1
    uint16_t EepromCheckWord2;        // EepromCheckWord2
    uint16_t AiaoChkWord;             // AIAO出厂校正
    // REMEMBER 掉电保存，共48个
    uint16_t ExtendType;                  // FR-00  ExtendType
    uint16_t PlcStepRem;                  // FR-01  PLC当前step
    uint16_t PlcTimeHighRem;              // FR-02  PLC当前step运行的时间，高位
    uint16_t PlcTimeLowRem;               // FR-03  PLC当前step运行的时间，低位
    uint16_t DispParaRunBit;              // FR-04  运行时LED显示参数的bit位值
    uint16_t DispParaStopBit;             // FR-05  停机时LED显示参数的bit位置
    uint16_t RunTimeAddupSec;             // FR-06  累计运行时间的s(秒)
    uint16_t CounterTicker;               // FR-07  计数器输入的ticker
    uint16_t LengthTickerRemainder;       // FR-08  长度计数器的tickerDelta的Remainder
    uint16_t FrqComm;                     // FR-09  通讯修改频率值, 100.00%-MaxFrq
    uint16_t UpDownFrqRem;                // FR-10  UP/DOWN的频率
    uint16_t PmsmRotorPos;                // FR-11  同步机转子位置
    uint16_t PowerAddupInt;               // FR-12  累计耗电量辅助计算
    uint16_t PowerUpTimeAddupSec;         // FR-13  累计上电时间的s(秒)
    uint16_t ErrorFrqUnit;                // FR-14  故障时频率记录
    uint16_t SaveUserParaFlag1;           // FR-15  已保存用户参数标志1
    uint16_t SaveUserParaFlag2;           // FR-16  已保存用户参数标志2
    uint16_t SpeedFdbDir;                 // FR-17  电机反馈度方向
    uint16_t rsvdRem[6];                  // FR-18~FR-19
//      uint16_t RunTimeAddup;            //   累计运行时间, 单位: h
//      uint16_t PowerUpTimeAddup;        //   累计上电时间
//      uint16_t PowerAddup;              //   累计耗电量
//      uint16_t LengthCurrent;           //   实际长度
    uint16_t rsvdRem1[19];                // 预留
    uint16_t RemPOffMotorCtrl[REM_P_OFF_MOTOR_NUMBER];  // FR-43~FR-47  性能使用的掉电记忆
    uint16_t u1[U1NUM];               // U1显示
    uint16_t u2[U2NUM];               // U2显示
};
typedef union FUNCTIONCODE_ALL_UNION_VFD
{
    uint16_t allVFD[FNUM_ALL];
    struct FUNCCODE_GROUP group;
    struct FUNCCODE_CODE_VFD codeVFD;
} FUNCTIONCODE_ALL_VFD;
// 主辅频率源选择
#define FUNCCODE_FRQXYSELECT_FC                0   // 功能码设定，掉电不记忆
#define FUNCCODE_FRQXYSELECT_FC_P_OFF_REM      1   // 功能码设定，掉电记忆
#define FUNCCODE_FRQXYSELECT_AI1               2   // AI1
#define FUNCCODE_FRQXYSELECT_AI2               3   // AI2
#define FUNCCODE_FRQXYSELECT_AI3               4   // AI3
#define FUNCCODE_FRQXYSELECT_PULSE             5   // PULSE脉冲设定(DI5)
#define FUNCCODE_FRQXYSELECT_MULTI_SET         6   // 多段指令
#define FUNCCODE_FRQXYSELECT_PLC               7   // PLC
#define FUNCCODE_FRQXYSELECT_PID               8   // PID
#define FUNCCODE_FRQXYSELECT_COMM              9   // 通讯设定
// 辅助频率源Y范围选择
#define FUNCCODE_FRQYRANGEBASE_MAX_FRQ      0   // 相对于最大频率
#define FUNCCODE_FRQYRANGEBASE_FRQ_X        1   // 相对于主频率源X
// 频率源(切换关系)选择
#define FUNCCODE_FRQCALCSELECT_X               0   // 主频率源X
#define FUNCCODE_FRQCALCSELECT_COMPOSE         1   // 主辅运算结果
#define FUNCCODE_FRQCALCSELECT_X_OR_Y          2   // 主 <--> 辅
#define FUNCCODE_FRQCALCSELECT_X_OR_COMPOSE    3   // 主 <--> 主辅运算结果
#define FUNCCODE_FRQCALCSELECT_Y_OR_COMPOSE    4   // 辅 <--> 主辅运算结果
// 主辅频率运算关系
#define FUNCCODE_FRQCALCSELECT_ADD             0   // 主 + 辅
#define FUNCCODE_FRQCALCSELECT_SUBTRATION      1   // 主 - 辅
#define FUNCCODE_FRQCALCSELECT_MAX             2   // MAX(主, 辅)
#define FUNCCODE_FRQCALCSELECT_MIN             3   // MIN(主, 辅)
#define FUNCCODE_FRQCALCSELECT_4               4   //
#define FUNCCODE_FRQCALCSELECT_5               5   //
// 上限频率源
#define FUNCCODE_UPPERFRQSELECT_FC         0   // 功能码设定
#define FUNCCODE_UPPERFRQSELECT_AI1        1   // AI1
#define FUNCCODE_UPPERFRQSELECT_AI2        2   // AI2
#define FUNCCODE_UPPERFRQSELECT_AI3        3   // AI3
#define FUNCCODE_UPPERFRQSELECT_PULSE      4   // PULSE脉冲设定(DI5)
#define FUNCCODE_UPPERFRQSELECT_COMM       5   // 通讯给定
// 加减速时间的单位
#define FUNCCODE_ACCDECTIMEUNIT_0POINT  0   // 0个小数点，1s
#define FUNCCODE_ACCDECTIMEUNIT_1POINT  1   // 1个小数点，0.1s
#define FUNCCODE_ACCDECTIMEUNIT_2POINT  2   // 2个小数点，0.01s
// 数值设定频率记忆设定
#define FUNCCODE_FRQNUMERICALSAVE_POWEROFF_NO     0   // 掉电不记忆
#define FUNCCODE_FRQNUMERICALSAVE_POWEROFF_YES    1   // 掉电记忆
#define FUNCCODE_FRQNUMERICALSAVE_STOP_NO         0   // 停机不记忆
#define FUNCCODE_FRQNUMERICALSAVE_STOP_YES        1   // 停机记忆
// 加减速方式
#define FUNCCODE_ACCDECSPDCURVE_LINE        0   // 直线加减速
#define FUNCCODE_ACCDECSPDCURVE_S_CURVE_A   1   // S曲线1，普通二次方
#define FUNCCODE_ACCDECSPDCURVE_S_CURVE_B   2   // S曲线2，参考三菱S曲线B
#define ACC_DEC_LINE    FUNCCODE_ACCDECSPDCURVE_LINE
#define ACC_DEC_SA      FUNCCODE_ACCDECSPDCURVE_S_CURVE_A
#define ACC_DEC_SB      FUNCCODE_ACCDECSPDCURVE_S_CURVE_B
// 转矩上限源
#define FUNCCODE_UPPERTORQUESELECT_FC      0   // 功能码设定
#define FUNCCODE_UPPERTORQUESELECT_AI1     1   // AI1
#define FUNCCODE_UPPERTORQUESELECT_AI2     2   // AI2
#define FUNCCODE_UPPERTORQUESELECT_AI3     3   // AI3
#define FUNCCODE_UPPERTORQUESELECT_PULSE   4   // PULSE
#define FUNCCODE_UPPERTORQUESELECT_COMM    5   // 通讯
#define FUNCCODE_UPPERTORQUESELECT_MIN_AI1_AI2 6  // min(ai1,ai2)
#define FUNCCODE_UPPERTORQUESELECT_MAX_AI1_AI2 7  // max(ai1,ai2)
#define TIME_UNIT_ACC_DEC_SPEED         100 // 加减速时间单位, ms
// (电机)控制方式
#define FUNCCODE_MOTORCTRLMODE_SVC  0   // SVC
#define FUNCCODE_MOTORCTRLMODE_FVC  1   // FVC
#define FUNCCODE_MOTORCTRLMODE_VF   2   // VF
// 命令源选择
#define FUNCCODE_RUNCOMMANDSELECT_PANEL       0   // 操作面板控制通道
#define FUNCCODE_RUNCOMMANDSELECT_DI          1   // 端子命令通道
#define FUNCCODE_RUNCOMMANDSELECT_COMM        2   // 串行口通讯控制通道
#define FUNCCODE_RUNCOMMANDSELECT_AUTO_RUN    3   // 上电运行
// 运行方向
#define FUNCCODE_RUNDIRECTION_NO_REVERSE      0   // 方向一致
#define FUNCCODE_RUNDIRECTION_REVERSE         1   // 方向相反
// 端子启动方式选择
#define FUNCCODE_DICONTROLMODE_2LINE1   0   // 两线式1
#define FUNCCODE_DICONTROLMODE_2LINE2   1   // 两线式2
#define FUNCCODE_DICONTROLMODE_3LINE1   2   // 三线式1
#define FUNCCODE_DICONTROLMODE_3LINE2   3   // 三线式2
// 多功能端子输出选择
#define FUNCCODE_FMOUTSELECT_PULSE      0   // FMP脉冲输出
#define FUNCCODE_FMOUTSELECT_DO         1   // DO
// 启动方式
#define FUNCCODE_STARTMODE_DIRECT_START 0   // 直接启动
#define FUNCCODE_STARTMODE_SPEED_TRACK  1   // 转速跟踪再启动
#define FUNCCODE_STARTMODE_FORE_MAG     2   // 异步机励磁启动
// 停机方式
#define FUNCCODE_STOPMODE_DEC_STOP      0   // 减速停机
#define FUNCCODE_STOPMODE_FREESTOP      1   // 自由停机
#define FUNCCODE_STOPMODE_HURRY_STOP    2   // 急停停机
// 频率低于下限频率运行动作
#define FUNCCODE_FRQLOWERDEAL_RUN_LOWER    0   // 以下限频率运行
#define FUNCCODE_FRQLOWERDEAL_DELAY_STOP   1   // 延时停机
#define FUNCCODE_FRQLOWERDEAL_RUN_ZERO     2   // 零速运行
// PID给定源
#define FUNCCODE_PIDSETSELECT_FC               0   // 功能码设定
#define FUNCCODE_PIDSETSELECT_AI1              1   // AI1
#define FUNCCODE_PIDSETSELECT_AI2              2   // AI2
#define FUNCCODE_PIDSETSELECT_AI3              3   // AI3
#define FUNCCODE_PIDSETSELECT_PULSE            4   // PULSE
#define FUNCCODE_PIDSETSELECT_COMM             5   // 通讯
#define FUNCCODE_PIDSETSELECT_MULTI_SET        6   // 多段指令
// PID参数切换条件
#define FUNCCODE_PIDPARACHGCONDITION_NO         0   // 不切换
#define FUNCCODE_PIDPARACHGCONDITION_DI         1   // DI端子
#define FUNCCODE_PIDPARACHGCONDITION_PID_ERROR  2   // 根据偏差自动切换
// PID反馈源
#define FUNCCODE_PIDFDBSELECT_AI1              0   // AI1
#define FUNCCODE_PIDFDBSELECT_AI2              1   // AI2
#define FUNCCODE_PIDFDBSELECT_AI3              2   // AI3
#define FUNCCODE_PIDFDBSELECT_AI1_SUB_AI2      3   // AI1-AI2
#define FUNCCODE_PIDFDBSELECT_PULSE            4   // PULSE
#define FUNCCODE_PIDFDBSELECT_COMM             5   // 通讯
#define FUNCCODE_PIDFDBSELECT_AI1_ADD_AI2      6   // AI1+AI2
#define FUNCCODE_PIDFDBSELECT_MAX_AI           7   // MAX(|AI1|, |AI2|)
#define FUNCCODE_PIDFDBSELECT_MIN_AI           8   // MIN(|AI1|, |AI2|)
// PID运算模式
#define FUNCCODE_PIDCALCMODE_NO             0   // 停机时不运算
#define FUNCCODE_PIDCALCMODE_YES            1   // 停机时运算
// MK键功能
#define FUNCCODE_MFKKEYFUNCTION_NONE        0   // MF.K键
#define FUNCCODE_MFKKEYFUNCTION_SWITCH      1   // 与操作面板通道切换
#define FUNCCODE_MFKKEYFUNCTION_REVERSE     2   // 正反转切换
#define FUNCCODE_MFKKEYFUNCTION_FWD_JOG     3   // 正转点动命令
#define FUNCCODE_MFKKEYFUNCTION_REV_JOG     4   // 反转点动命令
// STOP/RES键功能
#define FUNCCODE_STOPKEYFUNCTION_KEYBOARD   0   // 停机功能仅在键盘控制方式时有效
#define FUNCCODE_STOPKEYFUNCTION_ALL        1   // 均有效
// 多段指令0给定方式
#define FUNCCODE_PLCFRQ0SELECT_FC          0   // 功能码FC-00给定
#define FUNCCODE_PLCFRQ0SELECT_AI1         1   // AI1
#define FUNCCODE_PLCFRQ0SELECT_AI2         2   // AI2
#define FUNCCODE_PLCFRQ0SELECT_AI3         3   // AI3
#define FUNCCODE_PLCFRQ0SELECT_PULSE       4   // PULSE
#define FUNCCODE_PLCFRQ0SELECT_PID         5   // PID给定
#define FUNCCODE_PLCFRQ0SELECT_PRESET_FRQ  6   // 预置频率
// PLC运行方式
#define FUNCCODE_PLCRUNMODE_ONCE_STOP   0   // 单次运行结束停机
#define FUNCCODE_PLCRUNMODE_ONCE_RUN    1   // 单次运行结束保持终值
#define FUNCCODE_PLCRUNMODE_REPEAT      2   // 一直循环
#define FUNCCODE_PLCPOWEROFFREMMODE_REM   1   // plc掉电记忆   0为不记忆
#define FUNCCODE_PLCSTOPREMMODE_REM       1   // plc停机记忆  0为不记忆
//============================时间相关的定义======================================//
// PLC运行时间单位
#define FUNCCODE_PLCTIMEUNIT_H      1   // H(小时)  0为s秒
// 功能码的时间单位
#define TIME_UNIT_SEC_PER_HOUR          3600    // 1hour = 3600sec
#define TIME_UNIT_SEC_PER_MIN           60      // 1min  = 60sec
#define TIME_UNIT_MS_PER_SEC            1000    // 1s = 1000ms
#define TIME_UNIT_VF_VOL_ACC_TIME       100     // VF分离的电压上升时间
#define TIME_UNIT_AI_PULSE_IN_FILTER    10      // AI,pulseIn滤波时间, ms
#define TIME_UNIT_DI_DELAY              100     // DI输出延迟时间, ms
#define TIME_UNIT_DO_DELAY              100     // DO输出延迟时间, ms
#define TIME_UNIT_START_FRQ_WAIT        100      // 启动频率保持时间，ms
#define TIME_UNIT_START_BRAKE           100     // 启动直流制动时间，ms
#define TIME_UNIT_WAIT_STOP_BRAKE       100     // 停机直流制动等待时间，ms
#define TIME_UNIT_STOP_BRAKE            100     // 停机直流制动时间，ms
#define TIME_UNIT_LOWER_STOP_DELAY      100     // 频率低于下限频率时停机的延迟时间
#define TIME_UNIT_PID_SET_CHANGE        10      // PID给定变化时间
#define TIME_UNIT_PID_FILTER            10      // PID反馈，输出滤波时间
#define TIME_UNIT_PID_INIT              10      // PID初值保持时间
#define TIME_UNIT_PID_FDB_LOSE          100     // PID反馈丢失检测时间
#define TIME_UNIT_SWING_PERIOD          100      // 摆频周期
#define TIME_UNIT_SCICOMMOVERTIME       100     // 通讯超时时间
#define TIME_UNIT_ERR_AUTO_RST_DELAY    100     // 故障自动复位间隔时间，ms
#define TIME_UNIT_ERR_AUTO_RST_CLR      (TIME_UNIT_SEC_PER_HOUR*100UL) // 故障自动复位次数清除时间, 0.1h
#define TIME_UNIT_P_OFF_VOL_BACK        10      // 瞬停不停电压回升判断时间
#define TIME_UNIT_PLC                   100     // PLC运行时间单位
#define TIME_UNIT_WAKE_UP               100     // 唤醒时间的单位
#define TIME_UNIT_DORMANT               100     // 休眠时间的单位
#define TIME_UNIT_CURRENT_CHK           10      // 电流检测时间单位
//=====================================================================
#define ACC_DEC_T_INIT1  ((uint32_t)10*TIME_UNIT_MS_PER_SEC/TIME_UNIT_ACC_DEC_SPEED)   // 加减速时间出厂值，20s，机型 <= 20
#define ACC_DEC_T_INIT2  ((uint32_t)30*TIME_UNIT_MS_PER_SEC/TIME_UNIT_ACC_DEC_SPEED)   // 50s，机型 > 20
#define RATING_FRQ_INIT_0   50      // 电机额定频率，0个小数点
#define RATING_FRQ_INIT_1   500     // 电机额定频率，1个小数点
#define RATING_FRQ_INIT_2   5000    // 电机额定频率，2个小数点
#define BAUD_NUM_MAX        12      // 波特率选择范围的最大值
#define MENU_MODE_MAX       3       // 菜单模式最大值
//=====================================================================
// 有些功能码上下限是其它某个功能码，这里是在FunctionCode中的index
#define FACTORY_PWD_INDEX            (CodeSerialNum(FunctionCodeVFD.codeVFD.FactoryPassword))   //  厂家密码
#define FUNCCODE_FACTORY_START_INDEX  (CodeSerialNum(FunctionCodeVFD.group.af[0]))           // af组的开始
#define FUNCCODE_FACTORY_END_INDEX   (CodeSerialNum(FunctionCodeVFD.group.af[AFNUM - 1]))    // af组的结束
#define FUNCCODE_FACTORY1_START_INDEX  (CodeSerialNum(FunctionCodeVFD.group.ae[0]))           // ae组的开始
#define FUNCCODE_FACTORY1_END_INDEX   (CodeSerialNum(FunctionCodeVFD.group.af[AENUM - 1]))    // ae组的结束
#define FC_MOTOR1_START_INDEX        (CodeSerialNum(FunctionCodeVFD.group.b0[0]))        // 电机参数的起始
#define FC_MOTOR1_END_INDEX          (CodeSerialNum(FunctionCodeVFD.group.b0[B0NUM - 1]))   // 电机参数的结束
#define PRESET_FRQ_INDEX             (CodeSerialNum(FunctionCodeVFD.codeVFD.FrqNumerical))     //    数字频率
#define MAX_FRQ_INDEX                (CodeSerialNum(FunctionCodeVFD.codeVFD.MaxFrq))             //    最大频率
#define UPPER_FRQ_INDEX              (CodeSerialNum(FunctionCodeVFD.codeVFD.UpperFrqNumerical))       //   上限频率
#define LOWER_FRQ_INDEX              (CodeSerialNum(FunctionCodeVFD.codeVFD.lowerFrqNumerical))       //    下限频率
#define ACC_TIME1_INDEX              (CodeSerialNum(FunctionCodeVFD.codeVFD.AccTime1))           //    加速时间1
#define DEC_TIME1_INDEX              (CodeSerialNum(FunctionCodeVFD.codeVFD.DecTime1))           //    减速时间1
#define CARRIER_FRQ_INDEX            (CodeSerialNum(FunctionCodeVFD.codeVFD.CarrierFrq))         //    载波频率
#define RATING_POWER_INDEX           (CodeSerialNum(FunctionCodeVFD.codeVFD.ratingPower))        // 电机额定功率
#define RATING_VOL_INDEX             (CodeSerialNum(FunctionCodeVFD.codeVFD.ratingVoltage))      // 电机额定电压
#define RATING_CUR_INDEX             (CodeSerialNum(FunctionCodeVFD.codeVFD.ratingCurrent))      // 电机额定电流
#define RATING_FRQ_INDEX             (CodeSerialNum(FunctionCodeVFD.codeVFD.ratingFrq))          // 电机额定频率
#define ZERO_LOAD_CURRENT_INDEX      (CodeSerialNum(FunctionCodeVFD.codeVFD.zeroLoadCurrent))    // 空载电流
#define TUNE_CMD_INDEX               (CodeSerialNum(FunctionCodeVFD.codeVFD.tuneCmd))        // 调谐
#define VC_CHG_FRQ1_INDEX            (CodeSerialNum(FunctionCodeVFD.codeVFD.vcSpdLoopChgFrq1))  // 矢量控制速度环 切换频率1
#define VC_CHG_FRQ2_INDEX            (CodeSerialNum(FunctionCodeVFD.codeVFD.vcSpdLoopChgFrq2))  // 矢量控制速度环 切换频率2
#define OV_POINT_SET_INDEX           (CodeSerialNum(FunctionCodeVFD.codeVFD.OverVoltagePointSet))  // 过压点设置
#define CURVE1_MIN_INDEX             (CodeSerialNum(FunctionCodeVFD.codeVFD.CurveSet2P1[0]))       //   AI1最小输入
#define CURVE1_MAX_INDEX             (CodeSerialNum(FunctionCodeVFD.codeVFD.CurveSet2P1[2]))       //   AI1最大输入
#define CURVE2_MIN_INDEX             (CodeSerialNum(FunctionCodeVFD.codeVFD.CurveSet2P2[0]))       //   AI2最小输入
#define CURVE2_MAX_INDEX             (CodeSerialNum(FunctionCodeVFD.codeVFD.CurveSet2P2[2]))       //   AI2最大输入
#define CURVE3_MIN_INDEX             (CodeSerialNum(FunctionCodeVFD.codeVFD.CurveSet2P3[0]))       //   AI3最小输入
#define CURVE3_MAX_INDEX             (CodeSerialNum(FunctionCodeVFD.codeVFD.CurveSet2P3[2]))       //   AI3最大输入
#define PULSE_IN_MIN_INDEX           (CodeSerialNum(FunctionCodeVFD.codeVFD.CurveSet2P4[0]))       //   PULSE最小输入
#define PULSE_IN_MAX_INDEX           (CodeSerialNum(FunctionCodeVFD.codeVFD.CurveSet2P4[2]))       //   PULSE最大输入
#define CURVE4_MIN_INDEX             (CodeSerialNum(FunctionCodeVFD.codeVFD.CurveSet4P1[0]))       //   AI4最小输入
#define CURVE4_INFLEX1_INDEX         (CodeSerialNum(FunctionCodeVFD.codeVFD.CurveSet4P1[2]))       //   AI4拐点1输入
#define CURVE4_INFLEX2_INDEX         (CodeSerialNum(FunctionCodeVFD.codeVFD.CurveSet4P1[4]))       //   AI4拐点2输入
#define CURVE4_MAX_INDEX             (CodeSerialNum(FunctionCodeVFD.codeVFD.CurveSet4P1[6]))       //   AI4最大输入
#define CURVE5_MIN_INDEX             (CodeSerialNum(FunctionCodeVFD.codeVFD.CurveSet4P2[0]))       //   AI5最小输入
#define CURVE5_INFLEX1_INDEX         (CodeSerialNum(FunctionCodeVFD.codeVFD.CurveSet4P2[2]))       //   AI5拐点1输入
#define CURVE5_INFLEX2_INDEX         (CodeSerialNum(FunctionCodeVFD.codeVFD.CurveSet4P2[4]))       //   AI5拐点2输入
#define CURVE5_MAX_INDEX             (CodeSerialNum(FunctionCodeVFD.codeVFD.CurveSet4P2[6]))       //   AI5最大输入
#define ACC_TIME2_INDEX              (CodeSerialNum(FunctionCodeVFD.codeVFD.AccTime2))       //  加速时间2
#define DEC_TIME2_INDEX              (CodeSerialNum(FunctionCodeVFD.codeVFD.DecTime2))       //  减速时间2
#define ACC_TIME3_INDEX              (CodeSerialNum(FunctionCodeVFD.codeVFD.AccTime3))       //  加速时间3
#define DEC_TIME3_INDEX              (CodeSerialNum(FunctionCodeVFD.codeVFD.DecTime3))       //  减速时3
#define ACC_TIME4_INDEX              (CodeSerialNum(FunctionCodeVFD.codeVFD.AccTime4))       //  加速时间4
#define DEC_TIME4_INDEX              (CodeSerialNum(FunctionCodeVFD.codeVFD.DecTime4))       //  减速时间4
#define RUN_TIME_ADDUP_INDEX         (CodeSerialNum(FunctionCodeVFD.codeVFD.RunTimeAddup))     //   累计运行时间
#define POWER_TIME_ADDUP_INDEX       (CodeSerialNum(FunctionCodeVFD.codeVFD.PowerUpTimeAddup)) //   累计上电时间
#define POWER_ADDUP_INDEX            (CodeSerialNum(FunctionCodeVFD.codeVFD.PowerAddup))       //   累计耗电量
#define AI1_LIMIT                    (CodeSerialNum(FunctionCodeVFD.codeVFD.Ai1VoltageLimit))   //    AI保护下限
#define AI1_UPPER                    (CodeSerialNum(FunctionCodeVFD.codeVFD.Ai1VoltageUpper))   //    AI保护上限
#define PID_PARA_CHG_DELTA1_MAX      (CodeSerialNum(FunctionCodeVFD.codeVFD.PidParaChgDelta2))  //   PID参数切换偏差2
#define PID_PARA_CHG_DELTA2_MIN      (CodeSerialNum(FunctionCodeVFD.codeVFD.PidParaChgDelta1))  //   PID参数切换偏差1
#define DORMANT_UPPER                (CodeSerialNum(FunctionCodeVFD.codeVFD.WakeUpFrq))         // 休眠频率上限
#define WAKE_UP_LIMIT                (CodeSerialNum(FunctionCodeVFD.codeVFD.DormantFrq))        // 唤醒频率下限
#define LAST_ERROR_RECORD_INDEX      (CodeSerialNum(FunctionCodeVFD.codeVFD.errorScene1.allVFD[sizeof(struct ERROR_SCENE_STRUCT) - 1]))  // 最后一个故障记录
#define MIN_CBC_TIME_INDEX           (CodeSerialNum(FunctionCodeVFD.codeVFD.cbcMinTime))         //   逐波限流时间下限
#define MAX_CBC_TIME_INDEX           (CodeSerialNum(FunctionCodeVFD.codeVFD.cbcMaxTime))         //   逐波限流时间上限
#define EEPROM_CHECK_INDEX1          (CodeSerialNum(FunctionCodeVFD.codeVFD.EepromCheckWord1))
#define EEPROM_CHECK_INDEX2          (CodeSerialNum(FunctionCodeVFD.codeVFD.EepromCheckWord2))
#define SAVE_USER_PARA_PARA1         (CodeSerialNum(FunctionCodeVFD.codeVFD.SaveUserParaFlag1))
#define SAVE_USER_PARA_PARA2         (CodeSerialNum(FunctionCodeVFD.codeVFD.SaveUserParaFlag2))
//-------------------------------
#define FC_GROUP_FACTORY            FUNCCODE_GROUP_AF   // 厂家参数组
#define FC_START_GROUP              FUNCCODE_GROUP_A0   // 功能码组显示的第1组
//--------------------------------
extern FUNCTIONCODE_ALL_VFD FunctionCodeVFD;
extern const uint16_t OverVoltageInitValue[];
extern uint16_t FunctionCodeGradeCurMenuMode[];
#endif
/********************************************************************************************************/
