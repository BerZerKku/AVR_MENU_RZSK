#include <ioavr.h>
#include "MyDef.h"
#include "InterfaceS.h"
#include "driverLCD.h"
#include "DataCenter1.h"

int CRCSum;
extern uchar *MeasDop[];

extern unsigned char Temp_uart1[];

extern unsigned char DataLCD[];
extern unsigned char TimeLCD[];
extern unsigned char LCD2new;
extern unsigned char LCDtimerNew;
extern unsigned char GlobalCurrentState[];
extern unsigned char Mass1[];
extern unsigned char RecivVar;
extern unsigned char CurrentState[];
extern unsigned char MenuLevel;

//параметры ПОСТа
extern unsigned char MenuPossibleTimeSignal1[];
extern unsigned char MenuPossibleTimeSignal2[];
extern unsigned char MenuVoltageLimit[];
extern unsigned char MenuRZalarm[];
extern unsigned char MenuTypeDefend;
extern unsigned char MenuTypeLine;
extern unsigned char MenuTimeNoMan[];
extern unsigned char MenuOverlapPulse[];
extern unsigned char MenuTypePrm;

//Параметры Общие
extern unsigned char MenuAllKeepComPRM;
extern unsigned char MenuAllKeepComPRD;
extern unsigned char MenuAllTimeRerun[];
extern unsigned char MenuAllNumDevice[];
extern unsigned char MenuAllControlUout;
extern unsigned char MenuAllCF[];
extern unsigned int MyInsertion[];
extern unsigned char ValuePrdLongCom;
extern unsigned char ValuePrdCtrlByte;
extern unsigned char MenuVoltageLimit1[];
extern unsigned char MenuVoltageLimit2[];

//наши параметры
extern unsigned char MenuReturnDelay[];
extern unsigned char MenuZadTR[];
extern unsigned char MenuZadRe[];

//параметры приемника
extern unsigned char MenuPrmTimeOn[];
extern unsigned char ValuePrmTimeOff[];
extern unsigned char MenuPrmTimeOff[];
extern unsigned char ValuePrmBlockCom;
extern unsigned char MenuPrmBlockCom[];

//параметры передатчика
extern unsigned char MenuPrdTimeOn[];
extern unsigned char MenuPrdTimeCom[];
extern unsigned char MenuPrdBlockCom[];
extern unsigned char MenuPrdLongCom[];
extern unsigned char MenuPrdCtrlByte[];
extern unsigned char ValuePrdBlockCom;

//extern unsigned char Iline1[], Iline2[], Uline[], Usigndef[], Uinkch[], Kd[], Kov[], Pk[];
extern unsigned char Iline1[], Iline2[], Uline[], Usigndef1[], Usigndef2[], Uinkch1[], Uinkch2[], Kd[];
extern unsigned char LCDparam;
extern unsigned char FreqNum[];

extern unsigned char Mass1[];

extern unsigned char Dop_byte[];

//Архив
extern unsigned int NumberRecording;
extern unsigned int NumRecStart;
extern unsigned char NumberRec;
extern unsigned char ReadArch;
extern unsigned int AddressStartRegister;
extern unsigned char NumberRegister;
extern unsigned char StRegister;
extern strArchive sArchive;

//Тесты
extern unsigned char gr1, gr2, gr3, gr4;

//переменные проверки достоверности даты и времени
unsigned char TrDataTimeMass[]={0x30,0x30,0x30,0x30,0x30,0x30};;
unsigned char DataError=0;
unsigned char TimeError=0;
unsigned int Pk_temp=0;

unsigned char j;
unsigned char l;
unsigned char i_dc;

unsigned char TempMass[10];

//для единой версии
extern unsigned char cNumComR, cNumComT;
extern unsigned char cNumComR1, cNumComR2;
extern unsigned char cTypeLine;
extern unsigned char cNumLine;
extern bool bDef;
extern bool bReadVers; //true - версия была считана, false - ответ не пришел, продолжаем опрос
  //для вывода информации в 0 меню
  extern bool bDefAvar, bDefWarn;
  extern bool bGlobalAvar, bGlobalWarn;
  extern bool bRec1Avar, bRec1Warn;
  extern bool bTrAvar, bTrWarn;
  extern bool bRec2Avar, bRec2Warn;
extern __flash  unsigned char  Menu61[],Menu62[],Menu63[],Menu64[],Menu62_1[],Menu62_2[];
extern unsigned char LineInMenu6; //кол-во строк в меню просмотр параметров/ установить параметры
extern unsigned __flash char  *mMenu6point[]; //массив строк для меню просмотр параметров/ установить параметры



static void fCorrParam(void);

//функция вычисляет код CRC-16
//на входе указатель на начало буфера
//и количество байт сообщения (без принятого кода CRC-16)
//но при этом синхробайты не считаются
unsigned char GetCRCSum(unsigned char *buf, char bufsize)
{
	unsigned char CRCbuf;
	unsigned char k;
	
	CRCbuf = 0;
	for (k = 2; k < bufsize; k++)
		CRCbuf+=buf[k];
	
	return(CRCbuf);
}

