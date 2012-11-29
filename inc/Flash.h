#include "MyDef.h"


__flash unsigned char fNoIzmI[] = "I1= --- ";

__flash unsigned char LCDavarNoUSP[]= "��� ����� � ���     ";
__flash unsigned char fNoReadVers[]	= "  �������������     ";

__flash unsigned char Menu21[] = "1.������.";
__flash unsigned char Menu22[] = "2.����/�����.";
__flash unsigned char Menu23[] = "3.����������.";
__flash unsigned char Menu24[] = "4.����������.";
__flash unsigned char Menu25[] = "5.�������� �����.";
__flash unsigned  __flash char *Menu2point[] = 
{
	Menu21, 
	Menu22, 
	Menu23, 
	Menu24,
	Menu25
};

__flash unsigned char Menu31[] = "1. ����.            ";
__flash unsigned char Menu32[] = "2. �����.           ";

__flash unsigned char Menu51[] = "1.�����.";
__flash unsigned char Menu52[] = "2.���������.";
__flash unsigned char Menu53[] = "3.������.";
__flash unsigned char Menu54[] = "4.����.";
__flash unsigned __flash char *Menu5point[] = {Menu51, Menu52, Menu53, Menu54};

__flash unsigned char Menu61[] = ".������.";
__flash unsigned char Menu62[] = ".��������.";
__flash unsigned char Menu63[] = ".����������.";
__flash unsigned char Menu64[] = ".�����.";
__flash unsigned __flash char *Menu6point[] = {Menu61, Menu62, Menu63, Menu64}; //������ ����� ��� ���� �������� ����������/ ���������� ���������

//��������� �����
#define MAX_DEF_PARAM (sizeof(Menu7paramPOST) / sizeof(Menu7paramPOST[0]))
__flash unsigned char Menu70[] = "��� ������          ";
__flash unsigned char Menu71[] = "��� �����           ";
__flash unsigned char Menu72[] = "���. ����� ��� ���. ";
__flash unsigned char Menu73[] = "����.�������� � ���.";
__flash unsigned char Menu74[] = "���������� ���������";
__flash unsigned char Menu75[] = "����.��������. ��   ";
__flash unsigned char Menu76[] = "����� ������� �� �� ";
__flash unsigned char Menu77[] = "��� ���������       ";
__flash unsigned __flash char *Menu7paramPOST[] = 
{
	Menu70, 
	Menu71, 
	Menu72, 
	Menu73, 
	Menu74, 
	Menu75, 
	Menu76, 
	Menu77
};
//�������� �������� ��� �����
__flash unsigned char Menu13Shift0[] =	"������";
__flash unsigned char Menu13Shift1[] =	"������";
__flash unsigned char Menu13Shift2[] =	"0..99";
__flash unsigned char Menu13Shift3[] =	"0..18";
__flash unsigned char Menu13Shift4[] =	"0..54";
__flash unsigned char Menu13Shift5[] =	"0..20";
__flash unsigned char Menu13Shift6[] =	"0..20";
__flash unsigned char Menu13Shift7[] =	"������";
__flash unsigned __flash char *Menu13ParamRange[] = 
{
	Menu13Shift0, 
	Menu13Shift1, 
	Menu13Shift2, 
	Menu13Shift3, 
	Menu13Shift4, 
	Menu13Shift5, 
	Menu13Shift6,
	Menu13Shift7
};

//��������� ���������
__flash unsigned char Menu81[] = "����� ���������     ";
__flash unsigned char Menu82[] = "�������� �� ����. ?�";
__flash unsigned char Menu83[] = "����. �������  4..1 ";
__flash unsigned __flash char *Menu8paramPRM[] = {Menu81, Menu82, Menu83};
#define MAX_PRM_PARAM (sizeof(Menu8paramPRM) / sizeof(Menu8paramPRM[0]))
__flash unsigned char Menu8Shift0[]	= "0..10";
__flash unsigned char Menu8Shift2[]	= "0..1000";
__flash unsigned char Menu8Shift3[]	= "���";
__flash unsigned __flash char *Menu8ParamRange[] = {Menu8Shift0, Menu8Shift2, Menu8Shift3};


