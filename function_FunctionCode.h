/*
*********************************************************************************************************
*
*   ģ������ : ��Ƶ������
*   �ļ����� : function_FunctionCode.h
*   ��    �� : V1.0
*   ˵    �� : ͷ�ļ�
*
*********************************************************************************************************
*/
#ifndef _F_FUNCTIONCODE_H_
#define _F_FUNCTIONCODE_H_
#include "stdint.h"
// EEPROMУ����
#define EEPROM_CHECK_1       (SOFTWARE_VERSION)              // EEPROMУ����1   ������汾�й�
#define EEPROM_CHECK_2       (FNUM_EEPROM)                   // EEPROMУ����2  ����Ҫ����Ĺ��������
// �����������Լ����������������õ���ֵΪ�������˳��ֵ
#define CodeSerialNum(codeVFD)           ((uint16_t)((&(codeVFD)) - (&(FunctionCodeVFD.allVFD[0]))))
#define GradeSerialNum(group, grade)  (FunctionCodeGradeSum[group] + (grade))
//�����������
// ��λbit3-5
#define ATTRIBUTE_UNIT_HZ_BIT           0
#define ATTRIBUTE_UNIT_A_BIT            1
#define ATTRIBUTE_UNIT_V_BIT            2
// ��д���� bit 11-12
#define ATTRIBUTE_READ_AND_WRITE        0   // (�κ�ʱ��)��д
#define ATTRIBUTE_READ_ONLY_WHEN_RUN    1   // ����ʱֻ��
#define ATTRIBUTE_READ_ONLY_ANYTIME     2   // ֻ��
// ����������������ԣ����������й�
#define ATTRIBUTE_MULTI_LIMIT_SINGLE    0   // �����Ĺ�����
#define ATTRIBUTE_MULTI_LIMIT_DEC       1   // ��������룬ʮ����
#define ATTRIBUTE_MULTI_LIMIT_HEX       2   // ��������룬ʮ������
extern const uint16_t FunctionCodeGradeSum[];
struct  FUNC_ATTRIBUTE_BITS
{
    // bit    ����
    uint16_t point:3;         // 2-0    000����С���� 001��1λС����...��100��4λС��
    uint16_t unit:3;          // 5:3    000���޵�λ��001��hz,:010��A, 011��RPM, 100��V, 110-%;
    uint16_t displayBits:3;   // 8:6    000����ʾ0λ��001����ʾ1λ��...��101����ʾ5λ
    uint16_t upperLimit:1;    // 9      1-������������ع��������ƣ�0-ֱ������������
    uint16_t lowerLimit:1;    // 10     1-������������ع��������ƣ�0-ֱ������������
    uint16_t writable:2;      // 12:11  00�����Զ�д, 01��������ֻ����10������ֻ��
    uint16_t signal:1;        // 13     0���޷���; 1���з���
    uint16_t multiLimit:2;    // 15:14  00-����������(�����);
    //        01-ʮ����,  �������������;
    //        10-ʮ������,�������������;
};
union FUNC_ATTRIBUTE
{
    uint16_t                    allVFD;
    struct FUNC_ATTRIBUTE_BITS  bitVFD;
};
//=====================================================================
// ���������Ա����ޡ����ޡ�����ֵ������
//
typedef struct FUNCCODE_ATTRIBUTE_STRUCT_VFD
{
    uint16_t                  lower;          // ����
    uint16_t                  upper;          // ����
    uint16_t                  init;           // ����ֵ
    union FUNC_ATTRIBUTE      attribute;      // ����
} FUNCCODE_ATTRIBUTE_VFD;
extern const FUNCCODE_ATTRIBUTE_VFD funcCodeAttributeVFD[];
struct PLC_STRUCT           //PLC�����õĽṹ��
{
    uint16_t runTime;         // PLC��_������ʱ��
    uint16_t accDecTimeSet;   // PLC��_�μӼ���ʱ��ѡ��
};
struct AI_SET_CURVE  // AI�趨����
{
    uint16_t minIn;       // ������С����(��ѹֵ)
    uint16_t minInSet;    // ������С�����Ӧ�趨(�ٷֱ�)
    uint16_t maxIn;       // �����������(��ѹֵ)
    uint16_t maxInSet;    // ������������Ӧ�趨(�ٷֱ�)
};
struct AI_JUMP    //ģ������Ծ
{
    uint16_t point;   // �趨��Ծ��
    uint16_t arrange; // �趨��Ծ����
};
struct ANALOG_CALIBRATE_CURVE  // ģ����У�����ߣ�AIAO
{
    uint16_t before1;     // У��ǰ��ѹ1
    uint16_t after1;      // У�����ѹ1
    uint16_t before2;     // У��ǰ��ѹ2
    uint16_t after2;      // У�����ѹ2
};
typedef struct AO_PARA_STRUCT  //AO�������
{
    uint16_t offset;          // AO��ƫϵ��
    uint16_t gain;            // AO����
} AO_PARA;
struct ERROR_SCENE_STRUCT
{
    uint16_t errorFrq;                    // ������(���һ��)����ʱƵ��
    uint16_t errorCurrent;                // ������(���һ��)����ʱ����
    uint16_t errorGeneratrixVoltage;      // ������(���һ��)����ʱĸ�ߵ�ѹ
    uint16_t errorDiStatus;               // ������(���һ��)����ʱ�������״̬
    uint16_t errorTemp;                   // ������(���һ��)����ʱ��Ƶ��ģ���¶�
    uint16_t errorInverterStatus;         // ������(���һ��)����ʱ��Ƶ��״̬
    uint16_t errorTimeFromPowerUp;        // ������(���һ��)����ʱʱ�䣨�ӱ����ϵ翪ʼ��ʱ��
    uint16_t errorTimeFromRun;            // ������(���һ��)����ʱʱ�䣨������ʱ��ʼ��ʱ��
};
union ERROR_SCENE
{
    uint16_t allVFD[sizeof(struct ERROR_SCENE_STRUCT)/2];
    struct ERROR_SCENE_STRUCT elem;
};
//=================================
#define MOTOR_TYPE_ACI_GENERAL  0   // ��ͨ�첽���
#define MOTOR_TYPE_ACI_INV      1   // ��Ƶ�첽���
#define MOTOR_TYPE_PMSM         2   // ����ͬ�����
//------------------------------------------------
struct VC_PARA
{
    uint16_t vcSpdLoopKp1;            //   �ٶȻ���������1
    uint16_t vcSpdLoopTi1;            //   �ٶȻ�����ʱ��1
    uint16_t vcSpdLoopChgFrq1;        //   �л�Ƶ��1
    uint16_t vcSpdLoopKp2;            //   �ٶȻ���������2
    uint16_t vcSpdLoopTi2;            //   �ٶȻ�����ʱ��2
    uint16_t vcSpdLoopChgFrq2;        //   �л�Ƶ��2
    uint16_t vcSlipCompCoef;          //   ת���ϵ��
    uint16_t vcSpdLoopFilterTime;     //  �ٶȻ��˲�ʱ�䳣��
    uint16_t vcOverMagGain;           //   ʸ�����ƹ���������
    uint16_t spdCtrlDriveTorqueLimitSrc;  //   �ٶȿ���(����)ת������Դ
    uint16_t spdCtrlDriveTorqueLimit;     //   �ٶȿ���(����)ת�����������趨
    uint16_t spdCtrlBrakeTorqueLimitSrc;  //   �ٶȿ���(�ƶ�)ת������Դ
    uint16_t spdCtrlBrakeTorqueLimit;     //  �ٶȿ���(�ƶ�)ת�����������趨
    uint16_t mAcrKp;                  //   M�������Kp
    uint16_t mAcrKi;                  //   M�������Ki
    uint16_t tAcrKp;                  //   T�������Kp
    uint16_t tAcrKi;                  //    T�������Ki
    uint16_t spdLoopI;                //   �ٶȻ���������
    uint16_t weakFlusMode;            // ͬ��������ģʽ
    uint16_t weakFlusCoef;            // ͬ��������ϵ��
    uint16_t weakFlusCurMax;          //  ������ŵ���
    uint16_t weakFlusAutoCoef;        // �����Զ���гϵ��
    uint16_t weakFlusIntegrMul;       //  ���Ż��ֱ���
};
//------------------------------------------------
// �������������������������Ʋ���
struct MOTOR_FC
{
    // �����������
    uint16_t motorType;               //   �������ѡ��
    uint16_t ratingPower;             //   ��������
    uint16_t ratingVoltage;           //   ������ѹ
    uint16_t ratingCurrent;           //   ��������
    uint16_t ratingFrq;               //   ����Ƶ��
    uint16_t ratingSpeed;             //   ����ת��
    // ͬ������г����
    uint16_t pmsmRs;                  //   ͬ�������ӵ���
    uint16_t pmsmLd;                  //   ͬ����d����
    uint16_t pmsmLq;                  //   ͬ����q����
    uint16_t pmsmInitPWMTs;            //   ͬ����ת��λ�ü������
    uint16_t pmsmCoeff;               //  ͬ�������綯��ϵ��
    uint16_t pmsmCheckTime;           //   ͬ����
    // PG������
    uint16_t fvcPgSrc;                //     �ٶȷ���PG��ѡ��, 0-QEP1,1-QEP2(��չ)
    uint16_t pgType;                  //     ����������
    uint16_t enCoderDir;              //     ����������/������
    uint16_t encoderPulse;            //     ��������������
    uint16_t enCoderAngle;            //     ��������װ��
    uint16_t uvwSignDir;              //    UVW�źŷ���
    uint16_t uvwSignAngle;            //    UVW�ź����λ�ý�
    uint16_t enCoderPole;             //   ���伫����
    uint16_t rsvdF1_35;                //     UVW������
    uint16_t fvcPgLoseTime;           //    �ٶȷ���PG���߼��ʱ��
    uint16_t tuneCmd;                 // ��г����
    struct VC_PARA vcPara;          // VC���Ʋ���
    uint16_t MotorCtrlMode;           // ������Ʒ�ʽ
    uint16_t accDecTimeMotor;         // ����Ӽ���ʱ��ѡ��
};
//------------------------------------------------/
#define AI_NUMBER               3           // AI���Ӹ���
#define AO_NUMBER               2           // AO���Ӹ���
#define HDI_NUMBER              1           // HDI���Ӹ���
#define HDO_NUMBER              1           // HDO���Ӹ���
#define DI_NUMBER               10          // ����DI����
#define DI_FUNC_NUMBER          49          // DI�Ĺ���ѡ������
#define DO_NUMBER               5           // ����DO����
#define PLC_STEP_MAX            16          // PLC�����ָ�����
//=====================================================================
#define EEPROM_USE_LENGTH        FNUM_EEPROM    //����Ҫ����Ĺ�����ĳ���Ҳ��eeprom��ʹ�ó���
#define EEPROM_INDEX_USE_INDEX        4     // ���ܲ���ʹ��EEPROM��ʼ
// Ԥ��������ʹ�õĹ�����index
#define FUNCCODE_RSVD4ALL_INDEX     CodeSerialNum(FunctionCodeVFD.codeVFD.rsvd4All)
#define REM_P_OFF_MOTOR_NUMBER     5   // ���ܵ������Ĳ�������
// �����������
#define FUNCCODE_GROUP_NUMBER  41  // ����Ԥ������
// EEPROM��˳��: EEPROM_CHK, ����˳�򱣴�
//ÿ������
#define FUNCCODE_GROUP_A0   0       // A0��
#define FUNCCODE_GROUP_AE   7       // AE��
#define FUNCCODE_GROUP_AF   8       // AF��
#define FUNCCODE_GROUP_B0   9       // B0��
#define FUNCCODE_GROUP_BF   17      // BF��
#define FUNCCODE_GROUP_C0   18      // C0��
#define FUNCCODE_GROUP_CF   26      // CF��
#define FUNCCODE_GROUP_D0   27      // D0��
#define FUNCCODE_GROUP_D5   32      // D5��
#define FUNCCODE_GROUP_DF   35      // DF��
#define FUNCCODE_GROUP_U0   36      // U0��   ������ʾ
//������ �����飬ΪeepromУ��͵��籣��
#define FUNCCODE_GROUP_U1   39       // H0�飬��ʾ
#define FUNCCODE_GROUP_U2   40      // U1��   ������ʾ
//����Ϊ����ʹ�õĹ�����������壬������������
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
#define A0RSVDNUM              (10 )   // A0����
#define A1RSVDNUM              (10 )   // A1����
#define A2RSVDNUM              (10 )   // A2����
#define A3RSVDNUM              (10 )   // A3����
#define A4RSVDNUM              (10 )   // A4����
#define A5RSVDNUM              (10 )   // A5����
#define A6RSVDNUM              (10 )   // A6����
#define AERSVDNUM              (10 )   // AE����
#define AFRSVDNUM              (14 )   // AF����
#define B0RSVDNUM              (0 )   // B0����
#define B1RSVDNUM              (10 )   // B1����
#define B2RSVDNUM              (10 )   // B2����
#define B3RSVDNUM              (10 )   // B3����
#define B4RSVDNUM              (10 )   // B4����
#define B5RSVDNUM              (10 )   // B5����
#define B6RSVDNUM              (10 )   // B6����
#define BERSVDNUM              (10 )   // BE����
#define BFRSVDNUM              (10 )   // BF����
#define C0RSVDNUM              (10 )   // C0����
#define C1RSVDNUM              (10 )   // C1����
#define C2RSVDNUM              (10 )   // C2����
#define C3RSVDNUM              (30 )   // C3����
#define C4RSVDNUM              (10 )   // C4����
#define C5RSVDNUM              (10 )   // C5����
#define C6RSVDNUM              (10 )   // C6����
#define CERSVDNUM              (10 )   // CE����
#define CFRSVDNUM              (10 )   // CF����
#define D0RSVDNUM              (10 )   // D0����
#define D1RSVDNUM              (10 )   // D1����
#define D2RSVDNUM              (10 )   // D2����
#define D3RSVDNUM              (10 )   // D3����
#define D4RSVDNUM              (10 )   // D4����
#define D5RSVDNUM              (10 )   // D5����
#define D6RSVDNUM              (10 )   // D6����
#define DERSVDNUM              (10 )   // DE����
#define DFRSVDNUM              (10 )   // DF����
#define U0RSVDNUM              (10 )   // U0����
// ����Ϊÿ����Ĺ��������������������
// ��FF��֮�⣬ÿ��Ԥ��5�������롣Ϊ�������ӹ�����ʱ���������ûָ�����������
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
#define CHK_NUM         (4)        //   eeprom����Ǹ���
#define REM_NUM         (48)       //   ���е�����䱣��Ĺ��������
#define U1NUM           (50)      //   U1��ʾ�����������������eeprom����
#define U2NUM           (40)      //   U2��ʾ�����������������eeprom����
//���еĹ������������������ʾ�ģ�
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
//������Ҫ������eeprom�еĹ��������
#define FNUM_EEPROM    (FNUM_PARA + CHK_NUM + REM_NUM)
// ���й����롢������������ʾ
#define FNUM_ALL       (FNUM_EEPROM + U1NUM + U2NUM )
//==============================��������===================================//
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
    uint16_t fChk[CHK_NUM];           //eepromУ����
    uint16_t remember[REM_NUM];       // ���������
    uint16_t u1[U1NUM];               // u1 ��ʾ��
    uint16_t u2[U2NUM];               // u2
};
//=============================������================================//
struct FUNCCODE_CODE_VFD
{
    // A0 ����������
    uint16_t MotorCtrlMode;           // A0-00  (���)���Ʒ�ʽ
    uint16_t RunCommandSelect;        // A0-01  ����Դѡ��
    uint16_t FrqCalcSelect;           // A0-02  Ƶ��Դѡ��
    uint16_t FrqXSelect;              // A0-03  ��Ƶ��ԴXѡ��
    uint16_t FrqNumerical;            // A0-04  Ƶ�������趨
    uint16_t FrqPoint;                // A0-05  Ƶ��ָ��С����
    uint16_t FNumericalRecall;        // A0-06  �����趨Ƶ�ʼ���ѡ��
    uint16_t FrqYSelect;              // A0-07  ����Ƶ��ԴYѡ��
    uint16_t FrqYRangeBase;           // A0-08  ����Ƶ��ԴY��Χѡ��
    uint16_t FrqYRange;               // A0-09  ����Ƶ��ԴY��Χ
    uint16_t FrqYOffsetFc;            // A0-10  ����Ƶ��Դƫ�õ������趨
    uint16_t MaxFrq;                  // A0-11  ���Ƶ��
    uint16_t UpperFrqSelect;          // A0-12  ����Ƶ��Դ
    uint16_t UpperFrqNumerical;       // A0-13  ����Ƶ����ֵ�趨
    uint16_t lowerFrqNumerical;       // A0-14  ����Ƶ����ֵ�趨
    uint16_t UpdnBenchmark;           // A0-15  ����ʱƵ��ָ��UP/DOWN��׼
    uint16_t CarrierFrq;              // A0-16  �ز�Ƶ��
    uint16_t ChangeCarrierFrqByTem;   // A0-17  �ز�Ƶ�����¶ȵ���
    uint16_t AccDecBenchmark;         // A0-18  �Ӽ���ʱ���׼Ƶ��
    uint16_t AccDecTimeUnit;          // A0-19  �Ӽ���ʱ��ĵ�λ
    uint16_t AccTime1;                // A0-20 ����ʱ��1
    uint16_t DecTime1;                // A0-21  ����ʱ��1
    uint16_t AccTime2;                // A0-22  ����ʱ��2
    uint16_t DecTime2;                // A0-23  ����ʱ��2
    uint16_t RunDirection;            // A0-24  ���з���
    uint16_t AntiReverseRun;          // A0-25  ��ת����, 0-����ת��1-��ֹ��ת
    uint16_t rsvdA0[10];              // A0-27
    // A1 ��ͣ������
    uint16_t StartMode;               // A1-00  ������ʽ
    uint16_t SpeedTrackMode;          // A1-01  ת�ٸ��ٷ�ʽ
    uint16_t SpeedTrackVelocity;      // A1-02  ת�ٸ��ٿ���
    uint16_t StartFrq;                // A1-03  ����Ƶ��
    uint16_t StartFrqTime;            // A1-04  ����Ƶ�ʱ���ʱ��
    uint16_t StartBrakeCurrent;       // A1-05  ����ֱ���ƶ�����
    uint16_t StartBrakeTime;          // A1-06  ����ֱ���ƶ�ʱ��
    uint16_t StartProtect;            // A1-07  ��������ѡ��..............
    uint16_t StopMode;                // A1-08  ͣ����ʽ
    uint16_t StopBrakeFrq;            // A1-09  ͣ��ֱ���ƶ���ʼƵ��
    uint16_t StopBrakeWaitTime;       // A1-10  ͣ��ֱ���ƶ��ȴ�ʱ��
    uint16_t StopBrakeCurrent;        // A1-11  ͣ��ֱ���ƶ�����
    uint16_t StopBrakeTime;           // A1-12  ͣ��ֱ���ƶ�ʱ��
    uint16_t BrakeDutyRatio;          // A1-13  �ƶ�ʹ����
    uint16_t BreakStartVolt;          // A1-14  �ƶ���ʼ��ѹ...........
    uint16_t resdA1_15;               // A1-15  �ƶ�������ͨʱ��
    uint16_t AccDecSpdCurve;          // A1-16  �Ӽ��ٷ�ʽ
    uint16_t SCurveStartPhaseTime;    // A1-17  S���߿�ʼ��ʱ�����
    uint16_t SCurveEndPhaseTime;      // A1-18  S���߽�����ʱ�����
    uint16_t AccTimefrqChgValue;      // A1-19  ����ʱ��1/2�л�Ƶ�ʵ�
    uint16_t DecTimefrqChgValue;      // A1-20  ����ʱ��1/2�л�Ƶ�ʵ�
    uint16_t AccTime3;                // A1-21  ����ʱ��3
    uint16_t DecTime3;                // A1-22  ����ʱ��3
    uint16_t AccTime4;                // A1-23  ����ʱ��4
    uint16_t DecTime4;                // A1-24  ����ʱ��4
    uint16_t rsvdA1[10];              // A1-25����
    // A2 ����������
    uint16_t JogFrq;                  // A2-00  �㶯����Ƶ��
    uint16_t JogAccTime;              // A2-01  �㶯����ʱ��
    uint16_t JogDecTime;              // A2-02  �㶯����ʱ��
    uint16_t JogWhenRun;              // A2-03  ���ӵ㶯����
    uint16_t JumpFrq1;                // A2-04  ��ԾƵ��1
    uint16_t JumpFrq2;                // A2-05  ��ԾƵ��2
    uint16_t JumpFrqRange;            // A2-06  ��ԾƵ�ʷ���
    uint16_t JumpFrqMode;             // A2-07  �Ӽ��ٹ�������ԾƵ���Ƿ���Ч
    uint16_t UpperFrqOffset;          // A2-08  ����Ƶ��ƫ��
    uint16_t LowerDeal;               // A2-09  Ƶ�ʵ�������Ƶ�����ж���
    uint16_t PowerUpTimeArriveSet;    // A2-10  �趨�ϵ絽��ʱ��
    uint16_t RunTimeArriveSet;        // A2-11  �趨���е���ʱ��
    uint16_t PowerUpTimeAddup;        // A2-12 �ۼ��ϵ�ʱ��
    uint16_t RunTimeAddup;            // A2-13 �ۼ�����ʱ��, ��λ: h
    uint16_t PowerAddup;              // A2-14 �ۼƺĵ���
    uint16_t FrqRunCmdBind;           // A2-15  ����Դ����Ƶ��Դ
    uint16_t FanControl;              // A2-16  ���ȿ���
    uint16_t WakeUpFrq;               // A2-17  ����Ƶ��
    uint16_t WakeUpTime;              // A2-18  �����ӳ�ʱ��
    uint16_t DormantFrq;              // A2-19  ����Ƶ��
    uint16_t DormantTime;             // A2-20  �����ӳ�ʱ��
    uint16_t rsvdA2[10];              // A2-21����
    // A3 ����������
    uint16_t ErrorAction[5];              // A3-00 -A3-04  ����ʱ����ѡ��1-5
    uint16_t ErrorRunFrqSelect;           // A3-05  ����ʱ��������Ƶ��ѡ��
    uint16_t ErrorSecondFrq;              // A3-06  �쳣����Ƶ���趨
    uint16_t ErrAutoRstNumMax;            // A3-07  �����Զ���λ����
    uint16_t ErrAutoRstRelayAct;          // A3-08  �����Զ���λ�ڼ���ϼ̵�������ѡ��
    uint16_t ErrAutoRstSpaceTime;         // A3-09  �����Զ���λ���ʱ��, 0.1s
    uint16_t OverloadMode;                // A3-10  ������ر���ѡ��
    uint16_t OverloadGain;                // A3-11 ������ر�������
    uint16_t ForeOverloadCoef;            // A3-12  �������Ԥ��ϵ��
    uint16_t CbcEnable;                   // A3-13    ������ʹ��
    uint16_t OverVoltageGain;             // A3-14  ��ѹʧ������
    uint16_t OverVoltagePoint;            // A3-15  ��ѹʧ�ٱ�����ѹ
    uint16_t OverCurrentGain;             // A3-16  ����ʧ������
    uint16_t OverCurrentPoint;            // A3-17  ����ʧ�ٱ�������
    uint16_t ShortCheckMode;              // A3-18  �ϵ�Եض�·��������
    uint16_t InPhaseLossProtect;          // A3-19  ����ȱ�ౣ��ѡ��
    uint16_t OutPhaseLossProtect;         // A3-20  ���ȱ�ౣ��ѡ��
    uint16_t MotorOtMode;               // A3-21   ����¶ȴ���������
    uint16_t MotorOtProtect;              // A3-22  ������ȱ�����ֵ
    uint16_t MotorOtCoef;                 // A3-23  �������Ԥ������ֵ
    uint16_t MotorOtResved;               // A3-24  ����¶���ر���
    uint16_t pOffTransitoryNoStop;        // A3-25  ˲ͣ��ͣ����ѡ��
    uint16_t pOffTransitoryFrqDecSlope;   // A3-26  ˲ͣ������ͣ�жϵ�ѹ
    uint16_t pOffVolBackTime;             // A3-27  ˲ͣ��ͣ��ѹ�����ж�ʱ��
    uint16_t pOffThresholdVol;            // A3-28  ˲ͣ��ͣ�����жϵ�ѹ
    uint16_t LoseLoadProtectMode;         // A3-29  ���ر���ѡ��
    uint16_t LoseLoadLevel;               // A3-30  ���ؼ��ˮƽ
    uint16_t LoseLoadTime;                // A3-31  ���ؼ��ʱ��
    uint16_t OverSpeedChkValue;           // A3-32 ���ٶȼ��ֵ
    uint16_t OverSpeedChkTime;            // A3-33 ���ٶȼ��ʱ��
    uint16_t DevChkValue;                 // A3-34 �ٶ�ƫ�������ֵ
    uint16_t DevChkTime;                  // A3-35 �ٶ�ƫ�������ʱ��
    uint16_t rsvdA3[10];                  // A3-36����
    // A4 �û�������
    uint16_t UserPassword;            // A4-00  �û�����
    uint16_t MenuMode;                // A4-01  ���Ի�����ģʽѡ��
    uint16_t MfkKeyFunction;          // A4-02 MF.K������ѡ��
    uint16_t ParaInitMode;            // A4-03  ������ʼ��
    uint16_t MenuReadOnly;          // A4-04 �˵�����
    uint16_t StopKeyFunction;         // A4-05  STOP��
    uint16_t LedDispParaRun1;         // A4-06  LED������ʾ����1
    uint16_t LedDispParaRun2;         // A4-07  LED������ʾ����2
    uint16_t LedDispParaStop;         // A4-08  LEDͣ����ʾ����
    uint16_t SpeedDispPointPos;       // A4-09      �����ٶ���ʾС����λ��
    uint16_t SpeedDispCoeff;          // A4-10  �����ٶ���ʾϵ��
    uint16_t rsvdA4[10];              // A4-11  ����
    // A5 ͨѶ����
    uint16_t CommBaudRate;            // A5-00  ������
    uint16_t CommParity;              // A5-01  ���ݸ�ʽ
    uint16_t CommSlaveAddress;        // A5-02  ������ַ
    uint16_t CommDelay;               // A5-03  Ӧ���ӳ�
    uint16_t CommOverTime;            // A5-04  ͨѶ��ʱʱ��
    uint16_t CommProtocol;            // A5-05  ͨѶ���ݴ��͸�ʽѡ��
    uint16_t CommReadCurrentPoint;    // A5-06  ͨѶ��ȡ�����ֱ���
    uint16_t rsvdA5[16];              // A5-07  ����
    // A6  ����
    uint16_t rsvdA6[10];              // A6-00 ����
    // AE  ���Ҳ���1(ģ��������)
    //  AIAOУ��
    struct ANALOG_CALIBRATE_CURVE AiCalibrateCurve[AI_NUMBER];  // AE-00, AI1/2/3У������
    struct ANALOG_CALIBRATE_CURVE AoCalibrateCurve[AO_NUMBER];  // AE-12, AO1/AO2У������
    uint16_t AI_DeadZone[3];                                      // AE-20, AI����ʱ��
    uint16_t rsvdAE_3[10];               //AE-24����
    // AF  ���Ҳ���2
    uint16_t FactoryPassword;         // AF-00  ��������
    uint16_t InverterType;            // AF-01  ��Ƶ������
    uint16_t InverterGpType;            // AF-02 ��Ƶ��GP����
    uint16_t InverterPower;           // AF-03  ��Ƶ������
    uint16_t InverterCurrentDisp;       // AF-04  ��Ƶ�������    ����Ҫ���ǵ���С��������
    uint16_t TempCurve;               // AF-05  �¶�����
    uint16_t UvGainWarp;              // AF-06  UV���������������ƫ��
    uint16_t UwGainWarp;              // AF-07  Uw���������������ƫ��
    uint16_t RealVol;                 // AF-08  ʵ��ĸ�ߵ�ѹ
    uint16_t VolJudgeCoeff;           // AF-09  ��ѹУ��ϵ��
    uint16_t CurJudgeCoeff;           // AF-10  ����У��ϵ��
    uint16_t OverVoltagePointSet;     // AF-11  ��ѹ������
    uint16_t UvPoint;                 // AF-12  Ƿѹ������
    uint16_t CurSampleDelayComp;      // AF-13  ���������ʱ����
    uint16_t FuncParaView;            // AF-14  ���ܲ���ģʽ����  (���ܵ��Բ�������ʾ)
    uint16_t DeadTimeSet;             // AF-15  ����ʱ�����-1140Vר��
    uint16_t DeadCompMode;            // AF-16    ��������ģʽѡ��
    uint16_t PwmModeSwitchLF;         // AF-17 DPWM�л�����Ƶ��
    uint16_t OvCurEN;                 // AF-18    �����ж��������ʹ��
    uint16_t SoftVersion;             // AF-19   ����汾��
    uint16_t rsvdAF_3[4];             // AF-20--AF-23����
    uint16_t rsvdAF_4[10];            // AF-24   ����
    // B0 �������
    // �����������
    uint16_t ratingPower;             // B0-00  ��������
    uint16_t ratingVoltage;           // B0-01  ������ѹ
    uint16_t ratingCurrent;           // B0-02  ��������
    uint16_t ratingFrq;               // B0-03  ����Ƶ��
    uint16_t ratingSpeed;             // B0-04  ����ת��
    uint16_t pmsmRs;                  // B0-05  ͬ�������ӵ���
    uint16_t pmsmLd;                  // B0-06  ͬ����d����
    uint16_t pmsmLq;                  // B0-07  ͬ����q����
    uint16_t pmsmInitPWMTs;            // B0-08 ͬ����ת��λ�ü������
    uint16_t pmsmCoeff;               // B0-09  ͬ�������綯��ϵ��
    // PG������
    uint16_t fvcPgSrc;                // B0-10  �ٶȷ���PG��ѡ��, 0-QEP1,1-QEP2(��չ)
    uint16_t pgType;                  // B0-11  ����������
    uint16_t enCoderDir;              // B0-12  ����������/������
    uint16_t encoderPulse;            // B0-13  ��������������
    uint16_t enCoderAngle;            // B0-14  ��������װ��
    uint16_t uvwSignDir;              // B0-15  UVW�źŷ���
    uint16_t uvwSignAngle;            // B0-16  UVW�ź����λ�ý�
    uint16_t enCoderPole;             // B0-17     ���伫����
    uint16_t rsvdF1_35;               // B0-18  UVW������
    uint16_t fvcPgLoseTime;           // B0-19     �ٶȷ���PG���߼��ʱ��       ���������ǵ�������Ľ������޸�ʱע��
    uint16_t motorType;                 // B0-20  �������ѡ��
    uint16_t rsvdB0[9];                 // B0-21  ����
    uint16_t tuneCmd;                 // B0-30  ��гѡ��        ������һ��������
    // B1  �ٶȻ�����
    uint16_t vcSpdLoopKp1;              // B1-00  �ٶȻ���������1
    uint16_t vcSpdLoopTi1;              // B1-01  �ٶȻ�����ʱ��1
    uint16_t vcSpdLoopChgFrq1;          // B1-02  �л�Ƶ��1
    uint16_t vcSpdLoopKp2;              // B1-03  �ٶȻ���������2
    uint16_t vcSpdLoopTi2;              // B1-04  �ٶȻ�����ʱ��2
    uint16_t vcSpdLoopChgFrq2;          // B1-05  �л�Ƶ��2
    uint16_t vcSlipCompCoef;                // B1-06  ת���ϵ��
    uint16_t vcSpdLoopFilterTime;           // B1-07  �ٶȻ��˲�ʱ�䳣��
    uint16_t vcOverMagGain;             // B1-08  ʸ�����ƹ���������
    uint16_t spdCtrlDriveTorqueLimitSrc;  // B1-09  �ٶȿ���(����)ת������Դ
    uint16_t spdCtrlDriveTorqueLimit;     // B1-10  �ٶȿ���(����)ת�����������趨
    uint16_t spdCtrlBrakeTorqueLimitSrc;  // B1-11  �ٶȿ���(�ƶ�)ת������Դ
    uint16_t spdCtrlBrakeTorqueLimit;     // B1-12  �ٶȿ���(�ƶ�)ת�����������趨
    uint16_t spdLoopI;                    // B1-13  �ٶȻ���������
    uint16_t rsvdB1_0[3];               // B1-14 -16 ����
    uint16_t posInitMode;                   // B1-17 ת��λ�ñ�ʶ  0�������ʶ  1�����ʶ
    uint16_t rsvdB1[11];                    // B1-18 ����
    // B2  ����������
    uint16_t mAcrKp;                  // B2-00  M�������Kp
    uint16_t mAcrKi;                  // B2-01  M�������Ki
    uint16_t tAcrKp;                  // B2-02  T�������Kp
    uint16_t tAcrKi;                  // B2-03  T�������Ki
    uint16_t pmsmCheckTime;         // B2-04       ������ʶ��õı�������
    uint16_t resvedB2_1[13];          // B2-05----B2-17
    uint16_t weakFlusMode;            // B2-18 ͬ��������ģʽ
    uint16_t weakFlusCoef;            // B2-19 ͬ��������ϵ��
    uint16_t weakFlusCurMax;          // B2-20 ������ŵ���
    uint16_t weakFlusAutoCoef;        // B2-21 �����Զ���гϵ��
    uint16_t weakFlusIntegrMul;       // B2-22 ���Ż��ֱ���
    uint16_t resvedB2_2[29];          //B2-23----B2-
    uint16_t resvedB2_3[11];          //B2-52----B2-
    // B3 ת�ؿ���
    uint16_t TorqueCtrl;              // B3-00  ת�ؿ���
    uint16_t DriveUpperTorqueSelect;  // B3-01  ����ת������Դ
    uint16_t resvedB3_1;                // B3-02  ����
    uint16_t DriveUpperTorque;        // B3-03  ����ת������
    uint16_t resvedB3_2;                // B3-04  ����
    uint16_t TorqueCtrlFwdMaxFrq;     // B3-05  ת�ؿ����������Ƶ��
    uint16_t TorqueCtrlRevMaxFrq;     // B3-06  ת�ؿ��Ʒ������Ƶ��
    uint16_t TorqueCtrlAccTime;       // B3-07  ת�ؼ���ʱ��
    uint16_t TorqueCtrlDecTime;       // B3-08  ת�ؼ���ʱ��
    uint16_t resvedB3_4[10];          // B3-10
    //  B4 ����ѹ������
    uint16_t resvedB4[10];           //��ʱʮ�����Ժ���ϸ���100��
    // B5  ����
    uint16_t rsvdB5[10];               // ����
    // B6  ����
    uint16_t rsvdB6[10];               // ����
    // BE  ����
    uint16_t rsvdBE[10];               // ����
    //  BF   ���ܵ�����
    uint16_t resvedBF[40];            //��ʱʮ�����Ժ���ϸ���100��
    //  C0  ���������������
    uint16_t DiFunc[DI_NUMBER];       // C0-00  --C0-09   DI1���ӹ���ѡ��
    uint16_t DiFilterTime;            // C0-10  DI�˲�ʱ��
    uint16_t DiDelayTime[3];          // C0-11--C0-13  DI1---DI3  �ӳ�ʱ��
    uint16_t resvedC0_0[7];           // C0-14--C0-20  DI4---DI10 �ӳ�ʱ��
    uint16_t DiLogic[2];              // C0-21--C0-22  DI��Ч״̬ѡ��1,2
    uint16_t DiControlMode;           // C0-23  �������ʽ
    uint16_t DiUpDownSlope;           // C0-24  ����UP/DOWN����
    uint16_t DiUptime;                // C0-25      //δ��ʾ
    uint16_t DiDntime;                // C0-26
    uint16_t resvedC0_1[10];          // C0-27
    //  C1 ģ�������������
    uint16_t AiCurveSelect;           // C1-00  AI�趨����ѡ��
    uint16_t AiLimitSelect;           // C1-01  AI������С�趨ѡ��
    uint16_t Ai1FilterTime;           // C1-02  AI3�˲�ʱ��, 10ms
    uint16_t Ai2FilterTime;           // C1-03  AI1�˲�ʱ��, 10ms
    uint16_t Ai3FilterTime;           // C1-04  AI2�˲�ʱ��, 10ms
    uint16_t CurveSet2P1[4];          // C1-05,...,C1-08  ����1��2�㣬���ֵ����Сֵ
    uint16_t CurveSet2P2[4];          // C1-09,...,C1-12  ����2��2��
    uint16_t CurveSet2P3[4];          // C1-13,...,C1-16  ����3��2��
    uint16_t CurveSet4P1[8];          // C1-17,...,C1-24  ����4��4�㣬���ֵ����Сֵ��2���м��
    uint16_t CurveSet4P2[8];          // C1-25,...,C1-32  ����5��4��
    struct AI_JUMP AiJumpSet[AI_NUMBER]; // C1-33 --C1-38, AI1, AI2, AI3��Ծ
    uint16_t CurveSet2P4[4];          // C1-39,...,C1-42  HDI���ߣ�2��
    uint16_t PulseInFilterTime;            // C1-43  PULSE�˲�ʱ��, 10ms
    uint16_t resvedC1[10];                 // C1-44
    // C2 �������������
    uint16_t FmOutSelect;             // C2-00  �๦�ܶ������ѡ��
    uint16_t DoFunc[DO_NUMBER];         // C2-01  FMR���ѡ��
    // C2-02  ���ư�RELAY���ѡ��
    // C2-03  ��չ��RELAY���ѡ��
    // C2-04  DO1���ѡ��
    // C2-05  ��չ��DO2���ѡ��     ����
    uint16_t DoDelayTime[DO_NUMBER];    // C2-06  RELAY1����ӳ�ʱ��
    // C2-07  RELAY2����ӳ�ʱ��
    // C2-08  DO1����ӳ�ʱ��
    // C2-09  DO2����ӳ�ʱ��
    // C2-10  DO3����ӳ�ʱ��
    uint16_t DoLogic;                       // C2-11  DO��Ч״̬ѡ��
    uint16_t AoFunc[AO_NUMBER+HDO_NUMBER];    // C2-12  FMP���ѡ��
    // C2-13  AO1���ѡ��
    // C2-14  ��չ��AO2���ѡ��
    uint16_t FmpOutMaxFrq;                    // C2-15  FMP������Ƶ��
    AO_PARA AoPara[AO_NUMBER];              // C2-16  AO1��ƫϵ��
    // C2-17  AO1����
    // C2-18  AO2��ƫϵ��
    // C2-29  AO2����
    uint16_t AoLpfTime[AO_NUMBER+HDO_NUMBER]; // C2-20  HDO,AO1,AO2����˲�ʱ��
    // C2-21  HDO,AO1,AO2����˲�ʱ��
    // C2-22  HDO,AO1,AO2����˲�ʱ��
    uint16_t resvedC2[10];                    // C2-23  ����
    // C3 ������ӹ���
    uint16_t resvedC3[30];                   // C3 ����
    // C4 �����Ŀ��
    uint16_t FrqFdtValue;             // C4-00  Ƶ�ʼ��ֵ(FDT��ƽ)
    uint16_t FrqFdtLag;               // C4-01  Ƶ�ʼ���ͺ�ֵ
    uint16_t FrqArriveRange;          // C4-02  Ƶ�ʵ���������
    uint16_t FrqFdt1Value;            // C4-03  Ƶ�ʼ��ֵ(FDT1��ƽ)
    uint16_t FrqFdt1Lag;              // C4-04  Ƶ�ʼ��1�ͺ�ֵ
    uint16_t FrqArriveValue1;         // C4-05  Ƶ�ʵ�����ֵ1
    uint16_t FrqArriveRange1;         // C4-06  Ƶ�ʵ�����1����
    uint16_t FrqArriveValue2;         // C4-07  Ƶ�ʵ�����ֵ2
    uint16_t FrqArriveRange2;         // C4-08  Ƶ�ʵ�����2����
    uint16_t ZeroCurrentChkValue;     // C4-09  ��������ֵ
    uint16_t ZeroCurrentChkTime;      // C4-10  ���������ӳ�ʱ��
    uint16_t SoftOCValue;             // C4-11  ���������
    uint16_t SoftOCDelay;             // C4-12  �����������ӳ�ʱ��
    uint16_t CurrentArriveValue1;     // C4-13  ����������ֵ1
    uint16_t CurrentArriveRange1;     // C4-14  ����������1����
    uint16_t CurrentArriveValue2;     // C4-15  ����������ֵ1
    uint16_t CurrentArriveRange2;     // C4-16  ����������1����
    uint16_t SetTimeMode;             // C4-17  ��ʱ����ѡ��
    uint16_t SetTimeSource;           // C4-18  ��ʱʱ���趨ѡ��
    uint16_t SetTimeValue;            // C4-19  �趨����ʱ��
    uint16_t Ai1VoltageLimit;         // C4-20  AI1�����ѹ����
    uint16_t Ai1VoltageUpper;         // C4-21  AI1�����ѹ����
    uint16_t TemperatureArrive;       // C4-22  ģ���¶ȵ���
    uint16_t SetTimeArrive;           // C4-23  ��ǰ���е���ʱ��
    uint16_t CounterSet;              // C4-24  �趨����ֵ
    uint16_t CounterPoint;            // C4-25  ָ������ֵ
    uint16_t LengthSet;               // C4-26  �趨����
    uint16_t LengthPulsePerMeter;     // C4-27 ÿ������������λ: 0.1
    uint16_t LengthCurrent;           // C4-28 ʵ�ʳ���
    uint16_t resvedC4[10];            // C4-29  ����
    // C5  ����
    uint16_t rsvdC5[10];               // ����
    // C6  ����
    uint16_t rsvdC6[10];               // ����
    // CE  ����
    uint16_t rsvdCE[10];               // ����
    // CF  ����
    uint16_t rsvdCF[10];               // ����
    // D0 PID������
    uint16_t PidSetSelect;            // D0-00  PID����Դ
    uint16_t PidSet;                  // D0-01  PID��ֵ����, 0.1%
    uint16_t PidFdbSelect;            // D0-02  PID����Դ
    uint16_t PidDir;                  // D0-03  PID���÷���
    uint16_t PidDisp;                 // D0-04  PID������������
    uint16_t PidKp;                   // D0-05  ��������P
    uint16_t PidTi;                   // D0-06  ����ʱ��I
    uint16_t PidTd;                   // D0-07  ΢��ʱ��D
    uint16_t ReverseCutOffFrq;        // D0-08  PID��ת��ֹƵ��
    uint16_t PidErrMin;               // D0-09  PIDƫ���
    uint16_t PidDLimit;               // D0-10  PID΢���޷�
    uint16_t PidSetChangeTime;        // D0-11  PID�����仯ʱ��
    uint16_t PidFdbLpfTime;           // D0-12  PID�����˲�ʱ��
    uint16_t PidOutLpfTime;           // D0-13  PID����˲�ʱ��
    uint16_t PidSampleTime;           // D0-14  PID��������(��δ��)
    uint16_t PidKp2;                  // D0-15  PID��������P2
    uint16_t PidTi2;                  // D0-16  PID����ʱ��I2
    uint16_t PidTd2;                  // D0-17  PID΢��ʱ��D2
    uint16_t PidParaChgCondition;     // D0-18  PID�����л�����
    uint16_t PidParaChgDelta1;        // D0-19  PID�����л�ƫ��1
    uint16_t PidParaChgDelta2;        // D0-20  PID�����л�ƫ��2
    uint16_t PidInit;                 // D0-21  PID��ֵ
    uint16_t PidInitTime;             // D0-22  PID��ֵ����ʱ��
    uint16_t PidOutDeltaMax;          // D0-23  PID�������֮��ƫ������ֵ
    uint16_t PidOutDeltaMin;          // D0-24  PID�������֮��ƫ�����Сֵ
    uint16_t PidIAttribute;           // D0-25  PID��������
    uint16_t PidFdbLoseDetect;        // D0-26  PID������ʧ���ֵ
    uint16_t PidFdbLoseDetectTime;    // D0-27  PID������ʧ���ʱ��
    uint16_t PidCalcMode;             // D0-28  PID����ģʽ(ͣ���Ƿ�����). ��ˮģʽ�£�ͣ��ʱPIDҲ����.
    uint16_t resvedD0[10];
    // D1 ����ٺ�PLC����
    uint16_t PlcFrq[PLC_STEP_MAX];                      // D1-00  --D1-15   ���ָ��0-���ָ��15
    uint16_t PlcRunMode;                            // D1-16  PLC���з�ʽ
    uint16_t PlcPowerOffRemMode;                        // D1-17  PLC�������ѡ��
    struct PLC_STRUCT PlcAttribute[PLC_STEP_MAX];   // D1-18  --D1-49   PLC����ʱ�䣬�Ӽ���ʱ��ѡ��
    uint16_t PlcTimeUnit;                           // D1-50  PLC����ʱ�䵥λ
    uint16_t PlcFrq0Src;                            // D1-51  ����0������ʽ
    uint16_t resvedD1[10];
    // D2  ����
    uint16_t rsvdD2[10];               // ����
    // D3  ����
    uint16_t rsvdD3[10];               // ����
    // D4  ����
    uint16_t rsvdD4[10];               // ����
    // D5  ����
    uint16_t rsvdD5[40];               // ����
    // D6  ����
    uint16_t rsvdD6[10];               // ����
    // DE  ����
    uint16_t rsvdDE[10];               // ����
    // DF  ����
    uint16_t rsvdDF[10];               // ����
    //U0 ���ϼ�¼��
    uint16_t ErrorLatest1;                // U0-00  ��һ�ι�������
    uint16_t ErrorLatest2;                // U0-01  �ڶ��ι�������
    uint16_t ErrorLatest3;                // U0-02  ������(���һ��)��������
    union ERROR_SCENE ErrorScene3;      // U0-03  ������(���һ��)����ʱƵ��
    // U0-04  ������(���һ��)����ʱ����
    // U0-05  ������(���һ��)����ʱĸ�ߵ�ѹ
    // U0-06  ������(���һ��)����ʱ�������״̬
    // U0-07  ������(���һ��)����ʱ��Ƶ��ģ���¶�
    // U0-08  ������(���һ��)����ʱ��Ƶ��״̬
    // U0-09  ������(���һ��)����ʱʱ�䣨�ӱ����ϵ翪ʼ��ʱ��
    // U0-10  ������(���һ��)����ʱʱ�䣨������ʱ��ʼ��ʱ��
    uint16_t rsvdF9_25[2];                // U0-11  U0-12
    union ERROR_SCENE errorScene2;      // U0-13  �ڶ��ι����ֳ�
    uint16_t rsvdF9_35[2];                // U0-21  U0-22
    union ERROR_SCENE errorScene1;      // U0-23  ��һ�ι����ֳ�
    uint16_t rsvdF9_45[2];                // U0-31  U0-32
    uint16_t ErrorShow[2];                // U0-33  -U0-34  ����ָʾѡ��1,2
    uint16_t resvedU1[10];
    // EepromCheckWord
    uint16_t rsvd4All;                // ����
    uint16_t EepromCheckWord1;        // EepromCheckWord1
    uint16_t EepromCheckWord2;        // EepromCheckWord2
    uint16_t AiaoChkWord;             // AIAO����У��
    // REMEMBER ���籣�棬��48��
    uint16_t ExtendType;                  // FR-00  ExtendType
    uint16_t PlcStepRem;                  // FR-01  PLC��ǰstep
    uint16_t PlcTimeHighRem;              // FR-02  PLC��ǰstep���е�ʱ�䣬��λ
    uint16_t PlcTimeLowRem;               // FR-03  PLC��ǰstep���е�ʱ�䣬��λ
    uint16_t DispParaRunBit;              // FR-04  ����ʱLED��ʾ������bitλֵ
    uint16_t DispParaStopBit;             // FR-05  ͣ��ʱLED��ʾ������bitλ��
    uint16_t RunTimeAddupSec;             // FR-06  �ۼ�����ʱ���s(��)
    uint16_t CounterTicker;               // FR-07  �����������ticker
    uint16_t LengthTickerRemainder;       // FR-08  ���ȼ�������tickerDelta��Remainder
    uint16_t FrqComm;                     // FR-09  ͨѶ�޸�Ƶ��ֵ, 100.00%-MaxFrq
    uint16_t UpDownFrqRem;                // FR-10  UP/DOWN��Ƶ��
    uint16_t PmsmRotorPos;                // FR-11  ͬ����ת��λ��
    uint16_t PowerAddupInt;               // FR-12  �ۼƺĵ�����������
    uint16_t PowerUpTimeAddupSec;         // FR-13  �ۼ��ϵ�ʱ���s(��)
    uint16_t ErrorFrqUnit;                // FR-14  ����ʱƵ�ʼ�¼
    uint16_t SaveUserParaFlag1;           // FR-15  �ѱ����û�������־1
    uint16_t SaveUserParaFlag2;           // FR-16  �ѱ����û�������־2
    uint16_t SpeedFdbDir;                 // FR-17  ��������ȷ���
    uint16_t rsvdRem[6];                  // FR-18~FR-19
//      uint16_t RunTimeAddup;            //   �ۼ�����ʱ��, ��λ: h
//      uint16_t PowerUpTimeAddup;        //   �ۼ��ϵ�ʱ��
//      uint16_t PowerAddup;              //   �ۼƺĵ���
//      uint16_t LengthCurrent;           //   ʵ�ʳ���
    uint16_t rsvdRem1[19];                // Ԥ��
    uint16_t RemPOffMotorCtrl[REM_P_OFF_MOTOR_NUMBER];  // FR-43~FR-47  ����ʹ�õĵ������
    uint16_t u1[U1NUM];               // U1��ʾ
    uint16_t u2[U2NUM];               // U2��ʾ
};
typedef union FUNCTIONCODE_ALL_UNION_VFD
{
    uint16_t allVFD[FNUM_ALL];
    struct FUNCCODE_GROUP group;
    struct FUNCCODE_CODE_VFD codeVFD;
} FUNCTIONCODE_ALL_VFD;
// ����Ƶ��Դѡ��
#define FUNCCODE_FRQXYSELECT_FC                0   // �������趨�����粻����
#define FUNCCODE_FRQXYSELECT_FC_P_OFF_REM      1   // �������趨���������
#define FUNCCODE_FRQXYSELECT_AI1               2   // AI1
#define FUNCCODE_FRQXYSELECT_AI2               3   // AI2
#define FUNCCODE_FRQXYSELECT_AI3               4   // AI3
#define FUNCCODE_FRQXYSELECT_PULSE             5   // PULSE�����趨(DI5)
#define FUNCCODE_FRQXYSELECT_MULTI_SET         6   // ���ָ��
#define FUNCCODE_FRQXYSELECT_PLC               7   // PLC
#define FUNCCODE_FRQXYSELECT_PID               8   // PID
#define FUNCCODE_FRQXYSELECT_COMM              9   // ͨѶ�趨
// ����Ƶ��ԴY��Χѡ��
#define FUNCCODE_FRQYRANGEBASE_MAX_FRQ      0   // ��������Ƶ��
#define FUNCCODE_FRQYRANGEBASE_FRQ_X        1   // �������Ƶ��ԴX
// Ƶ��Դ(�л���ϵ)ѡ��
#define FUNCCODE_FRQCALCSELECT_X               0   // ��Ƶ��ԴX
#define FUNCCODE_FRQCALCSELECT_COMPOSE         1   // ����������
#define FUNCCODE_FRQCALCSELECT_X_OR_Y          2   // �� <--> ��
#define FUNCCODE_FRQCALCSELECT_X_OR_COMPOSE    3   // �� <--> ����������
#define FUNCCODE_FRQCALCSELECT_Y_OR_COMPOSE    4   // �� <--> ����������
// ����Ƶ�������ϵ
#define FUNCCODE_FRQCALCSELECT_ADD             0   // �� + ��
#define FUNCCODE_FRQCALCSELECT_SUBTRATION      1   // �� - ��
#define FUNCCODE_FRQCALCSELECT_MAX             2   // MAX(��, ��)
#define FUNCCODE_FRQCALCSELECT_MIN             3   // MIN(��, ��)
#define FUNCCODE_FRQCALCSELECT_4               4   //
#define FUNCCODE_FRQCALCSELECT_5               5   //
// ����Ƶ��Դ
#define FUNCCODE_UPPERFRQSELECT_FC         0   // �������趨
#define FUNCCODE_UPPERFRQSELECT_AI1        1   // AI1
#define FUNCCODE_UPPERFRQSELECT_AI2        2   // AI2
#define FUNCCODE_UPPERFRQSELECT_AI3        3   // AI3
#define FUNCCODE_UPPERFRQSELECT_PULSE      4   // PULSE�����趨(DI5)
#define FUNCCODE_UPPERFRQSELECT_COMM       5   // ͨѶ����
// �Ӽ���ʱ��ĵ�λ
#define FUNCCODE_ACCDECTIMEUNIT_0POINT  0   // 0��С���㣬1s
#define FUNCCODE_ACCDECTIMEUNIT_1POINT  1   // 1��С���㣬0.1s
#define FUNCCODE_ACCDECTIMEUNIT_2POINT  2   // 2��С���㣬0.01s
// ��ֵ�趨Ƶ�ʼ����趨
#define FUNCCODE_FRQNUMERICALSAVE_POWEROFF_NO     0   // ���粻����
#define FUNCCODE_FRQNUMERICALSAVE_POWEROFF_YES    1   // �������
#define FUNCCODE_FRQNUMERICALSAVE_STOP_NO         0   // ͣ��������
#define FUNCCODE_FRQNUMERICALSAVE_STOP_YES        1   // ͣ������
// �Ӽ��ٷ�ʽ
#define FUNCCODE_ACCDECSPDCURVE_LINE        0   // ֱ�߼Ӽ���
#define FUNCCODE_ACCDECSPDCURVE_S_CURVE_A   1   // S����1����ͨ���η�
#define FUNCCODE_ACCDECSPDCURVE_S_CURVE_B   2   // S����2���ο�����S����B
#define ACC_DEC_LINE    FUNCCODE_ACCDECSPDCURVE_LINE
#define ACC_DEC_SA      FUNCCODE_ACCDECSPDCURVE_S_CURVE_A
#define ACC_DEC_SB      FUNCCODE_ACCDECSPDCURVE_S_CURVE_B
// ת������Դ
#define FUNCCODE_UPPERTORQUESELECT_FC      0   // �������趨
#define FUNCCODE_UPPERTORQUESELECT_AI1     1   // AI1
#define FUNCCODE_UPPERTORQUESELECT_AI2     2   // AI2
#define FUNCCODE_UPPERTORQUESELECT_AI3     3   // AI3
#define FUNCCODE_UPPERTORQUESELECT_PULSE   4   // PULSE
#define FUNCCODE_UPPERTORQUESELECT_COMM    5   // ͨѶ
#define FUNCCODE_UPPERTORQUESELECT_MIN_AI1_AI2 6  // min(ai1,ai2)
#define FUNCCODE_UPPERTORQUESELECT_MAX_AI1_AI2 7  // max(ai1,ai2)
#define TIME_UNIT_ACC_DEC_SPEED         100 // �Ӽ���ʱ�䵥λ, ms
// (���)���Ʒ�ʽ
#define FUNCCODE_MOTORCTRLMODE_SVC  0   // SVC
#define FUNCCODE_MOTORCTRLMODE_FVC  1   // FVC
#define FUNCCODE_MOTORCTRLMODE_VF   2   // VF
// ����Դѡ��
#define FUNCCODE_RUNCOMMANDSELECT_PANEL       0   // ����������ͨ��
#define FUNCCODE_RUNCOMMANDSELECT_DI          1   // ��������ͨ��
#define FUNCCODE_RUNCOMMANDSELECT_COMM        2   // ���п�ͨѶ����ͨ��
#define FUNCCODE_RUNCOMMANDSELECT_AUTO_RUN    3   // �ϵ�����
// ���з���
#define FUNCCODE_RUNDIRECTION_NO_REVERSE      0   // ����һ��
#define FUNCCODE_RUNDIRECTION_REVERSE         1   // �����෴
// ����������ʽѡ��
#define FUNCCODE_DICONTROLMODE_2LINE1   0   // ����ʽ1
#define FUNCCODE_DICONTROLMODE_2LINE2   1   // ����ʽ2
#define FUNCCODE_DICONTROLMODE_3LINE1   2   // ����ʽ1
#define FUNCCODE_DICONTROLMODE_3LINE2   3   // ����ʽ2
// �๦�ܶ������ѡ��
#define FUNCCODE_FMOUTSELECT_PULSE      0   // FMP�������
#define FUNCCODE_FMOUTSELECT_DO         1   // DO
// ������ʽ
#define FUNCCODE_STARTMODE_DIRECT_START 0   // ֱ������
#define FUNCCODE_STARTMODE_SPEED_TRACK  1   // ת�ٸ���������
#define FUNCCODE_STARTMODE_FORE_MAG     2   // �첽����������
// ͣ����ʽ
#define FUNCCODE_STOPMODE_DEC_STOP      0   // ����ͣ��
#define FUNCCODE_STOPMODE_FREESTOP      1   // ����ͣ��
#define FUNCCODE_STOPMODE_HURRY_STOP    2   // ��ͣͣ��
// Ƶ�ʵ�������Ƶ�����ж���
#define FUNCCODE_FRQLOWERDEAL_RUN_LOWER    0   // ������Ƶ������
#define FUNCCODE_FRQLOWERDEAL_DELAY_STOP   1   // ��ʱͣ��
#define FUNCCODE_FRQLOWERDEAL_RUN_ZERO     2   // ��������
// PID����Դ
#define FUNCCODE_PIDSETSELECT_FC               0   // �������趨
#define FUNCCODE_PIDSETSELECT_AI1              1   // AI1
#define FUNCCODE_PIDSETSELECT_AI2              2   // AI2
#define FUNCCODE_PIDSETSELECT_AI3              3   // AI3
#define FUNCCODE_PIDSETSELECT_PULSE            4   // PULSE
#define FUNCCODE_PIDSETSELECT_COMM             5   // ͨѶ
#define FUNCCODE_PIDSETSELECT_MULTI_SET        6   // ���ָ��
// PID�����л�����
#define FUNCCODE_PIDPARACHGCONDITION_NO         0   // ���л�
#define FUNCCODE_PIDPARACHGCONDITION_DI         1   // DI����
#define FUNCCODE_PIDPARACHGCONDITION_PID_ERROR  2   // ����ƫ���Զ��л�
// PID����Դ
#define FUNCCODE_PIDFDBSELECT_AI1              0   // AI1
#define FUNCCODE_PIDFDBSELECT_AI2              1   // AI2
#define FUNCCODE_PIDFDBSELECT_AI3              2   // AI3
#define FUNCCODE_PIDFDBSELECT_AI1_SUB_AI2      3   // AI1-AI2
#define FUNCCODE_PIDFDBSELECT_PULSE            4   // PULSE
#define FUNCCODE_PIDFDBSELECT_COMM             5   // ͨѶ
#define FUNCCODE_PIDFDBSELECT_AI1_ADD_AI2      6   // AI1+AI2
#define FUNCCODE_PIDFDBSELECT_MAX_AI           7   // MAX(|AI1|, |AI2|)
#define FUNCCODE_PIDFDBSELECT_MIN_AI           8   // MIN(|AI1|, |AI2|)
// PID����ģʽ
#define FUNCCODE_PIDCALCMODE_NO             0   // ͣ��ʱ������
#define FUNCCODE_PIDCALCMODE_YES            1   // ͣ��ʱ����
// MK������
#define FUNCCODE_MFKKEYFUNCTION_NONE        0   // MF.K��
#define FUNCCODE_MFKKEYFUNCTION_SWITCH      1   // ��������ͨ���л�
#define FUNCCODE_MFKKEYFUNCTION_REVERSE     2   // ����ת�л�
#define FUNCCODE_MFKKEYFUNCTION_FWD_JOG     3   // ��ת�㶯����
#define FUNCCODE_MFKKEYFUNCTION_REV_JOG     4   // ��ת�㶯����
// STOP/RES������
#define FUNCCODE_STOPKEYFUNCTION_KEYBOARD   0   // ͣ�����ܽ��ڼ��̿��Ʒ�ʽʱ��Ч
#define FUNCCODE_STOPKEYFUNCTION_ALL        1   // ����Ч
// ���ָ��0������ʽ
#define FUNCCODE_PLCFRQ0SELECT_FC          0   // ������FC-00����
#define FUNCCODE_PLCFRQ0SELECT_AI1         1   // AI1
#define FUNCCODE_PLCFRQ0SELECT_AI2         2   // AI2
#define FUNCCODE_PLCFRQ0SELECT_AI3         3   // AI3
#define FUNCCODE_PLCFRQ0SELECT_PULSE       4   // PULSE
#define FUNCCODE_PLCFRQ0SELECT_PID         5   // PID����
#define FUNCCODE_PLCFRQ0SELECT_PRESET_FRQ  6   // Ԥ��Ƶ��
// PLC���з�ʽ
#define FUNCCODE_PLCRUNMODE_ONCE_STOP   0   // �������н���ͣ��
#define FUNCCODE_PLCRUNMODE_ONCE_RUN    1   // �������н���������ֵ
#define FUNCCODE_PLCRUNMODE_REPEAT      2   // һֱѭ��
#define FUNCCODE_PLCPOWEROFFREMMODE_REM   1   // plc�������   0Ϊ������
#define FUNCCODE_PLCSTOPREMMODE_REM       1   // plcͣ������  0Ϊ������
//============================ʱ����صĶ���======================================//
// PLC����ʱ�䵥λ
#define FUNCCODE_PLCTIMEUNIT_H      1   // H(Сʱ)  0Ϊs��
// �������ʱ�䵥λ
#define TIME_UNIT_SEC_PER_HOUR          3600    // 1hour = 3600sec
#define TIME_UNIT_SEC_PER_MIN           60      // 1min  = 60sec
#define TIME_UNIT_MS_PER_SEC            1000    // 1s = 1000ms
#define TIME_UNIT_VF_VOL_ACC_TIME       100     // VF����ĵ�ѹ����ʱ��
#define TIME_UNIT_AI_PULSE_IN_FILTER    10      // AI,pulseIn�˲�ʱ��, ms
#define TIME_UNIT_DI_DELAY              100     // DI����ӳ�ʱ��, ms
#define TIME_UNIT_DO_DELAY              100     // DO����ӳ�ʱ��, ms
#define TIME_UNIT_START_FRQ_WAIT        100      // ����Ƶ�ʱ���ʱ�䣬ms
#define TIME_UNIT_START_BRAKE           100     // ����ֱ���ƶ�ʱ�䣬ms
#define TIME_UNIT_WAIT_STOP_BRAKE       100     // ͣ��ֱ���ƶ��ȴ�ʱ�䣬ms
#define TIME_UNIT_STOP_BRAKE            100     // ͣ��ֱ���ƶ�ʱ�䣬ms
#define TIME_UNIT_LOWER_STOP_DELAY      100     // Ƶ�ʵ�������Ƶ��ʱͣ�����ӳ�ʱ��
#define TIME_UNIT_PID_SET_CHANGE        10      // PID�����仯ʱ��
#define TIME_UNIT_PID_FILTER            10      // PID����������˲�ʱ��
#define TIME_UNIT_PID_INIT              10      // PID��ֵ����ʱ��
#define TIME_UNIT_PID_FDB_LOSE          100     // PID������ʧ���ʱ��
#define TIME_UNIT_SWING_PERIOD          100      // ��Ƶ����
#define TIME_UNIT_SCICOMMOVERTIME       100     // ͨѶ��ʱʱ��
#define TIME_UNIT_ERR_AUTO_RST_DELAY    100     // �����Զ���λ���ʱ�䣬ms
#define TIME_UNIT_ERR_AUTO_RST_CLR      (TIME_UNIT_SEC_PER_HOUR*100UL) // �����Զ���λ�������ʱ��, 0.1h
#define TIME_UNIT_P_OFF_VOL_BACK        10      // ˲ͣ��ͣ��ѹ�����ж�ʱ��
#define TIME_UNIT_PLC                   100     // PLC����ʱ�䵥λ
#define TIME_UNIT_WAKE_UP               100     // ����ʱ��ĵ�λ
#define TIME_UNIT_DORMANT               100     // ����ʱ��ĵ�λ
#define TIME_UNIT_CURRENT_CHK           10      // �������ʱ�䵥λ
//=====================================================================
#define ACC_DEC_T_INIT1  ((uint32_t)10*TIME_UNIT_MS_PER_SEC/TIME_UNIT_ACC_DEC_SPEED)   // �Ӽ���ʱ�����ֵ��20s������ <= 20
#define ACC_DEC_T_INIT2  ((uint32_t)30*TIME_UNIT_MS_PER_SEC/TIME_UNIT_ACC_DEC_SPEED)   // 50s������ > 20
#define RATING_FRQ_INIT_0   50      // ����Ƶ�ʣ�0��С����
#define RATING_FRQ_INIT_1   500     // ����Ƶ�ʣ�1��С����
#define RATING_FRQ_INIT_2   5000    // ����Ƶ�ʣ�2��С����
#define BAUD_NUM_MAX        12      // ������ѡ��Χ�����ֵ
#define MENU_MODE_MAX       3       // �˵�ģʽ���ֵ
//=====================================================================
// ��Щ������������������ĳ�������룬��������FunctionCode�е�index
#define FACTORY_PWD_INDEX            (CodeSerialNum(FunctionCodeVFD.codeVFD.FactoryPassword))   //  ��������
#define FUNCCODE_FACTORY_START_INDEX  (CodeSerialNum(FunctionCodeVFD.group.af[0]))           // af��Ŀ�ʼ
#define FUNCCODE_FACTORY_END_INDEX   (CodeSerialNum(FunctionCodeVFD.group.af[AFNUM - 1]))    // af��Ľ���
#define FUNCCODE_FACTORY1_START_INDEX  (CodeSerialNum(FunctionCodeVFD.group.ae[0]))           // ae��Ŀ�ʼ
#define FUNCCODE_FACTORY1_END_INDEX   (CodeSerialNum(FunctionCodeVFD.group.af[AENUM - 1]))    // ae��Ľ���
#define FC_MOTOR1_START_INDEX        (CodeSerialNum(FunctionCodeVFD.group.b0[0]))        // �����������ʼ
#define FC_MOTOR1_END_INDEX          (CodeSerialNum(FunctionCodeVFD.group.b0[B0NUM - 1]))   // ��������Ľ���
#define PRESET_FRQ_INDEX             (CodeSerialNum(FunctionCodeVFD.codeVFD.FrqNumerical))     //    ����Ƶ��
#define MAX_FRQ_INDEX                (CodeSerialNum(FunctionCodeVFD.codeVFD.MaxFrq))             //    ���Ƶ��
#define UPPER_FRQ_INDEX              (CodeSerialNum(FunctionCodeVFD.codeVFD.UpperFrqNumerical))       //   ����Ƶ��
#define LOWER_FRQ_INDEX              (CodeSerialNum(FunctionCodeVFD.codeVFD.lowerFrqNumerical))       //    ����Ƶ��
#define ACC_TIME1_INDEX              (CodeSerialNum(FunctionCodeVFD.codeVFD.AccTime1))           //    ����ʱ��1
#define DEC_TIME1_INDEX              (CodeSerialNum(FunctionCodeVFD.codeVFD.DecTime1))           //    ����ʱ��1
#define CARRIER_FRQ_INDEX            (CodeSerialNum(FunctionCodeVFD.codeVFD.CarrierFrq))         //    �ز�Ƶ��
#define RATING_POWER_INDEX           (CodeSerialNum(FunctionCodeVFD.codeVFD.ratingPower))        // ��������
#define RATING_VOL_INDEX             (CodeSerialNum(FunctionCodeVFD.codeVFD.ratingVoltage))      // ������ѹ
#define RATING_CUR_INDEX             (CodeSerialNum(FunctionCodeVFD.codeVFD.ratingCurrent))      // ��������
#define RATING_FRQ_INDEX             (CodeSerialNum(FunctionCodeVFD.codeVFD.ratingFrq))          // ����Ƶ��
#define ZERO_LOAD_CURRENT_INDEX      (CodeSerialNum(FunctionCodeVFD.codeVFD.zeroLoadCurrent))    // ���ص���
#define TUNE_CMD_INDEX               (CodeSerialNum(FunctionCodeVFD.codeVFD.tuneCmd))        // ��г
#define VC_CHG_FRQ1_INDEX            (CodeSerialNum(FunctionCodeVFD.codeVFD.vcSpdLoopChgFrq1))  // ʸ�������ٶȻ� �л�Ƶ��1
#define VC_CHG_FRQ2_INDEX            (CodeSerialNum(FunctionCodeVFD.codeVFD.vcSpdLoopChgFrq2))  // ʸ�������ٶȻ� �л�Ƶ��2
#define OV_POINT_SET_INDEX           (CodeSerialNum(FunctionCodeVFD.codeVFD.OverVoltagePointSet))  // ��ѹ������
#define CURVE1_MIN_INDEX             (CodeSerialNum(FunctionCodeVFD.codeVFD.CurveSet2P1[0]))       //   AI1��С����
#define CURVE1_MAX_INDEX             (CodeSerialNum(FunctionCodeVFD.codeVFD.CurveSet2P1[2]))       //   AI1�������
#define CURVE2_MIN_INDEX             (CodeSerialNum(FunctionCodeVFD.codeVFD.CurveSet2P2[0]))       //   AI2��С����
#define CURVE2_MAX_INDEX             (CodeSerialNum(FunctionCodeVFD.codeVFD.CurveSet2P2[2]))       //   AI2�������
#define CURVE3_MIN_INDEX             (CodeSerialNum(FunctionCodeVFD.codeVFD.CurveSet2P3[0]))       //   AI3��С����
#define CURVE3_MAX_INDEX             (CodeSerialNum(FunctionCodeVFD.codeVFD.CurveSet2P3[2]))       //   AI3�������
#define PULSE_IN_MIN_INDEX           (CodeSerialNum(FunctionCodeVFD.codeVFD.CurveSet2P4[0]))       //   PULSE��С����
#define PULSE_IN_MAX_INDEX           (CodeSerialNum(FunctionCodeVFD.codeVFD.CurveSet2P4[2]))       //   PULSE�������
#define CURVE4_MIN_INDEX             (CodeSerialNum(FunctionCodeVFD.codeVFD.CurveSet4P1[0]))       //   AI4��С����
#define CURVE4_INFLEX1_INDEX         (CodeSerialNum(FunctionCodeVFD.codeVFD.CurveSet4P1[2]))       //   AI4�յ�1����
#define CURVE4_INFLEX2_INDEX         (CodeSerialNum(FunctionCodeVFD.codeVFD.CurveSet4P1[4]))       //   AI4�յ�2����
#define CURVE4_MAX_INDEX             (CodeSerialNum(FunctionCodeVFD.codeVFD.CurveSet4P1[6]))       //   AI4�������
#define CURVE5_MIN_INDEX             (CodeSerialNum(FunctionCodeVFD.codeVFD.CurveSet4P2[0]))       //   AI5��С����
#define CURVE5_INFLEX1_INDEX         (CodeSerialNum(FunctionCodeVFD.codeVFD.CurveSet4P2[2]))       //   AI5�յ�1����
#define CURVE5_INFLEX2_INDEX         (CodeSerialNum(FunctionCodeVFD.codeVFD.CurveSet4P2[4]))       //   AI5�յ�2����
#define CURVE5_MAX_INDEX             (CodeSerialNum(FunctionCodeVFD.codeVFD.CurveSet4P2[6]))       //   AI5�������
#define ACC_TIME2_INDEX              (CodeSerialNum(FunctionCodeVFD.codeVFD.AccTime2))       //  ����ʱ��2
#define DEC_TIME2_INDEX              (CodeSerialNum(FunctionCodeVFD.codeVFD.DecTime2))       //  ����ʱ��2
#define ACC_TIME3_INDEX              (CodeSerialNum(FunctionCodeVFD.codeVFD.AccTime3))       //  ����ʱ��3
#define DEC_TIME3_INDEX              (CodeSerialNum(FunctionCodeVFD.codeVFD.DecTime3))       //  ����ʱ3
#define ACC_TIME4_INDEX              (CodeSerialNum(FunctionCodeVFD.codeVFD.AccTime4))       //  ����ʱ��4
#define DEC_TIME4_INDEX              (CodeSerialNum(FunctionCodeVFD.codeVFD.DecTime4))       //  ����ʱ��4
#define RUN_TIME_ADDUP_INDEX         (CodeSerialNum(FunctionCodeVFD.codeVFD.RunTimeAddup))     //   �ۼ�����ʱ��
#define POWER_TIME_ADDUP_INDEX       (CodeSerialNum(FunctionCodeVFD.codeVFD.PowerUpTimeAddup)) //   �ۼ��ϵ�ʱ��
#define POWER_ADDUP_INDEX            (CodeSerialNum(FunctionCodeVFD.codeVFD.PowerAddup))       //   �ۼƺĵ���
#define AI1_LIMIT                    (CodeSerialNum(FunctionCodeVFD.codeVFD.Ai1VoltageLimit))   //    AI��������
#define AI1_UPPER                    (CodeSerialNum(FunctionCodeVFD.codeVFD.Ai1VoltageUpper))   //    AI��������
#define PID_PARA_CHG_DELTA1_MAX      (CodeSerialNum(FunctionCodeVFD.codeVFD.PidParaChgDelta2))  //   PID�����л�ƫ��2
#define PID_PARA_CHG_DELTA2_MIN      (CodeSerialNum(FunctionCodeVFD.codeVFD.PidParaChgDelta1))  //   PID�����л�ƫ��1
#define DORMANT_UPPER                (CodeSerialNum(FunctionCodeVFD.codeVFD.WakeUpFrq))         // ����Ƶ������
#define WAKE_UP_LIMIT                (CodeSerialNum(FunctionCodeVFD.codeVFD.DormantFrq))        // ����Ƶ������
#define LAST_ERROR_RECORD_INDEX      (CodeSerialNum(FunctionCodeVFD.codeVFD.errorScene1.allVFD[sizeof(struct ERROR_SCENE_STRUCT) - 1]))  // ���һ�����ϼ�¼
#define MIN_CBC_TIME_INDEX           (CodeSerialNum(FunctionCodeVFD.codeVFD.cbcMinTime))         //   ������ʱ������
#define MAX_CBC_TIME_INDEX           (CodeSerialNum(FunctionCodeVFD.codeVFD.cbcMaxTime))         //   ������ʱ������
#define EEPROM_CHECK_INDEX1          (CodeSerialNum(FunctionCodeVFD.codeVFD.EepromCheckWord1))
#define EEPROM_CHECK_INDEX2          (CodeSerialNum(FunctionCodeVFD.codeVFD.EepromCheckWord2))
#define SAVE_USER_PARA_PARA1         (CodeSerialNum(FunctionCodeVFD.codeVFD.SaveUserParaFlag1))
#define SAVE_USER_PARA_PARA2         (CodeSerialNum(FunctionCodeVFD.codeVFD.SaveUserParaFlag2))
//-------------------------------
#define FC_GROUP_FACTORY            FUNCCODE_GROUP_AF   // ���Ҳ�����
#define FC_START_GROUP              FUNCCODE_GROUP_A0   // ����������ʾ�ĵ�1��
//--------------------------------
extern FUNCTIONCODE_ALL_VFD FunctionCodeVFD;
extern const uint16_t OverVoltageInitValue[];
extern uint16_t FunctionCodeGradeCurMenuMode[];
#endif
/********************************************************************************************************/