//подпрограмма подготовки отпраки одного байта информации по UART
void TransDataByte(unsigned char ByteCommand, unsigned char ByteData)
{
      Sost|=CurSost;
    //байты синхронизации уже лежат в массиве
      Tr_buf_data_uart[2]=ByteCommand;//состояния Пост
      Tr_buf_data_uart[3]=0x01;
      Tr_buf_data_uart[4]=ByteData;
      CRCSum=GetCRCSum(Tr_buf_data_uart,5);
      Tr_buf_data_uart[5]=CRCSum;
      StartTrans(6);
}

void TransDataInf(unsigned char ByteCommand, unsigned char NumberTransByte)
{
      Sost|=CurSost;
    //байты синхронизации уже лежат в массиве
      Tr_buf_data_uart[2]=ByteCommand;
      Tr_buf_data_uart[3]=NumberTransByte;
      CRCSum=GetCRCSum(Tr_buf_data_uart,4+NumberTransByte);
      Tr_buf_data_uart[4+NumberTransByte]=CRCSum;
      StartTrans(5+NumberTransByte);
}

//возвращаем сообщение с ошибкой
void ErrorMessage(char code)
{
      Sost|=CurSost;
    //байты синхронизации уже лежат в массиве
      Tr_buf_data_uart[2]=code;
      Tr_buf_data_uart[3]=0x02; //т.е. посылается два байта данных
      CRCSum=GetCRCSum(Tr_buf_data_uart,6);
      Tr_buf_data_uart[6]=CRCSum;
      StartTrans(7);
}

void FParamDef(unsigned char command)
{
  
  switch(command)
	{
		case 0x01:	// тип защиты
		{
			MenuTypeDefend = (Rec_buf_data_uart[4] < 8) ? Rec_buf_data_uart[4] : 8;
		}break;
		case 0x02:	// тип линии
		{
			if ( (Rec_buf_data_uart[4] >= 1) && (Rec_buf_data_uart[4] <= 2) )
				MenuTypeLine = Rec_buf_data_uart[4] - 1;
			else
				MenuTypeLine = 2;
		}break;
		case 0x03:	// доп. время без манипуляции
		{
			if (Rec_buf_data_uart[4] <= 99)
			{
				MenuTimeNoMan[0] = (Rec_buf_data_uart[4] / 10) + '0';
				MenuTimeNoMan[1] = (Rec_buf_data_uart[4] % 10) + '0';
			}
			else
			{
				MenuTimeNoMan[0] = '?';
				MenuTimeNoMan[1] = '?';
			}
		}break;
		case 0x04:	// компенсация задержки в линии
		{
			if ( (Rec_buf_data_uart[4]) <= 18 )
			{
				MenuPossibleTimeSignal1[0] = (Rec_buf_data_uart[4] / 10) + 0x30;
				MenuPossibleTimeSignal1[1] = (Rec_buf_data_uart[4] % 10) + 0x30;
			}
			else  
			{
				MenuPossibleTimeSignal1[0] = '?';
				MenuPossibleTimeSignal1[1] = '?';
			}
			
			 if ((Rec_buf_data_uart[5]) <= 18)
			 {
       			MenuPossibleTimeSignal2[0] = (Rec_buf_data_uart[5] / 10) + 0x30;
				MenuPossibleTimeSignal2[1] = (Rec_buf_data_uart[5] % 10) + 0x30;
      		}else  
			{
				MenuPossibleTimeSignal2[0] = '?';
				MenuPossibleTimeSignal2[1] = '?';
			}
		}break;
		case 0x05:	// принято значение перекрытия импульсов
		{
			if (Rec_buf_data_uart[4] <= 54 )
			{
				MenuOverlapPulse[0] = (Rec_buf_data_uart[4] / 10) + 0x30;
				MenuOverlapPulse[1] = (Rec_buf_data_uart[4] % 10) + 0x30;
			}
			else
			{
				MenuOverlapPulse[0] = '?';
				MenuOverlapPulse[1] = '?';
			}
		}break;
		case 0x06:	// принято значение напряжения порога
		{
			if (Rec_buf_data_uart[4] <= 20)
			{
				MenuVoltageLimit[0] = (Rec_buf_data_uart[4] / 10) + 0x30;
				MenuVoltageLimit[1] = (Rec_buf_data_uart[4] % 10) + 0x30;
			}
			else  
			{
				MenuVoltageLimit[0] = '?';
				MenuVoltageLimit[1] = '?';
			}
		}break;
		case 0x07:	// тип приемника
		{
			if (Rec_buf_data_uart[4] < 3)
				MenuTypePrm = Rec_buf_data_uart[4];
			else
				MenuTypePrm = 3;
		}break;
		case 0x09:	// порог предупреждения по РЗ
		{
			if (Rec_buf_data_uart[4] <= 20)
			{
				MenuRZalarm[0] = (Rec_buf_data_uart[4] / 10) + '0';
				MenuRZalarm[1] = (Rec_buf_data_uart[4] % 10) + '0';
			}
			else
			{
				MenuRZalarm[0] = '?';
				MenuRZalarm[1] = '?';
			}
		}break;
	}
  
  RecivVar=1;
  LCD2new=1;
};