//��������� �����������
__flash unsigned char Menu91[]= "����� ���������     ";
__flash unsigned char Menu92[]= "������������ �������";
__flash unsigned char Menu93[]= "����. �������  4..1 ";
__flash unsigned char Menu94[]= "����. �������  4..1 ";
__flash unsigned __flash char* Menu9paramPRD[]={Menu91, Menu92, Menu93, Menu94};
#define MAX_PRD_PARAM (sizeof(Menu9paramPRD) / sizeof(Menu9paramPRD[0]))
__flash unsigned char Menu9Shift0[]="0..10";
__flash unsigned char Menu9Shift1[]="20..100";
__flash unsigned char Menu9Shift2[]="";
__flash unsigned __flash char* Menu9ParamRange[]={Menu9Shift0, Menu9Shift1, Menu9Shift2, Menu9Shift2};

//����� ���������
__flash unsigned char Menu101[]="��������� ���� ���";
__flash unsigned char Menu102[]="��������� ���� ���";
__flash unsigned char Menu103[]="����� �����������";
__flash unsigned char Menu104[]="�������� ���.�������";
__flash unsigned char Menu105[]="����� ������� �� �� ";
__flash unsigned char Menu106[]="����.�������� 1 ���.";
#define defNumParamAll 6
__flash unsigned __flash char* Menu10paramAll[]={Menu101, Menu102, Menu103, Menu104, Menu105, Menu106};

__flash unsigned char Menu11d[]="���";
__flash unsigned char Menu11r[]="���";
__flash unsigned char Menu11r1[]="��1";
__flash unsigned char Menu11r2[]="��2";
__flash unsigned char Menu11t[]="���";

__flash unsigned char Menu11var1[]="����� ";
__flash unsigned char Menu11var2[]="����� ";
__flash unsigned char Menu11var3[]="������";
__flash unsigned char Menu11var4[]="����  ";
__flash unsigned char Menu11var5[]="����1 ";
__flash unsigned char Menu11var6[]="����2 ";
__flash unsigned char Menu11var7[]="����3 ";

__flash unsigned __flash char* Menu11var[]={Menu11var1, Menu11var2, Menu11var3,Menu11var4,Menu11var5,Menu11var6,Menu11var7};
__flash unsigned char Menu11Err[]="????";

__flash unsigned char TestDelayMline2[]="   ������ �����     ";
__flash unsigned char TestDelayMline3[]="  ��������� ? ���   ";

__flash unsigned char ParamTypeDef0[]="���-����  ";
__flash unsigned char ParamTypeDef1[]="���-�����1";
__flash unsigned char ParamTypeDef2[]="���-�����2";
__flash unsigned char ParamTypeDef3[]="���-L60   ";
__flash unsigned char ParamTypeDef4[]="��-����   ";
__flash unsigned char ParamTypeDef5[]="��-���-�� ";
__flash unsigned char ParamTypeDef6[]="���-����  ";
__flash unsigned char ParamTypeDef7[]="���-���-��";
__flash unsigned char ParamTypeDef8[]="������    ";
#define defNumTypeDef 8
__flash unsigned __flash char *ParamTypeDef[]={	ParamTypeDef0, ParamTypeDef1, ParamTypeDef2, ParamTypeDef3,
												ParamTypeDef4, ParamTypeDef5, ParamTypeDef6, ParamTypeDef7, ParamTypeDef8};

__flash unsigned char ParamLineDef0[]="2 ��������";
__flash unsigned char ParamLineDef1[]="3 ��������";
__flash unsigned char ParamLineDef2[]="������    ";
__flash unsigned __flash char *ParamLineDef[] = {ParamLineDef0, ParamLineDef1, ParamLineDef2};


