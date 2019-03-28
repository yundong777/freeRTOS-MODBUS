/*
*********************************************************************************************************
*
*   ģ������ : ���棺��������1 601
*   �ļ����� : gui_set_601.c
*   ��    �� : V1.0
*   ˵    �� : ��������1���� ����601
*
*********************************************************************************************************
*/
#include "includes.h"
#include "MainTask.h"
void _cbDialog_set_601(WM_MESSAGE * pMsg)
{
    WM_HWIN hItem;
    int     NCode;
    int     Id;
    uint16_t sendBuf[4];
    switch (pMsg->MsgId)
    {
    case WM_UPDATE:
        //A0.11--A0.21  �����������
        sendBuf[0] = 3;
        sendBuf[1] = 0xA00B;
        sendBuf[2] = 11;
        sendBuf[3] = funcCode.code.address / 100;
        enqueue(sendBuf);
        //
        // ���Ƶ��
        //
        hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_0);
        EDIT_SetFloatMode(hItem,(float)FunctionCodeVFD.codeVFD.MaxFrq/
                          pow(10,funcCodeAttributeVFD[CodeSerialNum(FunctionCodeVFD.group.a0[11])].attribute.bitVFD.point),
                          50,320,funcCodeAttributeVFD[CodeSerialNum(FunctionCodeVFD.group.a0[11])].attribute.bitVFD.point,0);
        //
        // ����Ƶ��
        //
        hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_1);
        EDIT_SetFloatMode(hItem,(float)FunctionCodeVFD.codeVFD.UpperFrqNumerical/
                          pow(10,funcCodeAttributeVFD[CodeSerialNum(FunctionCodeVFD.group.a0[13])].attribute.bitVFD.point),
                          0,320,funcCodeAttributeVFD[CodeSerialNum(FunctionCodeVFD.group.a0[13])].attribute.bitVFD.point,0);
        //
        // ����Ƶ��
        //
        hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_2);
        EDIT_SetFloatMode(hItem,(float)FunctionCodeVFD.codeVFD.lowerFrqNumerical/
                          pow(10,funcCodeAttributeVFD[CodeSerialNum(FunctionCodeVFD.group.a0[14])].attribute.bitVFD.point),
                          0,320,funcCodeAttributeVFD[CodeSerialNum(FunctionCodeVFD.group.a0[14])].attribute.bitVFD.point,0);
        EDIT_SetFont(hItem, GUI_FONT_32_ASCII);
        EDIT_SetTextAlign(hItem, GUI_TA_LEFT | GUI_TA_VCENTER);
        //
        // �ز�Ƶ��
        //
        hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_3);
        EDIT_SetFloatMode(hItem,(float)FunctionCodeVFD.codeVFD.CarrierFrq/
                          pow(10,funcCodeAttributeVFD[CodeSerialNum(FunctionCodeVFD.group.a0[16])].attribute.bitVFD.point),
                          0.5,16,funcCodeAttributeVFD[CodeSerialNum(FunctionCodeVFD.group.a0[16])].attribute.bitVFD.point,0);
        //
        // ����ʱ��1
        //
        hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_4);
        EDIT_SetFloatMode(hItem,(float)FunctionCodeVFD.codeVFD.AccTime1/
                          pow(10,funcCodeAttributeVFD[CodeSerialNum(FunctionCodeVFD.group.a0[20])].attribute.bitVFD.point),
                          0,6500,funcCodeAttributeVFD[CodeSerialNum(FunctionCodeVFD.group.a0[20])].attribute.bitVFD.point,0);
        //
        // ����ʱ��1
        //
        hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_5);
        EDIT_SetFloatMode(hItem,(float)FunctionCodeVFD.codeVFD.DecTime1/
                          pow(10,funcCodeAttributeVFD[CodeSerialNum(FunctionCodeVFD.group.a0[21])].attribute.bitVFD.point),
                          0,6500,funcCodeAttributeVFD[CodeSerialNum(FunctionCodeVFD.group.a0[21])].attribute.bitVFD.point,0);
        break;
    case WM_INIT_DIALOG:
        hItem = pMsg->hWin;
        WINDOW_SetBkColor(hItem, GUI_MAKE_COLOR(bkColor));
        //
        // ��һҳ
        //
        hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_0);
        WM_SetHasTrans(hItem);
        WM_SetCallback(hItem, _cbButtonPreviousPage);
        BUTTON_SetText(hItem, "");
        //
        // ��һҳ
        //
        hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_1);
        WM_SetHasTrans(hItem);
        WM_SetCallback(hItem, _cbButtonNextPage);
        BUTTON_SetText(hItem, "");
        //
        // ����
        //
        hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_2);
        WM_SetHasTrans(hItem);
        WM_SetCallback(hItem, _cbButtonBackMenu);
        BUTTON_SetText(hItem, "");
        //
        // ����������
        //
        hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_3);
        WM_SetHasTrans(hItem);
        WM_SetCallback(hItem, _cbButtonBackHome);
        BUTTON_SetText(hItem, "");
        //
        // ������ѡ��
        //
        hItem = WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_0);
        DROPDOWN_SetFont(hItem, &GUI_FontHZ32);
        DROPDOWN_AddString(hItem, "ȫ������");      //0
        DROPDOWN_AddString(hItem, "��������");      //1
        DROPDOWN_AddString(hItem, "�������");      //2
        DROPDOWN_AddString(hItem, "�ٶȻ�����");    //3
        DROPDOWN_AddString(hItem, "����������");    //4
        DROPDOWN_AddString(hItem, "���ϼ�¼");      //5
        DROPDOWN_AddString(hItem, "���Ӳ���");      //6
        DROPDOWN_SetSel(hItem, 1);                                      //���ùر�״̬�ĵ�ǰѡ������
        DROPDOWN_SetSelExp(hItem, 1);                                   //����չ��״̬�ĵ�ǰѡ������
        DROPDOWN_SetTextHeight(hItem, 50);                              //���ùر�״̬�ı��߶�
        DROPDOWN_SetTextAlign(hItem, GUI_TA_LEFT | GUI_TA_VCENTER);     //���ùر�״̬�ı����뷽ʽ
        DROPDOWN_SetListHeight(hItem, 370);                             //���������б�ĸ߶�
        DROPDOWN_SetItemSpacing(hItem, 20);                             //���������б��и���Ŀ֮��ļ��
        //
        // ���Ƶ��
        //
        hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_0);
        TEXT_SetFont(hItem, &GUI_FontHZ32);
        TEXT_SetTextAlign(hItem, GUI_TA_LEFT | GUI_TA_VCENTER);
        //
        // ����Ƶ��
        //
        hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_1);
        TEXT_SetFont(hItem, &GUI_FontHZ32);
        TEXT_SetTextAlign(hItem, GUI_TA_LEFT | GUI_TA_VCENTER);
        //
        // ����Ƶ��
        //
        hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_2);
        TEXT_SetFont(hItem, &GUI_FontHZ32);
        TEXT_SetTextAlign(hItem, GUI_TA_LEFT | GUI_TA_VCENTER);
        //
        // �ز�Ƶ��
        //
        hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_3);
        TEXT_SetFont(hItem, &GUI_FontHZ32);
        TEXT_SetTextAlign(hItem, GUI_TA_LEFT | GUI_TA_VCENTER);
        //
        // ����ʱ��1
        //
        hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_4);
        TEXT_SetFont(hItem, &GUI_FontHZ32);
        TEXT_SetTextAlign(hItem, GUI_TA_LEFT | GUI_TA_VCENTER);
        //
        // ����ʱ��1
        //
        hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_5);
        TEXT_SetFont(hItem, &GUI_FontHZ32);
        TEXT_SetTextAlign(hItem, GUI_TA_LEFT | GUI_TA_VCENTER);
        //
        // ���Ƶ��
        //
        hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_0);
        EDIT_SetFloatMode(hItem,(float)FunctionCodeVFD.codeVFD.MaxFrq/
                          pow(10,funcCodeAttributeVFD[CodeSerialNum(FunctionCodeVFD.group.a0[11])].attribute.bitVFD.point),
                          50,320,funcCodeAttributeVFD[CodeSerialNum(FunctionCodeVFD.group.a0[11])].attribute.bitVFD.point,0);
        EDIT_SetFont(hItem, GUI_FONT_32_ASCII);
        EDIT_SetTextAlign(hItem, GUI_TA_LEFT | GUI_TA_VCENTER);
        //
        // ����Ƶ��
        //
        hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_1);
        EDIT_SetFloatMode(hItem,(float)FunctionCodeVFD.codeVFD.UpperFrqNumerical/
                          pow(10,funcCodeAttributeVFD[CodeSerialNum(FunctionCodeVFD.group.a0[13])].attribute.bitVFD.point),
                          0,320,funcCodeAttributeVFD[CodeSerialNum(FunctionCodeVFD.group.a0[13])].attribute.bitVFD.point,0);
        EDIT_SetFont(hItem, GUI_FONT_32_ASCII);
        EDIT_SetTextAlign(hItem, GUI_TA_LEFT | GUI_TA_VCENTER);
        //
        // ����Ƶ��
        //
        hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_2);
        EDIT_SetFloatMode(hItem,(float)FunctionCodeVFD.codeVFD.lowerFrqNumerical/
                          pow(10,funcCodeAttributeVFD[CodeSerialNum(FunctionCodeVFD.group.a0[14])].attribute.bitVFD.point),
                          0,320,funcCodeAttributeVFD[CodeSerialNum(FunctionCodeVFD.group.a0[14])].attribute.bitVFD.point,0);
        EDIT_SetFont(hItem, GUI_FONT_32_ASCII);
        EDIT_SetTextAlign(hItem, GUI_TA_LEFT | GUI_TA_VCENTER);
        //
        // �ز�Ƶ��
        //
        hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_3);
        EDIT_SetFloatMode(hItem,(float)FunctionCodeVFD.codeVFD.CarrierFrq/
                          pow(10,funcCodeAttributeVFD[CodeSerialNum(FunctionCodeVFD.group.a0[16])].attribute.bitVFD.point),
                          0.5,16,funcCodeAttributeVFD[CodeSerialNum(FunctionCodeVFD.group.a0[16])].attribute.bitVFD.point,0);
        EDIT_SetFont(hItem, GUI_FONT_32_ASCII);
        EDIT_SetTextAlign(hItem, GUI_TA_LEFT | GUI_TA_VCENTER);
        //
        // ����ʱ��1
        //
        hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_4);
        EDIT_SetFloatMode(hItem,(float)FunctionCodeVFD.codeVFD.AccTime1/
                          pow(10,funcCodeAttributeVFD[CodeSerialNum(FunctionCodeVFD.group.a0[20])].attribute.bitVFD.point),
                          0,6500,funcCodeAttributeVFD[CodeSerialNum(FunctionCodeVFD.group.a0[20])].attribute.bitVFD.point,0);
        EDIT_SetFont(hItem, GUI_FONT_32_ASCII);
        EDIT_SetTextAlign(hItem, GUI_TA_LEFT | GUI_TA_VCENTER);
        //
        // ����ʱ��1
        //
        hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_5);
        EDIT_SetFloatMode(hItem,(float)FunctionCodeVFD.codeVFD.DecTime1/
                          pow(10,funcCodeAttributeVFD[CodeSerialNum(FunctionCodeVFD.group.a0[21])].attribute.bitVFD.point),
                          0,6500,funcCodeAttributeVFD[CodeSerialNum(FunctionCodeVFD.group.a0[21])].attribute.bitVFD.point,0);
        EDIT_SetFont(hItem, GUI_FONT_32_ASCII);
        EDIT_SetTextAlign(hItem, GUI_TA_LEFT | GUI_TA_VCENTER);
        //
        // ҳ��
        //
        hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_6);
        TEXT_SetFont(hItem, GUI_FONT_24B_1);
        TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
        TEXT_SetText(hItem, "1/2");
        break;
    case WM_NOTIFY_PARENT:
        Id    = WM_GetId(pMsg->hWinSrc);
        NCode = pMsg->Data.v;
        if(NCode == WM_NOTIFICATION_RELEASED && errorFlag == 0)//�ؼ��ͷţ���������һ��
        {
            BEEP_KeyTone();
        }
        switch(Id)
        {
        case ID_BUTTON_0: // ��һҳ
            switch(NCode)
            {
            case WM_NOTIFICATION_CLICKED:
                break;
            case WM_NOTIFICATION_RELEASED:
                if(keyFlag == 0)
                {
                    GUI=0x602;
                }
                break;
            }
            break;
        case ID_BUTTON_1: // ��һҳ
            switch(NCode)
            {
            case WM_NOTIFICATION_CLICKED:
                break;
            case WM_NOTIFICATION_RELEASED:
                if(keyFlag == 0)
                {
                    GUI=0x602;
                }
                break;
            }
            break;
        case ID_BUTTON_2: // ����
            switch(NCode)
            {
            case WM_NOTIFICATION_CLICKED:
                break;
            case WM_NOTIFICATION_RELEASED:
                if(keyFlag == 0)
                {
                    GUI=0x001;
                }
                break;
            }
            break;
        case ID_BUTTON_3: // ����������
            switch(NCode)
            {
            case WM_NOTIFICATION_CLICKED:
                break;
            case WM_NOTIFICATION_RELEASED:
                if(keyFlag == 0)
                {
                    GUI=0x000;
                }
                break;
            }
            break;
        case ID_DROPDOWN_0: // ������ѡ��
            switch(NCode)
            {
            case WM_NOTIFICATION_CLICKED:
                break;
            case WM_NOTIFICATION_RELEASED:
                break;
            case WM_NOTIFICATION_SEL_CHANGED:
                EDIT = 6;
                hItem = WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_0);
                Index = DROPDOWN_GetSel(hItem);
                if(Index == 0 && keyFlag == 0)
                {
                    hWinKey002=GUI_CreateDialogBox(_aDialogCreate_key_002, countKey002, &_cbDialog_key_002, 0, 0, 0);
                    memcpy(gui_key1.key_value_buf,"\0\0\0\0\0\0\0\0\0",9);
                    EDIT_SetText(WM_GetDialogItem(hWinKey002,ID_EDIT_0),gui_key1.key_value_buf);
                    TEXT_SetText(WM_GetDialogItem(hWinKey002,ID_TEXT_2), "���������룺");
                    TEXT_SetText(WM_GetDialogItem(hWinKey002,ID_TEXT_0), "");
                    TEXT_SetText(WM_GetDialogItem(hWinKey002,ID_TEXT_1), "");
                    WM_DisableWindow(WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_0));
                    keyFlag = 1;
                }
                else if(Index == 2 && keyFlag == 0)
                {
                    GUI=0x603;
                }
                else if(Index == 3 && keyFlag == 0)
                {
                    hWinKey002=GUI_CreateDialogBox(_aDialogCreate_key_002, countKey002, &_cbDialog_key_002, 0, 0, 0);
                    memcpy(gui_key1.key_value_buf,"\0\0\0\0\0\0\0\0\0",9);
                    EDIT_SetText(WM_GetDialogItem(hWinKey002,ID_EDIT_0),gui_key1.key_value_buf);
                    TEXT_SetText(WM_GetDialogItem(hWinKey002,ID_TEXT_2), "���������룺");
                    TEXT_SetText(WM_GetDialogItem(hWinKey002,ID_TEXT_0), "");
                    TEXT_SetText(WM_GetDialogItem(hWinKey002,ID_TEXT_1), "");
                    WM_DisableWindow(WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_0));
                    keyFlag = 1;
                }
                else if(Index == 4 && keyFlag == 0)
                {
                    hWinKey002=GUI_CreateDialogBox(_aDialogCreate_key_002, countKey002, &_cbDialog_key_002, 0, 0, 0);
                    memcpy(gui_key1.key_value_buf,"\0\0\0\0\0\0\0\0\0",9);
                    EDIT_SetText(WM_GetDialogItem(hWinKey002,ID_EDIT_0),gui_key1.key_value_buf);
                    TEXT_SetText(WM_GetDialogItem(hWinKey002,ID_TEXT_2), "���������룺");
                    TEXT_SetText(WM_GetDialogItem(hWinKey002,ID_TEXT_0), "");
                    TEXT_SetText(WM_GetDialogItem(hWinKey002,ID_TEXT_1), "");
                    WM_DisableWindow(WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_0));
                    keyFlag = 1;
                }
                else if(Index == 5 && keyFlag == 0)
                {
                    GUI=0x609;
                }
                else if(Index == 6 && keyFlag == 0)
                {
                    GUI=0x613;
                }
                break;
            }
            break;
        case ID_EDIT_0: // ���Ƶ��
            switch(NCode)
            {
            case WM_NOTIFICATION_CLICKED:
                break;
            case WM_NOTIFICATION_RELEASED:
                if(keyFlag == 0)
                {
                    EDIT = 0;
                    hWinKey002=GUI_CreateDialogBox(_aDialogCreate_key_002, countKey002, &_cbDialog_key_002, 0, 0, 0);
                    sprintf(gui_key1.key_value_buf,"%.2f",(float)FunctionCodeVFD.codeVFD.MaxFrq/
                            pow(10,funcCodeAttributeVFD[CodeSerialNum(FunctionCodeVFD.group.a0[11])].attribute.bitVFD.point));
                    gui_key1.order += strlen(gui_key1.key_value_buf);
                    EDIT_SetText(WM_GetDialogItem(hWinKey002,ID_EDIT_0),gui_key1.key_value_buf);
                    TEXT_SetText(WM_GetDialogItem(hWinKey002,ID_TEXT_2), "���Ƶ�ʣ�");
                    TEXT_SetText(WM_GetDialogItem(hWinKey002,ID_TEXT_0), "Min:����Ƶ��");
                    TEXT_SetText(WM_GetDialogItem(hWinKey002,ID_TEXT_1), "Max:320Hz");
                    WM_DisableWindow(WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_0));
                    keyFlag = 1;
                }
                break;
            case WM_NOTIFICATION_VALUE_CHANGED:
                break;
            }
            break;
        case ID_EDIT_1: // ����Ƶ��
            switch(NCode)
            {
            case WM_NOTIFICATION_CLICKED:
                break;
            case WM_NOTIFICATION_RELEASED:
                if(keyFlag == 0)
                {
                    EDIT = 1;
                    hWinKey002=GUI_CreateDialogBox(_aDialogCreate_key_002, countKey002, &_cbDialog_key_002, 0, 0, 0);
                    sprintf(gui_key1.key_value_buf,"%.2f",(float)FunctionCodeVFD.codeVFD.UpperFrqNumerical/
                            pow(10,funcCodeAttributeVFD[CodeSerialNum(FunctionCodeVFD.group.a0[13])].attribute.bitVFD.point));
                    gui_key1.order += strlen(gui_key1.key_value_buf);
                    EDIT_SetText(WM_GetDialogItem(hWinKey002,ID_EDIT_0),gui_key1.key_value_buf);
                    TEXT_SetText(WM_GetDialogItem(hWinKey002,ID_TEXT_2), "����Ƶ�ʣ�");
                    TEXT_SetText(WM_GetDialogItem(hWinKey002,ID_TEXT_0), "Min:����Ƶ��");
                    TEXT_SetText(WM_GetDialogItem(hWinKey002,ID_TEXT_1), "Max:���Ƶ��");
                    WM_DisableWindow(WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_0));
                    keyFlag = 1;
                }
                break;
            case WM_NOTIFICATION_VALUE_CHANGED:
                break;
            }
            break;
        case ID_EDIT_2: // ����Ƶ��
            switch(NCode)
            {
            case WM_NOTIFICATION_CLICKED:
                break;
            case WM_NOTIFICATION_RELEASED:
                if(keyFlag == 0)
                {
                    EDIT = 2;
                    hWinKey002=GUI_CreateDialogBox(_aDialogCreate_key_002, countKey002, &_cbDialog_key_002, 0, 0, 0);
                    sprintf(gui_key1.key_value_buf,"%.2f",(float)FunctionCodeVFD.codeVFD.lowerFrqNumerical/
                            pow(10,funcCodeAttributeVFD[CodeSerialNum(FunctionCodeVFD.group.a0[14])].attribute.bitVFD.point));
                    gui_key1.order += strlen(gui_key1.key_value_buf);
                    EDIT_SetText(WM_GetDialogItem(hWinKey002,ID_EDIT_0),gui_key1.key_value_buf);
                    TEXT_SetText(WM_GetDialogItem(hWinKey002,ID_TEXT_2), "����Ƶ�ʣ�");
                    TEXT_SetText(WM_GetDialogItem(hWinKey002,ID_TEXT_0), "Min:0Hz");
                    TEXT_SetText(WM_GetDialogItem(hWinKey002,ID_TEXT_1), "Max:����Ƶ��");
                    WM_DisableWindow(WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_0));
                    keyFlag = 1;
                }
                break;
            case WM_NOTIFICATION_VALUE_CHANGED:
                break;
            }
            break;
        case ID_EDIT_3: // �ز�Ƶ��
            switch(NCode)
            {
            case WM_NOTIFICATION_CLICKED:
                break;
            case WM_NOTIFICATION_RELEASED:
                if(keyFlag == 0)
                {
                    EDIT = 3;
                    hWinKey002=GUI_CreateDialogBox(_aDialogCreate_key_002, countKey002, &_cbDialog_key_002, 0, 0, 0);
                    sprintf(gui_key1.key_value_buf,"%.1f",(float)FunctionCodeVFD.codeVFD.CarrierFrq/
                            pow(10,funcCodeAttributeVFD[CodeSerialNum(FunctionCodeVFD.group.a0[16])].attribute.bitVFD.point));
                    gui_key1.order += strlen(gui_key1.key_value_buf);
                    EDIT_SetText(WM_GetDialogItem(hWinKey002,ID_EDIT_0),gui_key1.key_value_buf);
                    TEXT_SetText(WM_GetDialogItem(hWinKey002,ID_TEXT_2), "�ز�Ƶ�ʣ�");
                    TEXT_SetText(WM_GetDialogItem(hWinKey002,ID_TEXT_0), "Min:0.5kHz");
                    TEXT_SetText(WM_GetDialogItem(hWinKey002,ID_TEXT_1), "Max:16kHz");
                    WM_DisableWindow(WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_0));
                    keyFlag = 1;
                }
                break;
            case WM_NOTIFICATION_VALUE_CHANGED:
                break;
            }
            break;
        case ID_EDIT_4: // ����ʱ��1
            switch(NCode)
            {
            case WM_NOTIFICATION_CLICKED:
                break;
            case WM_NOTIFICATION_RELEASED:
                if(keyFlag == 0)
                {
                    EDIT = 4;
                    hWinKey002=GUI_CreateDialogBox(_aDialogCreate_key_002, countKey002, &_cbDialog_key_002, 0, 0, 0);
                    sprintf(gui_key1.key_value_buf,"%.1f",(float)FunctionCodeVFD.codeVFD.AccTime1/
                            pow(10,funcCodeAttributeVFD[CodeSerialNum(FunctionCodeVFD.group.a0[20])].attribute.bitVFD.point));
                    gui_key1.order += strlen(gui_key1.key_value_buf);
                    EDIT_SetText(WM_GetDialogItem(hWinKey002,ID_EDIT_0),gui_key1.key_value_buf);
                    TEXT_SetText(WM_GetDialogItem(hWinKey002,ID_TEXT_2), "����ʱ��1��");
                    TEXT_SetText(WM_GetDialogItem(hWinKey002,ID_TEXT_0), "Min:0s");
                    TEXT_SetText(WM_GetDialogItem(hWinKey002,ID_TEXT_1), "Max:6500s");
                    WM_DisableWindow(WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_0));
                    keyFlag = 1;
                }
                break;
            case WM_NOTIFICATION_VALUE_CHANGED:
                break;
            }
            break;
        case ID_EDIT_5: // ����ʱ��1
            switch(NCode)
            {
            case WM_NOTIFICATION_CLICKED:
                break;
            case WM_NOTIFICATION_RELEASED:
                if(keyFlag == 0)
                {
                    EDIT = 5;
                    hWinKey002=GUI_CreateDialogBox(_aDialogCreate_key_002, countKey002, &_cbDialog_key_002, 0, 0, 0);
                    sprintf(gui_key1.key_value_buf,"%.1f",(float)FunctionCodeVFD.codeVFD.DecTime1/
                            pow(10,funcCodeAttributeVFD[CodeSerialNum(FunctionCodeVFD.group.a0[21])].attribute.bitVFD.point));
                    gui_key1.order += strlen(gui_key1.key_value_buf);
                    EDIT_SetText(WM_GetDialogItem(hWinKey002,ID_EDIT_0),gui_key1.key_value_buf);
                    TEXT_SetText(WM_GetDialogItem(hWinKey002,ID_TEXT_2), "����ʱ��1��");
                    TEXT_SetText(WM_GetDialogItem(hWinKey002,ID_TEXT_0), "Min:0s");
                    TEXT_SetText(WM_GetDialogItem(hWinKey002,ID_TEXT_1), "Max:6500s");
                    WM_DisableWindow(WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_0));
                    keyFlag = 1;
                }
                break;
            case WM_NOTIFICATION_VALUE_CHANGED:
                break;
            }
            break;
        }
        break;
    default:
        WM_DefaultProc(pMsg);
        break;
    }
}
/********************************************************************************************************/