void FParamPrm(unsigned char com)
{
	switch(com)
	{
		case 0x11:	//задержка на включение команд
		{
			if (Rec_buf_data_uart[4] <= 10)
			{
				MenuPrmTimeOn[0] = (Rec_buf_data_uart[4] / 10) + '0';
				MenuPrmTimeOn[1] = (Rec_buf_data_uart[4] % 10) + '0';
			}
			else
			{
				MenuPrmTimeOn[0] = '?';
				MenuPrmTimeOn[1] = '?';
			}
		}
		break;
		case 0x13:	//задержка на выключение, в 10 раз меньше искомого
		{
			for( char i = 0; i < 8; i++)
			{
				if (Rec_buf_data_uart[4 + i] > 100)
					ValuePrmTimeOff[i] = 0xFF;
				else
					ValuePrmTimeOff[i] = Rec_buf_data_uart[4 + i];
			}
		}
		break;
		case 0x14:	//блокированные команды
		{
			ValuePrmBlockCom = Rec_buf_data_uart[4];
			for (char i = 0; i < 4; i++)
			{
				if (ValuePrmBlockCom & (1 << i))
					MenuPrmBlockCom[3 - i] = '1';
				else
					MenuPrmBlockCom[3 - i] = '0';
			}
		}
		break;
	}
	
	LCD2new=1;
  	RecivVar=1;
}

void FParamPrd(unsigned char com)
{
	switch(com)
	{	
	  case 0x21:	//задержка на включение команд
		{
			if (Rec_buf_data_uart[4] <= 10)
			{
				MenuPrdTimeOn[0] = (Rec_buf_data_uart[4] / 10) + '0';
				MenuPrdTimeOn[1] = (Rec_buf_data_uart[4] % 10) + '0';
			}
			else
			{
				MenuPrdTimeOn[0] = '?';
				MenuPrdTimeOn[1] = '?';
			}
		}
		break;	
	  case 0x22:	//длительность команд
		{
			if ( (Rec_buf_data_uart[4] >= 20) && (Rec_buf_data_uart[4] <= 100) )
			{
				if (Rec_buf_data_uart[4] == 100)
				{
					MenuPrdTimeCom[0] = '1';
					MenuPrdTimeCom[1] = '0';
					MenuPrdTimeCom[2] = '0';
				}
				else
				{
					MenuPrdTimeCom[0] = '0';
					MenuPrdTimeCom[1] = (Rec_buf_data_uart[4] / 10) + '0';
					MenuPrdTimeCom[2] = (Rec_buf_data_uart[4] % 10) + '0';
				}
			}
			else
			{
				MenuPrdTimeCom[0] = '?';
				MenuPrdTimeCom[1] = '?';
				MenuPrdTimeCom[2] = '?';
			}
		}
		break;
	  case 0x24:	//блокированные команды
		{
			ValuePrdBlockCom = Rec_buf_data_uart[4];
			for (char i = 0; i < 4; i++)
			{
				if (ValuePrdBlockCom & (1 << i))
					MenuPrdBlockCom[3 - i] = '1';
				else
					MenuPrdBlockCom[3 - i] = '0';
			}
		}
		break;
	  case 0x25:	//длинные команды
		{
			ValuePrdLongCom=Rec_buf_data_uart[4];
			for (char i = 0; i < 4; i++)
			{
				if (ValuePrdLongCom & (1 << i))
					MenuPrdLongCom[3 - i] = '1';
				else
					MenuPrdLongCom[3 - i] = '0';
			}
		}
		break;
	  case 0x26:	//байт управления
		{
			ValuePrdCtrlByte=Rec_buf_data_uart[4];
			for (char i = 0; i < 8; i++)
			{
				if (ValuePrdCtrlByte & (1 << i))
					MenuPrdCtrlByte[7 - i] = '1';
				else
					MenuPrdCtrlByte[7 - i] = '0';
			}
		}
                break;
	}	
	LCD2new=1;
 	RecivVar=1;
}

void FCurrentState(void)
{
	for (unsigned char l = 0; l < 4; l++) 
	{ //сделаем проверку принятого состояния и перепишем значение в массив
		if (Rec_buf_data_uart[l * 3 + 4] > 0x05) 
			CurrentState[l * 2] = 0x4E;
		else 
			CurrentState[l * 2] = Rec_buf_data_uart[l * 3 + 4];
		
		if (Rec_buf_data_uart[l * 3 + 1 + 4] > 0x0A) 
			CurrentState[l * 2 + 1] = 0x4E;
		else 
			CurrentState[l * 2 + 1] = Rec_buf_data_uart[l * 3 + 4 + 1];
		
		Dop_byte[l] = Rec_buf_data_uart[l * 3 + 2 + 4];
	}
	
	LCD2new = 1;
	RecivVar = 1;
}