#define MAX_NUM_ITEM_TYPE_PRM ( (sizeof(ParamTypePrmDef) / sizeof(ParamTypePrmDef[0])) - 1 )
__flash unsigned char ParamTypePrmDef0[] = "���+���� ";
__flash unsigned char ParamTypePrmDef1[] = "�������� ";
__flash unsigned char ParamTypePrmDef2[] = "���������";
__flash unsigned char ParamTypePrmDef3[] = "������   ";
__flash unsigned __flash char *ParamTypePrmDef[] = 
{ 
	ParamTypePrmDef0,
	ParamTypePrmDef1,
	ParamTypePrmDef2,
	ParamTypePrmDef3
};
							

//���� ����������
__flash unsigned char Menu1Def[]="���:";
__flash unsigned char Menu1Rec[]="���:";
__flash unsigned char Menu1Rec1[]="��1:";
__flash unsigned char Menu1Rec2[]="��2:";
__flash unsigned char Menu1Tran[]="���:";
__flash unsigned char Menu1Err[]="????    ";
__flash unsigned char Menu1disrepair[]="������  ";
__flash unsigned char Menu1warning[]="������� ";

__flash unsigned char Menu1Disc[]= "������� ";
__flash unsigned char Menu1Ready[]="�����   ";
__flash unsigned char Menu1Work[]= "������  ";
__flash unsigned char Menu1Talk[]= "����    ";
__flash unsigned char Menu1Test1[]="����1   ";
__flash unsigned char Menu1Test2[]="����2   ";
__flash unsigned char Menu1Test3[]="����3   ";
__flash unsigned __flash char* Menu1regime[]={Menu1Disc,Menu1Ready,Menu1Work,Menu1Talk,Menu1Test1,Menu1Test2};
__flash unsigned __flash char* Menu1regimeInp[]={Menu1Disc,Menu1Work};
__flash unsigned __flash char* Menu1regimeInp1[]={Menu1Disc,Menu1Test1,Menu1Test2,Menu1Test3};
__flash unsigned __flash char* Menu1regimeInp2[]={Menu1Disc,Menu1Work,Menu1Test1,Menu1Test2,Menu1Test3};

//��������� �����
__flash unsigned char Menu1post00[]="������. ";
__flash unsigned char Menu1post01[]="��������";
__flash unsigned char Menu1post02[]="����    ";
__flash unsigned char Menu1post03[]="������  ";
__flash unsigned char Menu1post04[]="������. ";
__flash unsigned char Menu1post05[]="�.������";
__flash unsigned char Menu1post06[]="��������";
__flash unsigned char Menu1post07[]="���.����";
__flash unsigned char Menu1post08[]="?0x08?  ";
__flash unsigned char Menu1post09[]="��� ��  ";
__flash unsigned char Menu1post0A[]="����    ";
__flash unsigned __flash char* Menu1condPOST[]={Menu1post00,Menu1post01,Menu1post02,Menu1post03,Menu1post04,Menu1post05,Menu1post06,Menu1post07,Menu1post08,Menu1post09,Menu1post0A};
//��������� ���
__flash unsigned char Menu1prm00[]="������. ";
__flash unsigned char Menu1prm01[]="��� ��  ";
__flash unsigned char Menu1prm02[]="��� ��  ";
__flash unsigned char Menu1prm03[]="��� ��  ";
__flash unsigned char Menu1prm04[]="������. ";
__flash unsigned char Menu1prm05[]="�.������";
__flash unsigned char Menu1prm06[]="��������";
__flash unsigned char Menu1prm07[]="��� ��  ";
__flash unsigned char Menu1prm08[]="?0x08?  ";
__flash unsigned char Menu1prm09[]="?0x09?  ";
__flash unsigned char Menu1prm0A[]="����    ";
__flash unsigned __flash char* Menu1condPRM[]={Menu1prm00,Menu1prm01,Menu1prm02,Menu1prm03,Menu1prm04,Menu1prm05,Menu1prm06,Menu1prm07,Menu1prm08,Menu1prm09,Menu1prm0A};
//��������� ���
__flash unsigned char Menu1prd00[]="������. ";
__flash unsigned char Menu1prd01[]="��� ��  ";
__flash unsigned char Menu1prd02[]="��� ��  ";
__flash unsigned char Menu1prd03[]="��� ��  ";
__flash unsigned char Menu1prd04[]="������. ";
__flash unsigned char Menu1prd05[]="�.������";
__flash unsigned char Menu1prd06[]="��������";
__flash unsigned char Menu1prd07[]="?0x07?  ";
__flash unsigned char Menu1prd08[]="?0x08?  ";
__flash unsigned char Menu1prd09[]="������. ";
__flash unsigned char Menu1prd0A[]="����    ";
__flash unsigned __flash char* Menu1condPRD[]={Menu1prd00,Menu1prd01,Menu1prd02,Menu1prd03,Menu1prd04,Menu1prd05,Menu1prd06,Menu1prd07,Menu1prd08,Menu1prd09,Menu1prd0A};

__flash unsigned char GlobalAvar[]="g-";
__flash unsigned char LocalAvar[]="l-";

__flash unsigned char MenuInputData[]="����:";
__flash unsigned char MenuInputOldPassword[]="������:";
__flash unsigned char MenuInputNewPassword[]="����� ������:";


__flash unsigned char MenuValue[]="��������: ";
__flash unsigned char MenuState[]="��������: ";


__flash unsigned char MenuAllSynchrTimer2[]="????";
__flash unsigned char MenuAllSynchrTimer0[]="����.";
__flash unsigned char MenuAllSynchrTimer1[]="���.";
__flash unsigned __flash char* MenuAllSynchrTimerNum[]={MenuAllSynchrTimer0, MenuAllSynchrTimer1, MenuAllSynchrTimer2};

__flash unsigned char ParamRange[]="��������: ";

//�������� �������� ��� ����� ����������
__flash unsigned char Menu16Shift0[]="���/����";
__flash unsigned char Menu16Shift1[]="���/����";
__flash unsigned char Menu16Shift2[]="0..5";
__flash unsigned char Menu16Shift3[]="���/����";
__flash unsigned char Menu16Shift4[]="0..22";
__flash unsigned char Menu16Shift5[]="0..22";
__flash unsigned char Menu16Shift6[]="100..1000";
__flash unsigned char Menu16Shift7[]="0..10";
__flash unsigned __flash char* Menu16ParamRange[]={Menu16Shift0,Menu16Shift1,Menu16Shift2,Menu16Shift3, Menu16Shift4,Menu16Shift5,Menu16Shift7};

//��������
__flash unsigned char Menu19Shift0[]="�������� P-USP  ";
__flash unsigned char Menu19Shift1[]="�������� AT-USP ";
__flash unsigned char Menu19Shift2[]="�������� DSP-USP";
__flash unsigned char Menu19Shift3[]="������. ���������";
__flash unsigned char Menu19Shift4[]="��� ���������";
__flash unsigned __flash char* Menu19Param[]={Menu19Shift0, Menu19Shift1, Menu19Shift2, Menu19Shift3, Menu19Shift4};
  __flash unsigned char fDopParamViewTrue[]="����";
  __flash unsigned char fDopParamViewFalse[]="���";
  __flash unsigned __flash char*fDopParamView[]={fDopParamViewFalse, fDopParamViewTrue};
  __flash unsigned char fDopParamValueTrue[]="��������";
  __flash unsigned char fDopParamValueFalse[]="HEX";
  __flash unsigned __flash char*fDopParamValue[]={fDopParamValueFalse, fDopParamValueTrue};

//����������� ������
__flash unsigned char UnknownErrorT[]=       "     Error      ";
//����� ��������������
__flash unsigned char Menu1GlobalWarning1[] ="���������� ���� ";