void FGlobalCurrentState(void){
  for (unsigned char l = 0; l < 20; l++) 
	  GlobalCurrentState[l] = Rec_buf_data_uart[l + 4];

  if ((GlobalCurrentState[12]!=0)||(GlobalCurrentState[13]!=0)) bGlobalAvar=true; else bGlobalAvar=false;
  if ((GlobalCurrentState[14]!=0)||(GlobalCurrentState[15]!=0)) bGlobalWarn=true; else bGlobalWarn=false;

  if ((GlobalCurrentState[0]!=0)||(GlobalCurrentState[1]!=0)) bDefAvar=true; else bDefAvar=false;
  if ((GlobalCurrentState[2]!=0)||(GlobalCurrentState[3]!=0)) bDefWarn=true; else bDefWarn=false;

  if ((GlobalCurrentState[4]!=0)||(GlobalCurrentState[5]!=0)) bRec1Avar=true; else bRec1Avar=false;
  if ((GlobalCurrentState[6]!=0)||(GlobalCurrentState[7]!=0)) bRec1Warn=true; else bRec1Warn=false;

  if ((GlobalCurrentState[8]!=0)||(GlobalCurrentState[9]!=0)) bTrAvar=true; else bTrAvar=false;
  if ((GlobalCurrentState[10]!=0)||(GlobalCurrentState[11]!=0)) bTrWarn=true; else bTrWarn=false;

  if ((GlobalCurrentState[16]!=0)||(GlobalCurrentState[17]!=0)) bRec2Avar=true; else bRec2Avar=false;
  if ((GlobalCurrentState[18]!=0)||(GlobalCurrentState[19]!=0)) bRec2Warn=true; else bRec2Warn=false;

  LCD2new = 1;
  RecivVar = 1;
}

void FDataTime(void){
	DataLCD[7]=(Rec_buf_data_uart[4]&0x0F)+0x30; //1-цы лет
	DataLCD[6]=(Rec_buf_data_uart[4]>>4)+0x30;  //10-ки лет
	DataLCD[4]=(Rec_buf_data_uart[5]&0x0F)+0x30;  //1-цы мес
	DataLCD[3]=(Rec_buf_data_uart[5]>>4)+0x30;  //10-и мес
	DataLCD[1]=(Rec_buf_data_uart[6]&0x0F)+0x30;  //1-цы дней
	DataLCD[0]=(Rec_buf_data_uart[6]>>4)+0x30;  //10-и дней
	
	TimeLCD[1]=(Rec_buf_data_uart[7]&0x0F)+0x30; //1-цы час
	TimeLCD[0]=(Rec_buf_data_uart[7]>>4)+0x30;  //10-ки час
	TimeLCD[4]=(Rec_buf_data_uart[8]&0x0F)+0x30;  //1-цы мин
	TimeLCD[3]=(Rec_buf_data_uart[8]>>4)+0x30;  //10-и мин
	TimeLCD[7]=(Rec_buf_data_uart[9]&0x0F)+0x30;  //1-цы сек
	TimeLCD[6]=(Rec_buf_data_uart[9]>>4)+0x30;  //10-и сек
	
	//Проверка на достовереность
	for (unsigned char j = 0; j < 8; j++)
	{
		if  (((DataLCD[j]>0x39)||(DataLCD[j]<0x30))&&(DataLCD[j]!='.')) DataError++; else DataError=0;
		if  (((TimeLCD[j]>0x39)||(TimeLCD[j]<0x30))&&(TimeLCD[j]!=':')) TimeError++; else TimeError=0;
	}
	
	if (((DataLCD[7]-0x30)+((DataLCD[6]-0x30)*10))>99)  DataError++;
	if (((DataLCD[4]-0x30)+((DataLCD[3]-0x30)*10))>12)  DataError++;
	if (((DataLCD[1]-0x30)+((DataLCD[0]-0x30)*10))>31)  DataError++;
	if (((TimeLCD[1]-0x30)+((TimeLCD[0]-0x30)*10))>24)  TimeError++;
	if (((TimeLCD[4]-0x30)+((TimeLCD[3]-0x30)*10))>60)  TimeError++;
	if (((TimeLCD[7]-0x30)+((TimeLCD[6]-0x30)*10))>60)  TimeError++;
	
	if (DataError!=0)
	{
		DataLCD[7]='0'; //1-цы лет
		DataLCD[6]='0';  //10-ки лет
		DataLCD[4]='0';  //1-цы мес
		DataLCD[3]='0';  //10-и мес
		DataLCD[1]='0';  //1-цы дней
		DataLCD[0]='0';  //10-и дней
		DataError=0;
		for (unsigned char j=0; j<3; j++) 
			TrDataTimeMass[j]=0x00;
	}
	else 
		for (unsigned char j = 0; j < 3; j++) 
			TrDataTimeMass[j]=Rec_buf_data_uart[4+j];
	
	if (TimeError!=0){
		TimeLCD[1]='0'; //1-цы час
		TimeLCD[0]='0';  //10-ки час
		TimeLCD[4]='0';  //1-цы мин
		TimeLCD[3]='0';  //10-и мин
		TimeLCD[7]='0';  //1-цы сек
		TimeLCD[6]='0';  //10-и сек
		TimeError=0;
		for (unsigned char j = 0; j < 3; j++) 
			TrDataTimeMass[j+3]=0x00;
	}
	else 
		for (unsigned char j = 0; j < 3; j++) 
			TrDataTimeMass[j + 3] = Rec_buf_data_uart[4 + j + 3];
	
	RecivVar = 1;
	LCDtimerNew = 1; //говорим, что было получено новое время и дата
};

void fDopCodeToChar(unsigned char *Mass, unsigned char StartAddr, unsigned char Factor, unsigned char Param, unsigned char Value){  //массив, начальный адрес, множитель, параметр
	unsigned char i;
	//множитель 1 или 2, других значений быть не должно
	Factor--;
	
	// очистим все позиции, начиная с заданной
	for(i = StartAddr; i <= 8; i++) 
		Mass[i]=' ';
	
	//проверка на отрицательное число
	if (Value > 127)
	{ 
		Value = (Value ^ 0xFF) + 1;
		Mass[StartAddr++] = '-'; 
	}
	
	Value = Value << Factor;
	
	if (Value < 99)
	{
		Mass[StartAddr++] = (Value / 10) + '0'; 
		Mass[StartAddr++] = (Value % 10) + '0';
	}else{
		Mass[StartAddr++]='?'; 
		Mass[StartAddr++]='?';
	}
	
	// добавим пробел, если есть место
	if (StartAddr < 7) 
		StartAddr++;
	
	// вывод размерности
	switch(Param)
	{
	case 2:
		Mass[StartAddr++] = 'д';
		Mass[StartAddr] = 'Б';
		break;
	}
}

void fIntCodeToChar(unsigned char *Mass, unsigned char StartAddr, unsigned char Factor, unsigned char Param, unsigned int Value, unsigned int Max){
  unsigned char i;
// Value - должно быть меньше 1000
  Factor--;

  for(i=StartAddr; i<9; i++) Mass[i]=' ';

  Value=Value<<Factor;
  if (Value<Max){
    if (Value>999){
      Mass[StartAddr]=Value/1000;
      Value=Value-Mass[StartAddr]*1000;
      Mass[StartAddr]+=0x30; StartAddr++;
    }
    if (Value>99){
      Mass[StartAddr]=Value/100;
      Value=Value-Mass[StartAddr]*100;
      Mass[StartAddr]+=0x30; StartAddr++;
    }
    Mass[StartAddr]=Value/10+0x30; StartAddr++;
    Mass[StartAddr]=Value%10+0x30; StartAddr++;
  }else{
    Mass[StartAddr]='?'; StartAddr++;
    Mass[StartAddr]='?'; StartAddr++;
  }
  if (StartAddr<7) StartAddr++;
  switch(Param){
    case 1:{
      Mass[StartAddr]='м';
      Mass[StartAddr+1]='А';
    }break;
    case 3:{
      Mass[StartAddr]='О';
      Mass[StartAddr+1]='м';
    }
  }

}

void HexToViewHex(unsigned char *Mass, unsigned char StartAddr, unsigned char Hex)
{
	unsigned char t1, t2;
	
	t1 = Hex >> 4;
	t2 = Hex & 0x0F;
	if (t1 > 9) 
		Mass[StartAddr] = t1 + 0x37; 
	else  
		Mass[StartAddr] = t1 + 0x30;
	
	if (t2 > 9) 
		Mass[StartAddr + 1] = t2 + 0x37; 
	else  
		Mass[StartAddr + 1] = t2 + 0x30;
}

extern signed int UlineValue, IlineValue;
void FMeasureParam(void){
	unsigned int itmp;
	if (Rec_buf_data_uart[4]==0x00)
	{  //пришли все параметры
		
		//сопротивление
		itmp = (Rec_buf_data_uart[5]<<8) + Rec_buf_data_uart[6];
		if (itmp > 999) itmp = 999;
		fIntCodeToChar(Iline1,2,1,3,itmp,1000);
		HexToViewHex(MeasDop[0], 3, Rec_buf_data_uart[5]);
		HexToViewHex(MeasDop[0], 5, Rec_buf_data_uart[6]);
		
		//ток первый
		itmp=(Rec_buf_data_uart[7]<<8)+Rec_buf_data_uart[8];
		IlineValue = itmp; 		//значение для вычисления коррекции
		fIntCodeToChar(Iline2,3,1,1,itmp,1000);
		HexToViewHex(MeasDop[1], 3, Rec_buf_data_uart[7]);
		HexToViewHex(MeasDop[1], 5, Rec_buf_data_uart[8]);
		
		//напряжение на линии
		if ((Rec_buf_data_uart[9] < 100)&&(Rec_buf_data_uart[10]<99)){
			Uline[2]=(Rec_buf_data_uart[9]/10)+0x30;
			Uline[3]=(Rec_buf_data_uart[9]%10)+0x30;
			Uline[5]=(Rec_buf_data_uart[10]/10)+0x30;
			//значение для вычисления коррекции
			UlineValue=((int16_t) Rec_buf_data_uart[9])*10 + (Rec_buf_data_uart[10]/10);  
		}else{
			Uline[2] = '?';
			Uline[3] = '?';
			Uline[5] = '?';
			//значение для вычисления коррекции
			UlineValue = 0;
		}
		HexToViewHex(MeasDop[2], 3, Rec_buf_data_uart[9]);
		HexToViewHex(MeasDop[2], 5, Rec_buf_data_uart[10]);
		
		//напряжение защиты
		fDopCodeToChar(Usigndef1,4,1,2,Rec_buf_data_uart[11]);
		fDopCodeToChar(Usigndef2,4,1,2,Rec_buf_data_uart[12]);
		HexToViewHex(MeasDop[3], 3, Rec_buf_data_uart[11]);
		
		//контрльная частота
		fDopCodeToChar(Uinkch1,4,1,2,Rec_buf_data_uart[13]);
		fDopCodeToChar(Uinkch2,4,1,2,Rec_buf_data_uart[14]);
		HexToViewHex(MeasDop[4], 3, Rec_buf_data_uart[13]);
		
		//использование динамического диапазона приемника
		fDopCodeToChar(Kd, 3, 1, 2, Rec_buf_data_uart[15]);
		HexToViewHex(MeasDop[5], 3, Rec_buf_data_uart[15]);
	}
	LCDparam=1;
	RecivVar=1;
}