//������ ����������
__flash unsigned char Menu1GlobalError1[]   ="������.��. FLASH";
__flash unsigned char Menu1GlobalError2[]   ="������.���.FLASH";
__flash unsigned char Menu1GlobalError4[]   ="������.��. PLIS ";
__flash unsigned char Menu1GlobalError8[]   ="������.���.PLIS ";
__flash unsigned char Menu1GlobalError10[]  ="������.���.2RAM ";
__flash unsigned char Menu1GlobalError100[] ="������.DSP 2RAM ";
__flash unsigned char Menu1GlobalError200[] ="������.��. 2RAM ";
__flash unsigned char Menu1GlobalError400[] ="���������� ���� ";
__flash unsigned char Menu1GlobalError800[] ="������ ����.���.";
__flash unsigned char Menu1GlobalError1000[]="������� ����.���";
__flash unsigned __flash char *Menu1GlobalErrorT[] = 
{
	Menu1GlobalError1, 
	Menu1GlobalError2, 
	Menu1GlobalError4, 
	Menu1GlobalError8, 
	Menu1GlobalError10,
	UnknownErrorT,
	UnknownErrorT,
	UnknownErrorT,
    Menu1GlobalError100, 
	Menu1GlobalError200,
	Menu1GlobalError400,
	Menu1GlobalError800,
	Menu1GlobalError1000,
	UnknownErrorT,
	UnknownErrorT,
	UnknownErrorT
};

//������ ������
__flash unsigned char Menu1PostError1[]   ="��� ����� ���   ";
__flash unsigned char Menu1PostError2[]   ="������.����.��� ";
__flash unsigned char Menu1PostError4[]   ="������.������.  ";
__flash unsigned char Menu1PostError8[]   ="������.���. ��� ";
__flash unsigned char Menu1PostError100[] ="������.����.����";
__flash unsigned char Menu1PostError200[] ="������.����.����";
__flash unsigned char Menu1PostError800[] ="������.����.��� ";
__flash unsigned char Menu1PostError2000[]="������.���. ��� ";
__flash unsigned char Menu1PostError4000[]="��� ������� ��  ";
__flash unsigned __flash char *Menu1PostErrorT[] = 
{
	Menu1PostError1,
	Menu1PostError2,
	Menu1PostError4,
	Menu1PostError8,
	UnknownErrorT,
	UnknownErrorT,
	UnknownErrorT,
	UnknownErrorT,
    Menu1PostError100,
	Menu1PostError200,
	UnknownErrorT,
	Menu1PostError800,
	UnknownErrorT,
	Menu1PostError2000,
	Menu1PostError4000,
	UnknownErrorT
};

//�������������� ������
__flash unsigned char Menu1PostWarning1[] ="������ ����. �� ";

//������ ���������
__flash unsigned char Menu1PrmError1[]    ="��� ����� ���   ";
__flash unsigned char Menu1PrmError2[]    ="������.����.��� ";
__flash unsigned char Menu1PrmError4[]    ="������.��� ��1  ";
__flash unsigned char Menu1PrmError8[]    ="����.��-���.���.";
__flash unsigned char Menu1PrmError100[]  ="������.���. ��� ";
__flash unsigned char Menu1PrmError200[]  ="������.����.��� ";
__flash unsigned char Menu1PrmError400[]  ="������.���. ��� ";
__flash unsigned char Menu1PrmError800[]  ="������.�����.���";
__flash unsigned char Menu1PrmError1000[] ="�������.��� ��1 ";
__flash unsigned char Menu1PrmError8000[] ="����� ����. ���.";
__flash unsigned __flash char *Menu1PrmErrorT[] = 
{
	Menu1PrmError1,
	Menu1PrmError2,
	Menu1PrmError4,
	Menu1PrmError8,
	UnknownErrorT,
	UnknownErrorT,
	UnknownErrorT,
	UnknownErrorT,
    Menu1PrmError100,
	Menu1PrmError200,
	Menu1PrmError400,
	Menu1PrmError800,
	Menu1PrmError1000,
	UnknownErrorT,
	UnknownErrorT,
	Menu1PrmError8000
};