void FParamGlobal(unsigned char command){
  switch(command){
    case 0x36:{ //удержание реле команд приемника (общие)
      if (Rec_buf_data_uart[4]<2) MenuAllKeepComPRM=Rec_buf_data_uart[4];
      else  MenuAllKeepComPRM=0x02;
    }break;
    case 0x37:{ //удержание реле команд передатчика (общие)
      if (Rec_buf_data_uart[4]<2) MenuAllKeepComPRD=Rec_buf_data_uart[4];
      else  MenuAllKeepComPRD=0x02;
    }break;
    case 0x3B:{ //задержка на возврат
      if ( (Rec_buf_data_uart[4] < 101) && (Rec_buf_data_uart[4] > 9) ){
        if (Rec_buf_data_uart[4] == 100)
        {
          MenuReturnDelay[0] = '1';
          MenuReturnDelay[1] = '0';
        }
        else
        {
          MenuReturnDelay[0] = '0';
          MenuReturnDelay[1] = Rec_buf_data_uart[4] / 10 + 0x30;
        }
        MenuReturnDelay[2] = '0';
        MenuReturnDelay[3] = '0';
      }else{
        MenuReturnDelay[0] = '?';
        MenuReturnDelay[1] = '?';
        MenuReturnDelay[2] = '?';
        MenuReturnDelay[3] = '?';
      }
    }break;
    case 0x39:{ //время перезапуска (общие)
       if (Rec_buf_data_uart[4]<=5) MenuAllTimeRerun[0]=Rec_buf_data_uart[4]+0x30;
       else MenuAllTimeRerun[0]='?';
    }break;
    case 0x3A:{ //задержка приемника (общие)
      if (Rec_buf_data_uart[4] == 1)
      {
         if (Rec_buf_data_uart[5] <= 10)
         {
           MenuZadTR[0] = (Rec_buf_data_uart[5] / 10) + 0x30;
           MenuZadTR[1] = (Rec_buf_data_uart[5] % 10) + 0x30;
         }
         else
         {
            MenuZadTR[0] = '?';
            MenuZadTR[1] = '?';
         }
      }
      if (Rec_buf_data_uart[4] == 2)
      {
        if (Rec_buf_data_uart[5] <= 10)
        {
          MenuZadRe[0] = (Rec_buf_data_uart[5] / 10) + 0x30;
          MenuZadRe[1] = (Rec_buf_data_uart[5] % 10) + 0x30;
        }
        else
        {
          MenuZadRe[0] = '?';
          MenuZadRe[1] = '?';
        }
      }
    }break;
    case 0x3C:{ //порог предупреждения по КЧ (общие)
        if (Rec_buf_data_uart[4]>22) {MenuAllCF[0]=0x3F;MenuAllCF[1]=0x3F;}
        else{
            MenuAllCF[0]=Rec_buf_data_uart[4]/10+0x30;
            MenuAllCF[1]=Rec_buf_data_uart[4]%10+0x30;
        }
        if (Rec_buf_data_uart[5]>22) {MenuVoltageLimit1[0]=0x3F;MenuVoltageLimit1[1]=0x3F;}
        else{
            MenuVoltageLimit1[0]=Rec_buf_data_uart[5]/10+0x30;
            MenuVoltageLimit1[1]=Rec_buf_data_uart[5]%10+0x30;
        }
        if (Rec_buf_data_uart[6]>22) {MenuVoltageLimit2[0]=0x3F;MenuVoltageLimit2[1]=0x3F;}

        else{
            MenuVoltageLimit2[0]=Rec_buf_data_uart[6]/10+0x30;
            MenuVoltageLimit2[1]=Rec_buf_data_uart[6]%10+0x30;
        }
    }break;
    case 0x3D:{ //контроль выходного сигнала (общие)
      if (Rec_buf_data_uart[4]<2) MenuAllControlUout=Rec_buf_data_uart[4];
      else MenuAllControlUout=0x02;
    }break;
  }
  RecivVar=1;
  LCD2new=1;
};

void FTest1(void){

  gr1=0; gr2=0; gr3=0; gr4=0;
    //первый приемник
    //Rec_buf_data_uart[4] - 0000[рз2][рз1][кч2][кч1]
    //Rec_buf_data_uart[5] - [ком8][ком7][ком6][ком5][ком4][ком3][ком2][ком1]
    if (Rec_buf_data_uart[4]&0x01) gr1=1;
    else
        if (Rec_buf_data_uart[4]&0x02) gr1=2;
        else{
            if (Rec_buf_data_uart[5]>0){
                gr1=2;
                do{
                    gr1++;
                    Rec_buf_data_uart[5]=Rec_buf_data_uart[5]>>1;
                }while(Rec_buf_data_uart[5]);
            }
        }
    if (Rec_buf_data_uart[4]&0x04) gr2=1;
    else
        if (Rec_buf_data_uart[4]&0x08) gr2=2;

    //второй приемник
    //Rec_buf_data_uart[7] - 0000[рз2][рз1][кч2][кч1]
    //Rec_buf_data_uart[8] - [ком8][ком7][ком6][ком5][ком4][ком3][ком2][ком1]
    if (Rec_buf_data_uart[7]&0x01) gr3=1;
    else
        if (Rec_buf_data_uart[7]&0x02) gr3=2;
        else{
            if (Rec_buf_data_uart[8]>0){
                gr3=2;
                do{
                    gr3++;
                    Rec_buf_data_uart[8]=Rec_buf_data_uart[8]>>1;
                }while(Rec_buf_data_uart[8]);
            }
        }
    if (Rec_buf_data_uart[7]&0x04) gr4=1;
    else
        if (Rec_buf_data_uart[7]&0x08) gr4=2;


  RecivVar=1;
  LCD2new=1;

}