//�������������� ���������
__flash unsigned char Menu1PrmWarning1[] = "����. ������ ���";

//������ �����������
__flash unsigned char Menu1PrdError1[]    ="��� ����� ���   ";
__flash unsigned char Menu1PrdError2[]    ="������.����.��� ";
__flash unsigned char Menu1PrdError100[]  ="������.��.������";
__flash unsigned char Menu1PrdError200[]  ="������.����.����";
__flash unsigned char Menu1PrdError400[]  ="������.���. ����";
__flash unsigned char Menu1PrdError800[]  ="������.����.��� ";
__flash unsigned __flash char *Menu1PrdErrorT[] = 
{
	Menu1PrdError1,
	Menu1PrdError2,
	UnknownErrorT,
	UnknownErrorT,
	UnknownErrorT,
	UnknownErrorT,
	UnknownErrorT,
	UnknownErrorT,
    Menu1PrdError100,
	Menu1PrdError200,
	Menu1PrdError400,
	Menu1PrdError800,
	UnknownErrorT,
	UnknownErrorT,
	UnknownErrorT,
	UnknownErrorT
};

//�������� ����� 1
__flash unsigned char Menu20line2test1[]= "��1:      ��2:      ";
__flash unsigned char Menu20line2test11[]="��1:";
__flash unsigned char Menu20line2test12[]="��2:";
__flash unsigned char Menu201line3test1[]="������ 1";
__flash unsigned char Menu202line3test1[]="������ 2";
__flash unsigned __flash char* Menu20line3[]={Menu201line3test1, Menu202line3test1};
__flash unsigned char Menu20gr11[]="����";
__flash unsigned char Menu20gr12[]="��1";
__flash unsigned char Menu20gr13[]="��2";
__flash unsigned char Menu20gr14[]="���1"; __flash unsigned char Menu20gr15[]="���2"; __flash unsigned char Menu20gr16[]="���3"; __flash unsigned char Menu20gr17[]="���4";
__flash unsigned char Menu20gr18[]="���5"; __flash unsigned char Menu20gr19[]="���6"; __flash unsigned char Menu20gr110[]="���7"; __flash unsigned char Menu20gr111[]="���8";
__flash unsigned __flash char* Menu20gr1[]={Menu20gr11, Menu20gr12, Menu20gr13, Menu20gr14, Menu20gr15, Menu20gr16, Menu20gr17,
                                             Menu20gr18, Menu20gr19, Menu20gr110, Menu20gr111};
__flash unsigned char Menu20gr21[]="����";
__flash unsigned char Menu20gr22[]="��1";
__flash unsigned char Menu20gr23[]="��2";
__flash unsigned __flash char* Menu20gr2[]={Menu20gr21, Menu20gr22, Menu20gr23};

__flash unsigned char Menu201line4[]="���� 1";
__flash unsigned char Menu202line4[]="���� 2";
__flash unsigned __flash char* Menu20line4[]={Menu201line4, Menu202line4};

__flash unsigned char Menu20no[]="���� �� �������";

//������
__flash unsigned char Archive[]="������ ";
__flash unsigned char ArchiveEvent[]="�������";
__flash unsigned char ArchiveDef[]= "������";
__flash unsigned char ArchivePrm[]= "���������";
__flash unsigned char ArchivePrd[]= "�����������";
__flash unsigned __flash char* Menu4point[]={ArchiveEvent,ArchiveDef,ArchivePrm,ArchivePrd};
__flash unsigned char Menu21event[]="�������.";
__flash unsigned char Menu2xRdArch[]="������ ������.";
__flash unsigned char ArchPrm[]="��";
__flash unsigned char ArchPrd[]="��";
__flash unsigned char ArchEvt[]="��";
__flash unsigned char ArchDef[]="��";
__flash unsigned __flash char* Menu21e[]={ArchEvt, ArchPrd, ArchPrm, ArchDef};