void VersDevice(void){
  bool CorrectVers;

  CorrectVers=true;

  if (Rec_buf_data_uart[4]==1){
    bDef=true; //пост есть
  }else CorrectVers=false;

  if (Rec_buf_data_uart[5]==2){
    cNumComR1=8;
  }else CorrectVers=false;

  if (Rec_buf_data_uart[6]==2){
    cNumComR2=8;  //кол-во команд на ПРМ2
  }else CorrectVers=false;

  if (Rec_buf_data_uart[7]==2){
    cNumComT=8;  //кол-во кмнанд на ПРД
  }else CorrectVers=false;

  if (Rec_buf_data_uart[8]==3){
    cNumLine=3; //3-х концевая версия
    cNumComR=8;
  }else CorrectVers=false;

  if (Rec_buf_data_uart[9]==1){
    cTypeLine=1;
  }else CorrectVers=false;

  MyInsertion[1]=(Rec_buf_data_uart[10]<<8)+Rec_buf_data_uart[11];  //версия АТмега БСП
  MyInsertion[2]=(Rec_buf_data_uart[12]<<8)+Rec_buf_data_uart[13];  //версия DSP

  CorrectVers = true;
  bReadVers=CorrectVers;

  sArchive.NumDev=4;
  sArchive.Dev[0]=0;
  sArchive.Dev[1]=3;
  sArchive.Dev[2]=2;
  sArchive.Dev[3]=1;
}

void FArchive(void)
{
	// Флаг принятия команды архива соответствующей текущему пункту
	// false - ошибка
	bool er = false;
	
	RecivVar=1;
	LCD2new=1;
	
	//проверка на соответствие принятой команды, текущему пункту меню
	if ((0x0F - (Rec_buf_data_uart[2]>>4)) == sArchive.Dev[sArchive.CurrDev])  
		er = true;
	
	if ((Rec_buf_data_uart[2]&0x0F)==0x01)
	{ //кол-во записей архива
		if (er)
		{  //если получена команда этого архива
			if (Rec_buf_data_uart[5]) 
			{
				sArchive.RecCount = 256;
				sArchive.CurCount = Rec_buf_data_uart[4];
			}
			else
			{
				sArchive.RecCount = Rec_buf_data_uart[4];
				sArchive.CurCount = 0;
			}
		}
	}
	
	if ((Rec_buf_data_uart[2]&0x0F)==0x02)
	{ 	//записи архива
		unsigned char i;
		if(er)
		{	//если получена команда этого архива
			for(i=0; i<16; i++) 
				sArchive.Data[i] = Rec_buf_data_uart[4+i];
		}else{
			for(i=0; i<16; i++) 
				sArchive.Data[i] = 0x00;
		}
	}
}


extern strCorrParam sCorrParam[];

static void fCorrParam(void){
	int8_t temp1, temp2;
	int16_t temp3 = 0;
	uint8_t step;

	//напряжение
	temp1 = (int8_t) Rec_buf_data_uart[4];
	temp2 = (int8_t) Rec_buf_data_uart[5];
	sCorrParam[COR_VOLTAGE].Corr = 0;
	if ((temp1 > 99) || (temp1 < -99) || (temp2 > 90) || (temp2 < -90)) {  
		// вне диапазона
		sCorrParam[COR_VOLTAGE].Print[0]='?';
		sCorrParam[COR_VOLTAGE].Print[1]='?';
		sCorrParam[COR_VOLTAGE].Print[2]=0x00;
	} else {
		step = 0;
		//знак коррекции
		uint8_t sign = ((temp1|temp2)&0x80) ? '-' : '+';
		sCorrParam[COR_VOLTAGE].Print[step++] = sign; 
		
		// после того как узнали знак коррекции, 
		// работаем с положительными числами
		temp1 = (temp1 < 0) ? -temp1 : temp1;
		temp2 = (temp2 < 0) ? -temp2 : temp2;
		temp2 /= 10;
		// если коррекция нулевая, заполним массив сразу
		if ((temp1 == 0) && (temp2 == 0)) {
			temp3 = 0;
			sCorrParam[COR_VOLTAGE].Print[0] = '0';
			sCorrParam[COR_VOLTAGE].Print[1] = 0x00;
		} else {
			temp3 = ((int16_t) temp1)*10 + temp2;
			temp3 = (sign == '-') ? -temp3 : temp3;
			sCorrParam[COR_VOLTAGE].Corr = temp3;
			// если коррекция >= 10В, запишем старший разряд
			if (temp1 > 9) {
				sCorrParam[COR_VOLTAGE].Print[step++] = (temp1/10) + '0';
				temp1 %= 10;
			}
			sCorrParam[COR_VOLTAGE].Print[step++] = temp1 + '0';
			sCorrParam[COR_VOLTAGE].Print[step++] = '.';
			sCorrParam[COR_VOLTAGE].Print[step++] = temp2 + '0';
			sCorrParam[COR_VOLTAGE].Print[step++] = 'В';
			sCorrParam[COR_VOLTAGE].Print[step++] = 0x00;
		}
		
	}
	// ток

	temp3 = ((int16_t)(Rec_buf_data_uart[6]<<8)) + Rec_buf_data_uart[7];
	sCorrParam[COR_CURRENT].Corr = 0; 			
	if ((temp3 > 999) || (temp3 < -999)) {	 		//вне диапазона
		sCorrParam[COR_CURRENT].Print[0]='?';
		sCorrParam[COR_CURRENT].Print[1]='?';
		sCorrParam[COR_CURRENT].Print[2]=0x00;
	} else {
		if (temp3 == 0) { 						// нулевая коррекция
			sCorrParam[COR_CURRENT].Print[0]='0';
			sCorrParam[COR_CURRENT].Print[1]=0x00;
		} else {
			step = 0;
			sCorrParam[COR_CURRENT].Corr = temp3;
			sCorrParam[COR_CURRENT].Print[step++] = (temp3 > 0) ? '+' : '-';
			temp3 = (temp3 < 0) ? -temp3 : temp3;
			if (temp3 > 99) {
				sCorrParam[COR_CURRENT].Print[step++]=(temp3 / 100) + '0';
				temp3 = temp3 % 100;
			}
			if (temp3 > 9) {
				sCorrParam[COR_CURRENT].Print[step++]=(temp3 / 10) + '0';
				temp3 = temp3 % 10;
			}
			sCorrParam[COR_CURRENT].Print[step++] = temp3 + '0';
			sCorrParam[COR_CURRENT].Print[step++] = 'м';
			sCorrParam[COR_CURRENT].Print[step++] = 'А';
			sCorrParam[COR_CURRENT].Print[step++] =0x00;
			
		}
	}
}

//обработка принятого сообщения
void DataModBus(unsigned char NumberByte)
{	
	//прверка принятого CRC
	CRCSum = GetCRCSum(Rec_buf_data_uart, NumberByte - 1);
    if (CRCSum != Rec_buf_data_uart[NumberByte - 1]) 	
	{
		EnableReceive;
	}
    else
    {
		//for (i_dc=0; i_dc<NumberByte; i_dc++) Tr_buf_data_uart1[i_dc]=Rec_buf_data_uart[i_dc];
        //TransDataInf1(Tr_buf_data_uart1[2], Tr_buf_data_uart1[3]);
		
//		uint8_t com = Rec_buf_data_uart[2];
//		if ( (com == 0xC2) || (com == 0xF2) || (com == 0xD2) || (com == 0xE2) )
//		{
//			for (uint8_t i = 0; i < NumberByte; i++) 
//				Tr_buf_data_uart1[i]=Rec_buf_data_uart[i];
//        	TransDataInf1(Tr_buf_data_uart1[2], Tr_buf_data_uart1[3]);
//		}

		
        if (PCready == 2)
		{
        	for (i_dc = 2; i_dc < NumberByte; i_dc++)
				Tr_buf_data_uart1[i_dc] = Rec_buf_data_uart[i_dc];
            PCready = 3;
            PCbyte = NumberByte;
        }

        if (PCready==0)
        {
        	switch(Rec_buf_data_uart[2]&0xF0){
            	case 0x00:
				{	//параметры защиты
					FParamDef(Rec_buf_data_uart[2]);
                }break;
				case 0x10:
				{	//параметры приемника
					FParamPrm(Rec_buf_data_uart[2]); 	
				}
				break;
				case 0x20:
				{	//параметры передатчика
					FParamPrd(Rec_buf_data_uart[2]);
				}
				break;
                case 0x30:
				{
                	switch(Rec_buf_data_uart[2])
					{
                    	case 0x30:  {FCurrentState();} break; 			// принято текущее состояние
                        case 0x31:  {FGlobalCurrentState();} break;  	// принято общее текущее состояние
                        case 0x32:  {FDataTime();} break;  				// пришли данные дата/время
						case 0x33: 	{fCorrParam();} break;				// коррекция тока/напряжения
                        case 0x34:  {FMeasureParam();} break; 			// пришли измеряемые параметры
                        case 0x36:  									// удержание реле команд приемника (общие)
                        case 0x37:  									// удержание реле команд передатчика (общие)
						case 0x39:
                        case 0x3C:
                        case 0x3D:  {FParamGlobal(Rec_buf_data_uart[2]);}break;
                        case 0x3E:  FTest1(); break; 					// принято значение Теста
                        case 0x3F:  VersDevice(); break; 				// информация об аппарате
                	};
             	}break;
                default:
				{
                	switch(Rec_buf_data_uart[2])
					{
                        case 0xC1:
                        case 0xD1:
                        case 0xE1:
                        case 0xF1:
                        case 0xC2:
                        case 0xD2:
                        case 0xE2:
                        case 0xF2: FArchive(); break; //архив событий
                    }
                }
			}
            EnableReceive;
		}
 	}
	ClearPortError();   //после начала отправки сообщения очистим регистр приема
    bUartRcReady1 = false;  //обработка сообщения закончена
} //end DataModBus