__flash unsigned char Menu21g[]="���";

__flash unsigned char ArchEv00[]="";
__flash unsigned char ArchEv01[]="�.���.���� ";
__flash unsigned char ArchEv02[]="�.����.����";
__flash unsigned char ArchEv03[]="�.����� ���";
__flash unsigned char ArchEv04[]="�.���. ��� ";
__flash unsigned char ArchEv05[]="�.����. ���";
__flash unsigned char ArchEv06[]="�.���. ��� ";
__flash unsigned char ArchEv07[]="�.�����.���";
__flash unsigned char ArchEv08[]="��� ��. ���";
__flash unsigned char ArchEv09[]="��.����.���";
__flash unsigned char ArchEv10[]="�.�����.���";
__flash unsigned char ArchEv11[]="�.���.���.�";
__flash unsigned char ArchEv12[]="�.���.���.�";
__flash unsigned char ArchEv13[]="��� �. ����";
__flash unsigned char ArchEv14[]="��� �. ����";
__flash unsigned char ArchEv15[]="��� ����.��";
__flash unsigned char ArchEv16[]="���������� ";
__flash unsigned char ArchEv17[]="���.������ ";
__flash unsigned char ArchEv18[]="�����. 1�� ";
__flash unsigned char ArchEv19[]="�����. 2�� ";
__flash unsigned char ArchEv20[]="��� 1�� 5� ";
__flash unsigned char ArchEv21[]="��� 2�� 5� ";
__flash unsigned char ArchEv22[]="�.�����.DSP";
__flash unsigned char ArchEv23[]="���.���.DSP";
__flash unsigned char ArchEv24[]="�.��. 2RAM ";
__flash unsigned char ArchEv25[]="�.���. 2RAM";
__flash unsigned char ArchEv26[]="�.�������  ";
__flash unsigned char ArchEv27[]="�.���. PLIS";
__flash unsigned char ArchEv28[]="�.��. PLIS ";
__flash unsigned char ArchEv29[]="�.��. FLASH";
__flash unsigned char ArchEv30[]="�.���.FLASH";
__flash unsigned char ArchEv31[]="���.���.���";
__flash unsigned char ArchEv32[]="���.���.���";
__flash unsigned __flash char* Menu21ev[]={ArchEv00,
                                           ArchEv01,ArchEv02,ArchEv03,ArchEv04,ArchEv05,ArchEv06,ArchEv07,ArchEv08,
                                           ArchEv09,ArchEv10,ArchEv11,ArchEv12,ArchEv13,ArchEv14,ArchEv15,ArchEv16,
                                           ArchEv17,ArchEv18,ArchEv19,ArchEv20,ArchEv21,ArchEv22,ArchEv23,ArchEv24,
                                           ArchEv25,ArchEv26,ArchEv27,ArchEv28,ArchEv29,ArchEv30,ArchEv31,ArchEv32};


__flash unsigned char ArchEvV[]="�����.";
__flash unsigned char ArchEvV1[]="������� ";
__flash unsigned char ArchEvStart[]="������ ";
__flash unsigned char ArchEvEnd[]=  "����� ";
__flash unsigned char ArchEvCom[]="�������";

// �������� ������ ���� / ����������
__flash unsigned char fuControlItem0[] = ".����� ������.     ";
__flash unsigned char fuControlItem1[] = ".���� �����. ���.  ";
__flash unsigned char fuControlItem2[] = ".���� �����. ����. ";

// �������� ��������� ������ ���� / ����������
// ������ �������������� eControl
__flash unsigned char fuControlAction0[] = "�����";
__flash unsigned char fuControlAction1[] = "����";
__flash unsigned char fuControlAction2[] = "����";
__flash unsigned char __flash *fuControlAction[] = 
{
	fuControlAction0,
	fuControlAction1,
	fuControlAction2
};