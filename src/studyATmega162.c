#include <ioavr.h>
#include <ina90.h>
#include "MyDef.h"
#include "StartSetup.h"
#include "InterfaceS.h"
#include "DataCenter.h"
#include "DataCenter1.h"
#include "driverLCD.h"
#include "WatchDog.h"
#include "Flash.h"
#include "Menu.h"

//определяет цикл работы программы
#define LoopUARTtime 10

//массив "дата/время" передаваемый в USP
extern unsigned char TrDataTimeMass[];

//внешние переменные для записи в EEPROM
extern unsigned char eWrite, eRead;
extern unsigned char *eMassiveWrite, *eMassiveRead;
extern unsigned int eAddressWrite,eAddressRead;

unsigned char PressKey;
unsigned char eNumberAskMeasuring[]={0x00,0x00};

unsigned int MyInsertion[]={Insertion, 0x0000, 0x0000};

unsigned int DataM[250];
unsigned int JournalM[129]; //первый элемент - количество записей в архиве

unsigned char LoopUART;
unsigned char LCD2new;
unsigned char LCDtimerNew = 1, LCDparam = 1;
unsigned char LCDtimer = 2;
unsigned char TransVar;
unsigned char RecivVar;
unsigned char NumberLostLetter = 0;
unsigned char AvarNoUsp = 0;
unsigned char NewTime = 0;
unsigned char MenuLevel = 1;
unsigned char PreduprMenu1[4]={0x01,0x01,0x01,0x00};
unsigned char AvarMenu1[4]={0x01,0x01,0x01,0x00};

bool TimeWink;

unsigned char NumberCom=1;

unsigned char CodePrint1, PCconn, PassCen;

unsigned char InputDataTime[]="??.??.??";
unsigned char DataLCD[]="??.??.??";
unsigned char TimeLCD[]="??:??:??";

//unsigned char Iline1H[]=  "1- ????h";
//unsigned char KovH[]=     "2- ??h  ";
//unsigned char PkH[]=      "3- ????h";
unsigned char Iline1[]=     "R=?? Ом  ";
unsigned char Iline2[]=     "I1=?? мА ";
unsigned char Uline[]=      "U=??,? B ";
unsigned char Kd[]=         "Uш=?? дБ ";
unsigned char Usigndef1[]=  "1Uз=?? дБ";
unsigned char Usigndef2[]=  "2Uз=?? дБ";
unsigned char Uinkch1[]=    "1Uк=?? дБ";
unsigned char Uinkch2[]=    "2Uк=?? дБ";
unsigned char *Measuring[]={Iline1, Iline2, Uline, Usigndef1, Uinkch1, Kd};

uchar uDopPar1[] = "1- ????h ";
uchar uDopPar2[] = "2- ????h ";
uchar uDopPar3[] = "3- ????h ";
uchar uDopPar4[] = "4- ??h   ";
uchar uDopPar5[] = "5- ??h   ";
uchar uDopPar6[] = "6- ??h   ";
uchar *MeasDop[] ={	uDopPar1, uDopPar2, uDopPar3, uDopPar4, uDopPar5, uDopPar6};

unsigned char GlobalCurrentState[]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0x00};
unsigned char CurrentState[]={0x4E,0x4E,0x4E,0x4E,0x4E,0x4E,0x4E,0x4E,0x00};
unsigned char Dop_byte[]={0x3F, 0x3F, 0x3F, 0x3F};

//Тесты
unsigned int TestDelay=0; //блок клаиватуры после входа в тестовый режим
//задержка в мс
#define Delay1 49
//Тестовый режим №1
unsigned char gr1='?', gr2='?', gr3='?', gr4='?';;

unsigned char TempCurrState; //переменная нужна для отображения режима в меню Установить\режим

//переменные необходимые для FuncInputData
unsigned char NumberInputSymbol;
unsigned char NewPass=0, EntPass=0;
unsigned char TempLine;

//параметры Поста

unsigned char NumberTransCom; //байт в котором хранится номер передоваемой "команды на приеме"

//Параметры Общие
unsigned char MenuAllKeepComPRM		= 0x02; 	//для него используется массив MenuAllSynchrTimerNum
unsigned char MenuAllKeepComPRD		= 0x02; 	//для него используется массив MenuAllSynchrTimerNum
unsigned char MenuAllTimeRerun[]	= "? сек";
unsigned char MenuAllControlUout	= 0x02; 	//для него используется массив MenuAllSynchrTimerNum
unsigned char MenuAllCF[]			= "?? дБ";
unsigned char MenuVoltageLimit1[]	= "?? дБ";
unsigned char MenuVoltageLimit2[]	= "?? дБ";
//наши параметры
unsigned char MenuReturnDelay[]	= "???? мс";
unsigned char MenuZadTR[]		= "?? мс";
unsigned char MenuZadRe[]		= "?? мс";
//параметры Приемника
unsigned char MenuPrmTimeOn[]	= "?? мс";
unsigned char ValuePrmTimeOff[] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
unsigned char MenuPrmTimeOff[] 	= "???? мс";
unsigned char ValuePrmBlockCom	= '?';
unsigned char MenuPrmBlockCom[]	= "????";
unsigned char *MenuParamPRM[] 	= {MenuPrmTimeOn, MenuPrmTimeOff, MenuPrmBlockCom};

//параметры передатчика
unsigned char MenuPrdTimeOn[]	= "?? мс";
unsigned char MenuPrdTimeCom[]	= "??? мс";
unsigned char ValuePrdBlockCom	= '?';
unsigned char MenuPrdBlockCom[]	= "????";
unsigned char ValuePrdLongCom;
unsigned char MenuPrdLongCom[]	= "????";
unsigned char ValuePrdCtrlByte;
unsigned char MenuPrdCtrlByte[]	= "????????";    //байт управления
unsigned char* MenuParamPRD[] 	=
{
	MenuPrdTimeOn, 
	MenuPrdTimeCom, 
	MenuPrdLongCom, 
	MenuPrdBlockCom, 
	MenuPrdCtrlByte
};

unsigned char DopComTrans;

//параметры поста
unsigned char MenuTypeDefend = defNumTypeDef;
unsigned char MenuPossibleTimeSignal1[]="?? град";
unsigned char MenuPossibleTimeSignal2[]="?? град";
unsigned char MenuVoltageLimit[]="?? дБ";
unsigned char MenuRZalarm[]="?? дБ";
unsigned char MenuTypeLine = 2;
unsigned char MenuTimeNoMan[]="?? час";
unsigned char MenuOverlapPulse[]="?? град";

//переменная отвечающая за вывод на экран Значения параметра или его допустимый диапазон
unsigned char ValueVsRange = 0;

//переменные необходимые для прокрутки меню
unsigned char ShiftMenu=0;
unsigned char MaxShiftMenu=0;
unsigned char MaxDisplayLine=0;


unsigned char TimeInitLCD=0; //переменная отвечающая за время переинициализации ЖК
//определяет период инициализации ЖК в сек.
#define ConstTimeInitLCD 5

unsigned char Menu1Temp[]={0x30,0x20,0x20,0x20,0x20,0x20,0x20,0x20,0x00};
//переменные для ввода инф-ы с клавиатуры
unsigned char InputValue[]={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
unsigned char NumberInputChar=0x00;
unsigned char MaxNumberInputChar=0x00;
unsigned char*  ChangeMass;
unsigned char WorkRate=0x00;
unsigned char ByteShift=0x00;
unsigned char Discret=0x00; //определяет дискретность вводимого числа
unsigned int MaxValue, MinValue;
unsigned int TempValueD; //введенное число до дискретизации
unsigned int TempValue; //введенное число, после дискретизации
unsigned char TempValueBCD;
unsigned char InputParameter; //опеределяет какой параметр вводят с клавиатуры, по умолчанию = 0
unsigned char ControlParameter; //определяет верно ли был введен параметр
unsigned char Winking=0x00; //мигание курсора
__flash unsigned char WinkingMass[]="_";

__flash unsigned char PCconnect[]="Связь с ПК";
__flash unsigned char Point[]=".";

//массив хранения пароля
unsigned char PressPassword = 2;
__flash unsigned char Password[]={0x30,0x30,0x30,0x30,0x00};
unsigned char ePassword[]={0x00,0x00,0x00,0x00,0x00};

//переменные для запрса измеряемых параметров
unsigned char NumberAskMeasuring=0x01;

//переменные выбора значения параметра из списка
unsigned char InputSelectValue;
unsigned char SelectValue;
unsigned char MaxSelectValue, MinSelectValue;
unsigned char __flash* __flash* MassSelectValue;
unsigned char TrParam, TrValue;
unsigned int TrValueD;

unsigned char PosModBusInfill=0, ModBusInfill=0;

//Архив
unsigned int NumberRecording;
unsigned char NumberRec;
unsigned char ReadArch = 0; // 1 - означает идет чтение архива, 2 - чтение закончено, 0 - архив не нужен
extern unsigned int NumRecStart;
unsigned char ComArch; //номер команды для считывания необходимого архива
strArchive sArchive;  //структура архива

//для единой версии
bool bDef; //false - нет, true-есть Защита.
unsigned char cNumComR, cNumComT,cNumComR1,cNumComR2; //кол-во команд на прием и передачу
bool bInpVal=false;  //разрешение на ввод нового значения параметра
unsigned char cTypeLine;  //тип линии 1/2/3 = ВЛ/ВОЛС/Е1
unsigned char cNumLine; //2-3 концевая линия
bool bReadVers=false; //true - версия была считана, false - ответ не пришел, продолжаем опрос
bool bLCDwork;  //разрешение обновления экрана
//для вывода на эран статуса устройств, в меню 0
bool bGlobalAvar=false, bGlobalWarn=false;
bool bDefAvar=false, bDefWarn=false;
bool bRec1Avar=false, bRec1Warn=false;
bool bTrAvar=false, bTrWarn=false;
bool bRec2Avar=false, bRec2Warn=false;
unsigned char cNumPrm;  //выбор 1/2 приемника в 3-х концевой версии
unsigned char LineInMenu6=0; //кол-во строк в меню просмотр параметров/ установить параметр

bool bParamView = false; //есть/нет вывод на экран дополнительных параметров
bool bParamValue = true; //true - норм отображение, false - hex вид


//фунцкия выбора значения параметра из списка
void FuncSelecValueInputParam(void)
{
	LCDprintf(4,1,2,MenuInputData,1); //выводим на экран "Ввод:"
	FuncClearCharLCD(4,6,15); //очищаем 4-ую строку
	LCDprintf(4,7,2,MassSelectValue[InputSelectValue],1);
	switch(PressKey)
    {
	case 'U': {
        if (InputSelectValue>MinSelectValue) InputSelectValue--;
	} break;
	case 'D': {
        if (InputSelectValue<MaxSelectValue)  InputSelectValue++;
	} break;
	case 'C': {
        WorkRate=0x00; LCDbufClear(); LCD2new=1; InputSelectValue=0x00;
        MaxSelectValue=0; MinSelectValue=0;TrParam=0;
	}break;
	case 'E': {
		if (MenuLevel == 13)
		{
			// при вводе Типа линии, переменная на 1 меньше, чем надо отослать в БСП
			if (SelectValue == 2)	
				InputSelectValue++;
		}
		
		if (MenuLevel == 11)
		{
			// при вводе изменения режима без пароля
			// идет смещение на 1 пункт
			if ( (SelectValue == 2) && (InputSelectValue > 0) )
				InputSelectValue++;
		}
		
		TrParam=SelectValue; TrValue=InputSelectValue;
		
		if (MenuLevel == 19)
		{	// меню прошивок
			if (SelectValue == 3) 
			{
				bParamValue = InputSelectValue;
				TrParam = 0;
			}
		}
		else if (MenuLevel == 11)
		{
			if (((SelectValue==1)&&(InputSelectValue>1))||((SelectValue==2)&&(InputSelectValue>0))) //ввод теста
                if ((CurrentState[2]<0x04)||(CurrentState[4]<0x04)||(CurrentState[0]<0x04))   /*если хоть 1 устройство не в Тест 1*/
					TestDelay=Delay1; //сделаем паузу при вводе в Тест
		}
		
        LCDbufClear();LCD2new=1; WorkRate=0x00;
	}break;
    }
	PressKey=0xF0;
}

void FuncInputDataTime(void)
{
	unsigned char num = NumberInputChar;	// текущий символ
	unsigned char poz = 7 + num; 			// текущая позиция курсора
	unsigned char key = PressKey;
	
	// очистка строки, и ее заполнение
	FuncClearCharLCD(4, 6, 15); 			
	LCDprintf(4, 1, 2, MenuInputData, 1); 	
	LCDprint(4, 7, 2, InputDataTime, 1);
	
	//вывод мигающего курсора
	if (NumberInputChar < 8)
	{ 
		if (Winking == 1)
		{
			LCDprintf(4, poz , 2, WinkingMass, 0);
			Winking = 0;
		}
		else 
			Winking = 1;
	}
	
	if (key != 0xF0)
	{
		if (key == 'E')
		{
			// перед отправкой проверим данные на достоверность
			if (InputParameter == 1)	
			{	// дата
				unsigned char year 	= ( (InputDataTime[6] - '0') * 10) + (InputDataTime[7] - '0');
				unsigned char month = ( (InputDataTime[3] - '0') * 10) + (InputDataTime[4] - '0');
				unsigned char day	= ( (InputDataTime[0] - '0') * 10) + (InputDataTime[1] - '0');
				
				// год
				TrDataTimeMass[0] = ( (InputDataTime[6] - '0') << 4) + (InputDataTime[7] - '0');
				
				// месяц, если не правильный, будет 1
				if (month < 12)
				{
					TrDataTimeMass[1] = ( (InputDataTime[3] - '0') << 4) + (InputDataTime[4] - '0');
				}
				else
				{
					month = 1;
					TrDataTimeMass[1] = 0x01;
				}
				
				switch(month)
				{
				case 2:		// 28-29 дней, февраль
					if (year % 4)
					{
						if (day <= 28)
							TrDataTimeMass[2] = ( (InputDataTime[0] - '0') << 4) + (InputDataTime[1] - '0');
						else
							TrDataTimeMass[2] = 0x01;
					}
					else
					{
						if (day <= 29)
							TrDataTimeMass[2] = ( (InputDataTime[0] - '0') << 4) + (InputDataTime[1] - '0');
						else
							TrDataTimeMass[2] = 0x01;
					}
					break;
				case 4:
				case 6:
				case 9:
				case 11:	// 30 дней
					if (day <= 30)
						TrDataTimeMass[2] = ( (InputDataTime[0] - '0') << 4) + (InputDataTime[1] - '0');
					else
						TrDataTimeMass[2] = 0x01;
					break;
				default:
					if (day <= 31)
						TrDataTimeMass[2] = ( (InputDataTime[0] - '0') << 4) + (InputDataTime[1] - '0');
					else
						TrDataTimeMass[2] = 0x01;
					break;
				}
			}
			else if (InputParameter == 2)
			{	// время
				unsigned char hour	 = (InputDataTime[0] - '0') * 10 + (InputDataTime[1] - 0x30);
				unsigned char minute = (InputDataTime[3] - '0') * 10 + (InputDataTime[4] - '0');
				unsigned char second = (InputDataTime[6] - '0') * 10 + (InputDataTime[7] - '0');
				
				if (hour < 24)
					TrDataTimeMass[3] = ( (InputDataTime[0] - '0') << 4) + (InputDataTime[1] - '0');
				else
					TrDataTimeMass[3] = 0x00;
				
				if (minute < 60)
					TrDataTimeMass[4] = ( (InputDataTime[3] - '0') << 4) + (InputDataTime[4] - '0');
				else
					TrDataTimeMass[4] = 0x00;
				
				if (second < 60)
					TrDataTimeMass[5] = ( (InputDataTime[6] - '0') << 4) + (InputDataTime[7] - '0');
				else
					TrDataTimeMass[5] = 0x00;
			}
			
			NewTime = 1;
			WorkRate = 0;
			num = 0;
			FuncClearCharLCD(4, 1, 20); 
			LCD2new = 1;
			
		}
		else if (key == 'C')
		{
			WorkRate = 0x00;
			NumberInputChar = 0;
			FuncClearCharLCD(4, 1, 20); 
			LCD2new = 1;
		}
		else if (key == 'U')
		{
			if (num < 7)
				num++;
			if ( (num == 2) || (num == 5) )
				num++;
		}
		else if (key == 'D')
		{
			if (num > 0)
				num--;
			if ( (num == 2) || (num == 5) )
				num--;
		}
		else if ( (key >= '0') && (key <= '9') )
		{
			if (InputParameter == 1)
			{	// дата
				switch(num)
				{
				case 0:
					if (key == '0')
					{
						InputDataTime[num++] = key;
						if (InputDataTime[num] == '0')
							InputDataTime[num] = '1';
					}
					else if (key < '3')
					{
						InputDataTime[num++] = key;
					}
					else if (key == '3')
					{
						InputDataTime[num++] = key;
						if (InputDataTime[num] > '1')
							InputDataTime[num] = '0';
					}
					break;
				case 1:
					if (key == '0')
					{
						if (InputDataTime[num - 1] > '0')
							InputDataTime[num++] = key;
					}
					else if (key == '1')
					{
						InputDataTime[num++] = key;	
					}
					else
					{
						if (InputDataTime[num - 1] < '3')
							InputDataTime[num++] = key;
					}
					break;
				case 3:
					if (key == '0')
					{
						InputDataTime[num++] = key;
						if (InputDataTime[num] == '0')
							InputDataTime[num] = '1';
					}
					else if (key == '1')
					{
						InputDataTime[num++] = key;
						if (InputDataTime[num] > '2')
							InputDataTime[num] = '0';
					}
					break;
				case 4:
					if (key == '0')
					{
						if (InputDataTime[num - 1] > '0')
							InputDataTime[num++] = key;
					}
					else 
					{
						if (InputDataTime[num - 1] == '1') 
						{
							if (key < '3')
								InputDataTime[num++] = key;
						}
						else
							InputDataTime[num++] = key;
					}
					break;
				case 6:
					InputDataTime[num++] = key;
					break;
				case 7:
					InputDataTime[num++] = key;
					break;
				}
			}
			else if (InputParameter == 2)
			{	// время
				switch(num)
				{
				case 0:
					if (key == '0')
					{
						InputDataTime[num++] = key;
						if (InputDataTime[num] == '0')
							InputDataTime[num] = '1';
					} 
					else if (key < '2')
					{
						InputDataTime[num++] = key;				
					}
					else if (key == '2')
					{
						InputDataTime[num++] = key;
						if (InputDataTime[num] > '3')
							InputDataTime[num] = '0';
					}
					break;
				case 1:
					if (key == '0')
					{
						if (InputDataTime[num - 1] > '0')
							InputDataTime[num++] = key;
					}
					else if (key < '4')
					{
						InputDataTime[num++] = key;
					}
					else
					{
						if (InputDataTime[num - 1] < '2')
							InputDataTime[num++] = key;
					}
					break;
				case 3:
				case 6:
					if (key == '0')
					{
						InputDataTime[num++] = key;
						if (InputDataTime[num] == '0')
							InputDataTime[num] = '1';
					}
					else if (key < '6') 
					{
						InputDataTime[num++] = key;
					}
					break;
				case 4:
				case 7:
					if (key == '0')
					{
						if (InputDataTime[num - 1] > '0')
							InputDataTime[num] = key;
					}
					else 
						InputDataTime[num++] = key;
					break;
				}
			}
			
			if ( (num == 2) || (num == 5) )
				num++;
		}
		
		NumberInputChar = num;
		PressKey = 0xF0;
	}
}


//функция ввода значения с клавиатуры
void FuncInputData(void)
{
	FuncClearCharLCD(4,6,15); //очищаем 4-ую строку
	//Выводим на индикатор "ВВод:", "Пароль:" или "Новый пароль:"
	if (MenuLevel==5){
		if (PressPassword==2) LCDprintf(4,1,2,MenuInputOldPassword,1);
		if (PressPassword==1) LCDprintf(4,1,2,MenuInputNewPassword,1);
	}else
		if ((MenuLevel==11)&&(PressPassword==2)) LCDprintf(4,1,2,MenuInputOldPassword,1);
		else LCDprintf(4,1,2,MenuInputData,1); //выводим на экран "Ввод:"
		
		if (NumberInputChar<MaxNumberInputChar) //если кол-во введеных символов еще меньше максимальновозможного
			if ( (InputParameter == 0xA5) || (InputParameter == 0x94) || (InputParameter == 0xA4) || (InputParameter == 0xA6)){
				if ((PressKey=='0')||(PressKey=='1')) {InputValue[NumberInputChar]=PressKey;NumberInputChar++;}
			}else
				if ((PressKey>='0')&&(PressKey<='9')) {InputValue[NumberInputChar]=PressKey;NumberInputChar++;} //реакция на нажатие цифры
		
		if (PressKey=='#')
			if ((MenuLevel>=13)&&(MenuLevel<=16)) {ValueVsRange++;if (ValueVsRange>1) ValueVsRange=0; LCD2new=1;}
		
		//вывод мигающего курсора, пока не введено макс. кол-во символов
		if (NumberInputChar<MaxNumberInputChar)
			if (Winking==1){
				if (MenuLevel==5){
					if (PressPassword==2) LCDprintf(4,9+NumberInputChar,2,WinkingMass,0);
					if (PressPassword==1) LCDprintf(4,15+NumberInputChar,2,WinkingMass,0);
				}else
					if ((MenuLevel==11)&&(PressPassword==2)) LCDprintf(4,9+NumberInputChar,2,WinkingMass,0);
					else LCDprintf(4,7+NumberInputChar,2,WinkingMass,0);
					Winking=0;
			}
			else Winking=1;
		//стирание предыдущего символа
		if ((PressKey=='D')&&(NumberInputChar>0)) {NumberInputChar--;InputValue[NumberInputChar]=0x00;}
		//вывод на индикатор введенного значения
		if (MenuLevel==5)
		{if (PressPassword==2) LCDprint(4,9,2,InputValue,1);if (PressPassword==1) LCDprint(4,15,2,InputValue,1);}
		else
			if ((MenuLevel==11)&&(PressPassword==2)) LCDprint(4,9,2,InputValue,1); else LCDprint(4,7,2,InputValue,1);
			
			//сброс введенного числа
			if (PressKey=='C'){
				//работа с паролем
				if (PressPassword==1) PressPassword=2;
				//если режим не Выведен, Тест1-3 , то сбросим EntPass
				if ((bDef)&&(CurrentState[0]==2)) EntPass=0;
				if ((cNumComR>0)&&((CurrentState[2]==1)||(CurrentState[2]==2))) EntPass=0;
				if ((cNumComT>0)&&(CurrentState[4]==2)) EntPass=0;
				
				if (EntPass==1) PassCen=1;
				WorkRate=0x00; LCDbufClear(); LCD2new=1;
				for(NumberInputChar=0; NumberInputChar<8;NumberInputChar++) InputValue[NumberInputChar]=0x00;
				ValueVsRange=0; NumberInputChar=0x00; MaxNumberInputChar=0x00; ByteShift=0;
				MaxValue=0; MinValue=0;
			}
			//обработка значений параметров, введенных 0 и 1
			if ( ((InputParameter == 0xA5) || (InputParameter == 0x94) || (InputParameter == 0xA4) || (InputParameter == 0xA6)) && (NumberInputChar==MaxNumberInputChar) && (PressKey=='E') ) 
			{
				
				WorkRate=0x00;
				LCDbufClear();
				LCD2new=1;
				TempValue=0;	
				TrValue = 0;
				for( char i = 0, j = MaxNumberInputChar; i < MaxNumberInputChar; i++)		
				{		
					j--;			
					TrValue += ( (InputValue[i] - 0x30) << j );
				}
				//TrValue=((InputValue[0]-0x30)*128)+((InputValue[1]-0x30)*64)+((InputValue[2]-0x30)*32)+((InputValue[3]-0x30)*16)+((InputValue[4]-0x30)*8)+((InputValue[5]-0x30)*4)+((InputValue[6]-0x30)*2)+((InputValue[7]-0x30));
				ControlParameter = 1;
				TrParam = InputParameter;
				// очищаем буферный массив
				for(NumberInputChar=0; NumberInputChar < 8; NumberInputChar++) InputValue[NumberInputChar]=0x00;
				NumberInputChar=0x00; MaxNumberInputChar=0x00; ByteShift=0; InputParameter = 0;
				MaxValue=0; MinValue=0;
			}
			
			//принятие введенного числa
			if ( (PressKey=='E')&&(MenuLevel!=2)&&(MenuLevel!=5)&&(MenuLevel!=11)&&(InputParameter!=0xA5)&&(InputParameter!=21)&&(InputParameter!=0x94)&&(InputParameter!=0xA4)&&(InputParameter!=0xA6)){
				WorkRate=0x00;LCDbufClear();LCD2new=1;
				
				switch(NumberInputChar){
				case 1:{
					TempValueD=(InputValue[0]-0x30)/Discret;TempValue=TempValueD*Discret;TempValueBCD=InputValue[0]-0x30;
					if ((TempValue>=MinValue)&&(TempValue<=MaxValue)) ControlParameter=1;
				}break;
				case 2:{
					TempValueD=((InputValue[0]-0x30)*10+(InputValue[1]-0x30))/Discret;
					TempValue=TempValueD*Discret;
					TempValueBCD=((InputValue[0]-0x30)<<4)+(InputValue[1]-0x30);
					if ((TempValue>=MinValue)&&(TempValue<=MaxValue)) ControlParameter=1;
				}break;
				case 3:{
					TempValueD=((InputValue[0]-0x30)*100+(InputValue[1]-0x30)*10+(InputValue[2]-0x30))/Discret;
					TempValue=TempValueD*Discret;
					if ((TempValue>=MinValue)&&(TempValue<=MaxValue)) ControlParameter=1;
				}break;
				case 4:{
					TempValueD=((InputValue[0]-0x30)*1000+(InputValue[1]-0x30)*100+(InputValue[2]-0x30)*10+(InputValue[3]-0x30))/Discret;
					TempValue=TempValueD*Discret;
					if ((TempValue>=MinValue)&&(TempValue<=MaxValue)) ControlParameter=1;
				}break;
				}
				//а теперь сформируем и отправим в USP введеное значение, если необходимо
				if (ControlParameter==1){
					switch(InputParameter){
					case 5:                                                     //общий параметр/порог по КЧ
					case 6:                                                     //общий параметр/уменьшение усиления 1 канала в дБ
					case 7:  {TrParam=InputParameter;TrValue=TempValue;} break; //общий параметр/уменьшение усиления 2 канала в дБ
					case 8:
					case 9:  
					case 10: {TrParam=InputParameter - 5;TrValue=TempValue;} break;	 	
					case 26: {TrParam=4;TrValue=TempValue;} break;
					case 27: {TrParam=6;TrValue=TempValue;} break;  //параметр поста/напряжение порога
					case 28: {TrParam=9;TrValue=TempValue;} break;  //параметр поста/порог предупреждения по РЗ
					case 0x91:
					case 0xA1:
					case 0xA2:
					case 0xA6:
						{
							TrParam = InputParameter;
							TrValue = TempValue;
						}
						break;
					case 0x92:
					case 0x93:		
						{
							TrParam = InputParameter;
							TrValue = TempValue/10;
						}
						break;
					}
					InputParameter=0;
					ControlParameter=0;
				}
				
				// очищаем буферный массив
				for(NumberInputChar=0; NumberInputChar<8;NumberInputChar++) InputValue[NumberInputChar]=0x00;
				
				ValueVsRange=0;NumberInputChar=0x00;MaxNumberInputChar=0x00;ByteShift=0;MaxValue=0; MinValue=0;
			}
			//если мы вводили пароль
			if ((PressKey=='E')&&(NumberInputChar==4)&&((PressPassword==1)||(PressPassword==2))&&((MenuLevel==5)||(MenuLevel==11))){
				WorkRate=0x00;LCDbufClear();LCD2new=1;
				//проверка введеного пароля если 2, если 1 ввод нового пароля
				if (PressPassword==2){
					//проверим совпадение с пользовательским паролем
					NumberInputChar=0;
					for (PressPassword=0; PressPassword<4; PressPassword++) if (InputValue[PressPassword]!=ePassword[PressPassword]) NumberInputChar++;
					//если пароль не совпал с пользовательским, сравним его с постоянным
					if (NumberInputChar!=0){
						NumberInputChar=0;
						for (PressPassword=0; PressPassword<4; PressPassword++) if (InputValue[PressPassword]!=Password[PressPassword]) NumberInputChar++;
					}
					//если пароль верен
					// NumberInputChar =0;
					
					if (NumberInputChar==0) {PressPassword=1;NewPass=1;}
					else{
						PressPassword=2;
						if ((bDef)&&(CurrentState[0]==2)) EntPass=0;
						if ((cNumComR>0)&&((CurrentState[2]==1)||(CurrentState[2]==2))) EntPass=0;
						if ((cNumComT>0)&&(CurrentState[4]==2)) EntPass=0;
						
						if (EntPass==1) PassCen=1;
					}
				}else{
					//ввели новый пароль
					if (PressPassword==1){
						ePassword[0]=InputValue[0];ePassword[1]=InputValue[1];ePassword[2]=InputValue[2];ePassword[3]=InputValue[3];
						//а теперь запишем пароль в EEPROM
						eWrite=1;eAddressWrite=0;eMassiveWrite=ePassword;
						PressPassword=2;
					}
				}
				// очищаем буферный массив
				for(NumberInputChar=0; NumberInputChar<4;NumberInputChar++) InputValue[NumberInputChar]=0x00;
				NumberInputChar=0x00;MaxNumberInputChar=0x00;ByteShift=0;MaxValue=0; MinValue=0;
			}
			PressKey=0xF0;
			if (PassCen==1) {PressKey='E'; PassCen=0;}
			if ((PressPassword==1)&&(MenuLevel==11)) PressKey='E';
			if ((PressPassword==1)&&(MenuLevel==5)&&(NewPass==1)) {PressKey='3'; NewPass=0;}
}

void PressInMenuJournal(char Key)
{  //нажатие в меню Журна -> переход в подпункты журнала
	MenuLevel=21;
	LCDbufClear();
	ShiftMenu=0;
	MaxDisplayLine=2;
	sArchive.CurrDev=Key-0x31;
	sArchive.RecCount=0;  //для начала сбросим кол-во данных
}

void Menu_Journal(void)
{
	MenuLevel=4;
	LCDbufClear();
	ShiftMenu=0;
	MaxDisplayLine=3;
	MaxShiftMenu=1;
	sArchive.Data[12]=0;
	FuncClearCharLCD(2,1,40);
	sArchive.RecCount=0;
	LCD2new=1;
}

void fCharToTimeOff(unsigned char byte)
{
	if (byte == 0xff)
	{
		MenuPrmTimeOff[0] = '?';
		MenuPrmTimeOff[1] = '?';
		MenuPrmTimeOff[2] = '?';
		MenuPrmTimeOff[3] = '?';
	}
	else
	{
		if (byte == 100)
		{
			MenuPrmTimeOff[0] = '1';
			MenuPrmTimeOff[1] = '0';
			MenuPrmTimeOff[2] = '0';
			MenuPrmTimeOff[3] = '0';
		}
		else
		{
			MenuPrmTimeOff[0] = '0';
			MenuPrmTimeOff[1] = (byte / 10) + '0';
			MenuPrmTimeOff[2] = (byte % 10) + '0';
			MenuPrmTimeOff[3] = '0';
			
		}
	}
}

//обработка нажатия кнопки
static void FuncPressKey(void)
{
	
	if (MenuLevel == LVL_START)
	{
		switch(PressKey)
		{
			case 'M':
			{
				LCDbufClear();
				MenuLevel = LVL_MENU;
				LCD2new = 1;
				LCDtimerNew = 1;
				ShiftMenu = 0;
				MaxShiftMenu = 1;
				MaxDisplayLine = 3;	
			}break;
			
			case '2':
			{
				(NumberAskMeasuring > 1) ? NumberAskMeasuring-- : NumberAskMeasuring = 6;
				eNumberAskMeasuring[0] = NumberAskMeasuring;
				eWrite = 1;
				eAddressWrite = 5;
				eMassiveWrite = eNumberAskMeasuring;
			}break;
			
			case '4':	
			case '6':		// Переключение отображения даты/времени
			LCDtimer = (LCDtimer == 1) ? 2 : 1;
			break; 
			
			case '8':		// Прокрутка измеряемых параметров
			{  
				(NumberAskMeasuring < 6) ? NumberAskMeasuring++ : NumberAskMeasuring=0;
				eNumberAskMeasuring[0] = NumberAskMeasuring;
				eWrite = 1;
				eAddressWrite = 5;
				eMassiveWrite = eNumberAskMeasuring;
			}
			break;
			
			case '#':		// Сброс индикации команд
			DopComTrans = 2;
			break;
			
			case 'E':		// Перевод приемника из режима Готов в Введен
			DopComTrans=1; 
			break;
		}
	}
	else if (MenuLevel == LVL_MENU)
	{
		switch(PressKey)
		{
			case 'M':
			case 'C':		// Возврат в начальное меню
			{
				LCDbufClear();
				LCDtimerNew = 1;
				MenuLevel = LVL_START;
				ShiftMenu = 0;
				MaxDisplayLine = 0;
				MaxShiftMenu = 1;
			}break;
			
			case 'U':		// Листание списка вверх
			{
				if (ShiftMenu > 0) 
				{
					LCDbufClear(); 
					ShiftMenu--; 
					LCD2new = 1; 				
					NumberCom = 1;
				}
			}break;
			
			case 'D':		// Листание списка вниз
			{
				if (ShiftMenu < MaxShiftMenu) 
				{
					LCDbufClear();
					ShiftMenu++; 
					LCD2new = 1; 
					NumberCom = 1;
				}
			}break;
			
			case '1':		// Переход в пункт Журнал
			{
				LCDbufClear();
				LCD2new = 1;
				MenuLevel = LVL_JRN;
				ShiftMenu = 0;
				MaxShiftMenu = 1;
				MaxDisplayLine = 3;
			}break;
			
			case '2':		// Переход в пункт "Дата/время"
			{
				LCDbufClear();
				LCD2new = 1;
				MenuLevel = LVL_DATA_TIME;
			}break;
			
			case '3':
			{
				LCDbufClear();
				LCD2new = 1;
				MenuLevel = LVL_UPR;
				ShiftMenu = 0;
				MaxShiftMenu = 0;
				MaxDisplayLine = 3;
			}break;
			
			case '4':		// Переход в пункт "Установить"
			{
				LCDbufClear();
				LCD2new = 1;
				MenuLevel = LVL_SETUP;
				ShiftMenu = 0;
				MaxDisplayLine = 3;
				MaxShiftMenu=1;
				if ( (cNumComR > 0) && (CurrentState[2] < 0x04) )				//если есть приемник, и он не в Тест1 
					MaxShiftMenu=0;   	
				if ( (cNumComT > 0)&&(CurrentState[4] < 0x04) ) 				//если есть передатчик, и он не в Тест1
					MaxShiftMenu=0;  	
				if ( (bDef) && (CurrentState[0] < 0x04) ) 						// если есть защита, и не в Тест1
					MaxShiftMenu=0;			
			}break;
			
			case '5':		// Переход в "Просмотр параметров"
			{  
				LCDbufClear();
				LCD2new = 1;
				MenuLevel = LVL_VIEW_PARAM;
				ShiftMenu = 0;
				MaxShiftMenu = 1;
				MaxDisplayLine = 3;
			}break;
			
			case '6':		// Переход в "Информация"
			{
				LCDbufClear();
				LCD2new = 1;
				MenuLevel = LVL_INFO;
				ShiftMenu = 0;
				MaxShiftMenu = 4;
				MaxDisplayLine = 1;
				
			}break;
		}
	}
	else if (MenuLevel == LVL_DATA_TIME)
	{
		switch(PressKey)
		{
			case 'M':		// Возврат в начальное меню
			{
				LCDbufClear();
				LCDtimerNew = 1;
				MenuLevel = LVL_START;
				ShiftMenu = 0;
				MaxDisplayLine = 0;
				MaxShiftMenu = 1;
			}break;
			
			case 'C':		// Переход на уровень выше
			{
				LCDbufClear();
				MenuLevel = LVL_MENU;
				LCD2new = 1;
				ShiftMenu = 0;
				MaxShiftMenu = 1;
				MaxDisplayLine = 3;
			}break;
			
			case '1':		// Ввод даты
			{
				WorkRate = 0x03;
				ChangeMass = DataLCD;
				InputParameter = 1;
				Discret = 1;  
				for (char i = 0; i <= 8; i++) 
					InputDataTime[i] = DataLCD[i];
			}break;
			
			case '2':		// Ввод времени
			{  
				WorkRate = 0x03;
				ChangeMass = TimeLCD;
				InputParameter = 2;
				Discret = 1;  
				for (char i = 0; i <= 8; i++) 
					InputDataTime[i] = TimeLCD[i];
			}break;
		}
	}
	else if (MenuLevel == LVL_JRN)
	{
		switch(PressKey)
		{
			case 'M':		// Возврат в начальное меню
			{
				LCDbufClear();
				LCDtimerNew = 1;
				MenuLevel = 1;
				ShiftMenu = 0;
				MaxDisplayLine = 0;
				MaxShiftMenu = 1;
			}break;
			
			case 'C':		// Переход на уровень выше
			{
				LCDbufClear();
				MenuLevel = LVL_MENU;
				LCD2new = 1;
				ShiftMenu = 0;
				MaxShiftMenu = 1;
				MaxDisplayLine = 3;
			}break;
			
			case 'U':		// Листание списка вверх
			{
				if (ShiftMenu > 0) 
				{
					LCDbufClear(); 
					ShiftMenu--; 
					LCD2new = 1; 				
					NumberCom = 1;
				}
			}break;
			
			case 'D':		// Листание списка вниз
			{
				if (ShiftMenu < MaxShiftMenu) 
				{
					LCDbufClear();
					ShiftMenu++; 
					LCD2new = 1; 
					NumberCom = 1;
				}
			}break;
			
			case '1':		// переход в подпункты журнала
			case '2':
			case '3':
			case '4':
			PressInMenuJournal('1');  
			break;
		}
	}
	else if (MenuLevel == LVL_SETUP)
	{
		switch(PressKey)
		{
			case 'M':		// Возврат в начальное меню
			{
				LCDbufClear();
				LCDtimerNew = 1;
				MenuLevel = 1;
				ShiftMenu = 0;
				MaxDisplayLine = 0;
				MaxShiftMenu = 1;
			}break;
			
			case 'C':		// Переход на уровень выше
			{
				LCDbufClear();
				MenuLevel = LVL_MENU;
				LCD2new = 1;
				ShiftMenu = 0;
				MaxShiftMenu = 1;
				MaxDisplayLine = 3;
			}break;
			
			case 'U':		// Листание списка вверх
			{
				if (ShiftMenu > 0) 
				{
					LCDbufClear(); 
					ShiftMenu--; 
					LCD2new = 1; 				
					NumberCom = 1;
				}
			}break;
			
			case 'D':		// Листание списка вниз
			{
				if (ShiftMenu < MaxShiftMenu) 
				{
					LCDbufClear();
					ShiftMenu++; 
					LCD2new = 1; 
					NumberCom = 1;
				}
			}break;
			
			case '1':		// Переход в пункт "Меню / Установить / Режим"
			{
				LCDbufClear();
				LCD2new = 1;
				MenuLevel = LVL_SETUP_RGM;
				ShiftMenu = 0;
				MaxShiftMenu = 0;
				MaxDisplayLine = 1;
			}break;  
						
			case '2':		// Переход в пункт "Меню / Установить / Параметры"
			{
				LCDbufClear();
				LCD2new = 1;
				MenuLevel = LVL_SETUP_PARAM;
				ShiftMenu = 0;
				MaxShiftMenu = 1;
				MaxDisplayLine = 3;
			}break;
			
			case '3':		// Переход в пункт "Меню / Установить / Пароль"
			{
				if (PressPassword == 2)
				{
					WorkRate = 0x01;
					MaxNumberInputChar = 4;
					ChangeMass = ePassword;
					ByteShift = 0;
					MaxValue = 0;
					MinValue = 0;
					Discret = 1;
				}
				else if (PressPassword == 1)
				{
					WorkRate = 0x01;
					MaxNumberInputChar = 4;
					ChangeMass = ePassword;
					ByteShift = 0;
					MaxValue = 0;
					MinValue = 0; 
					Discret = 1; 
				}
			}break;
			
			case '4': 		// Переход в пункт "Меню / Установить / Тест"
			{
				if (CurrentState[2]<0x04) break;   //если есть приемник, и он не в Тест
				if (CurrentState[4]<0x04) break;  //если есть передатчик, и он не в Тест
				if (CurrentState[0]<0x04) break;  //если есть Защита, и она не в Тест
				
				LCDbufClear();
				LCD2new = 1; 
				MenuLevel = LVL_TEST; 
				ShiftMenu = 0;
				MaxDisplayLine = 3;
				MaxShiftMenu = 1;
			}break;
		}
	}
	else if (MenuLevel == LVL_UPR)
	{
		switch(PressKey)
		{
			case 'M':		// Возврат в начальное меню
			{
				LCDbufClear();
				LCDtimerNew = 1;
				MenuLevel = 1;
				ShiftMenu = 0;
				MaxDisplayLine = 0;
				MaxShiftMenu = 1;
			}break;
			
			case 'C':		// Переход на уровень выше
			{
				LCDbufClear();
				MenuLevel = LVL_MENU;
				LCD2new = 1;
				ShiftMenu = 0;
				MaxShiftMenu = 1;
				MaxDisplayLine = 3;
			}break;
		}
	}
	else
	{
		switch(PressKey)
		{
			case '1':
			{
				switch(MenuLevel){
					case 6:
					case 12: // переход в установить\параметры\пост
					{
						LCDbufClear();
						LCD2new = 1;
						MenuLevel += 1;
						ShiftMenu = 0;
						MaxShiftMenu = 6;
						MaxDisplayLine = 1; 
						ValueVsRange = false;
					} 
					break;  
				}
			}break;
			case '2':
			{
				switch(MenuLevel)
				{
					case 6:	//усстановить\параметры\приемника
					case 12:	//просмотр парам\приемник	
					{
						LCDbufClear();LCD2new=1;ShiftMenu=0;MaxDisplayLine=1;NumberCom=1;
						MaxShiftMenu = defNumParamPrm - 1;
						MenuLevel += 2;	// 8 and 14
					}
					break;
				}
			}break;
			case '3':
			{
				switch(MenuLevel){
					case 6:	//усстановить\параметры\передатчика
					case 12:	//просмотр парам\передатчика	
					{
						LCDbufClear();LCD2new=1;ShiftMenu=0;MaxDisplayLine=1;NumberCom=1;
						MaxShiftMenu = defNumParamPrd - 1;
						MenuLevel += 3;	// 9 and 15
					}
					break;
				}
			}break;
			case '4':
			{
				switch(MenuLevel)
				{	
					case 6:
					case 12:	//переход в Просмотр параметров(установить\параметры)\общие
					{ 
						LCDbufClear();
						LCD2new=1;
						ShiftMenu=0;
						MaxDisplayLine=1;
						MenuLevel += 4;
						MaxShiftMenu = defNumParamAll -1;
					}break;
					
					case 8:	
					case 14:	// .*утсановить\параметры\применик
					{
						if (ShiftMenu == 1)
						{
							NumberCom = (NumberCom > 1) ? NumberCom - 1 : 4;
						}
					}
					break;
				}
			}break;
			case '6':
			{
				switch(MenuLevel)
				{
					case 8:	
					case 14:	// *установить\параметры\применик
					{
						if (ShiftMenu == 1)
						{
							NumberCom = (NumberCom < 4) ? NumberCom + 1 : 1;
						}
					}
					break;
				}
			}break;
			case 'M':
			{
				LCDbufClear();
				LCDtimerNew = 1;
				MenuLevel = 1;
				ShiftMenu = 0;
				MaxDisplayLine = 0;
				MaxShiftMenu = 1;
			}break;
			case 'C':
			{
				switch (MenuLevel){
					case 6:
					case 19: {LCDbufClear();MenuLevel=2;LCD2new=1;ShiftMenu=0;MaxShiftMenu=1;MaxDisplayLine=3;} break;  //возврат в меню 2-ого уровня
					case 7:
					case 8:
					case 9:
					case 10:{ //возврат в Просмотр параметров
						LCDbufClear();MenuLevel=6;LCD2new=1;ShiftMenu=0;NumberCom=1;MaxShiftMenu=1;MaxDisplayLine=3;
					}break;
					case 20:
					case 11:
					case 12:  {LCDbufClear();MenuLevel=5;LCD2new=1;ShiftMenu=0; MaxDisplayLine=3;
					MaxShiftMenu=1;
					if (CurrentState[2]<0x04) MaxShiftMenu=0;   //если есть приемник, и он не в Тест1
					if (CurrentState[4]<0x04) MaxShiftMenu=0;  //если есть передатчик, и он не в Тест1
					if (CurrentState[0]<0x04) MaxShiftMenu=0;
					} break; //Возврат в Установить
					case 13:
					case 14:
					case 15:
					case 16:{ //Возврат в Устанновить\Параметры
						LCDbufClear();LCD2new=1;MenuLevel=12;ShiftMenu=0;ValueVsRange=0;NumberCom=1;MaxShiftMenu=1;MaxDisplayLine=3;ValueVsRange=false;
					}break;
					case 21: Menu_Journal(); break; //возврат в меню Журнал
				}
			}break;
			case 'U':
			{
				switch(MenuLevel){
					case 13:
					case 16:  ValueVsRange=0;
					
					case 6:
					case 7:
					case 8:
					case 9:
					case 10:
					case 12:
					case 14:
					case 15:
					case 19:
					case 20:{
						if (ShiftMenu>0) {ShiftMenu--; LCD2new=1; LCDbufClear(); NumberCom=1;}
					} break; //прокрутка меню
					case 21:{
						if (sArchive.RecCount>1){
							if (ShiftMenu>0) ShiftMenu--;
							else ShiftMenu=sArchive.RecCount-1;
							sArchive.Data[12]=0;
							FuncClearCharLCD(2,1,40); //очистка инф-ы
							FuncClearCharLCD(4,13,8); //очистка даты записи
						}
					}break;
					
				}
			}break;
			case 'D':
			{
				switch(MenuLevel){
					case 13:
					case 16: ValueVsRange =0;
					
					case 6:
					case 7:
					case 8:
					case 9:
					case 10:
					case 12:
					case 14:
					case 15:
					case 19:
					case 20:{
						if (ShiftMenu<MaxShiftMenu) {ShiftMenu++; LCD2new=1; LCDbufClear(); NumberCom=1;}
					} break; //прокрутка меню
					case 21:{
						if (sArchive.RecCount>1){
							if (ShiftMenu<(sArchive.RecCount-1)) ShiftMenu++;
							else ShiftMenu=0;
							sArchive.Data[12]=0;
							FuncClearCharLCD(2,1,40); //очистка инф-ы
							FuncClearCharLCD(4,13,8); //очистка даты записи
						}
					}break;
				}
			}break;
			case '#':
			{
				if ((MenuLevel>12)&&(MenuLevel<17)) {ValueVsRange++; if (ValueVsRange>1) ValueVsRange=0; LCD2new=1;}
			} break;
			case 'E':
			{
				switch(MenuLevel)
				{
					case 11:
					{  //ввод режимов работы
						if ( (PressPassword == 2) && (EntPass == 0) )
						{
							WorkRate=0x01;MaxNumberInputChar=4;ChangeMass=ePassword;
							ByteShift=0;MaxValue=0;MinValue=0;Discret=1;EntPass=1;
						}
						else
						{
							if (PressPassword == 1)
							{
								bInpVal=false;
								if ((CurrentState[0]==2)||(CurrentState[0]==0x4E)) bInpVal=true;
								if ((CurrentState[2]==2)||(CurrentState[2]==1)||(CurrentState[2]==0x4E)) bInpVal=true;
								if ((CurrentState[4]==2)||(CurrentState[4]==0x4E)) bInpVal=true;
								WorkRate=2;SelectValue=1;InputSelectValue=0;MinSelectValue=0;PressPassword=2;
								if (bInpVal)
								{
									MaxSelectValue=1;MassSelectValue=Menu1regimeInp;
								}
								else
								{
									MaxSelectValue=3;MassSelectValue=Menu1regimeInp2;
								}
								EntPass=0;
							}
							else
							{
								bInpVal=false;
								if ((CurrentState[0]!=2)&&(CurrentState[0]!=0x4E)) bInpVal=true;
								if ((CurrentState[2]!=2)&&(CurrentState[2]!=1)&&(CurrentState[2]!=0x4E)) bInpVal=true;
								if ((CurrentState[4]!=2)&&(CurrentState[4]!=0x4E)) bInpVal=true;
								
								if (bInpVal){
									WorkRate=2;
									SelectValue=2;
									InputSelectValue=0;
									MinSelectValue=0;
									MassSelectValue=Menu1regimeInp1;
									PressPassword=2;
									MaxSelectValue=2;
								}
								EntPass=0;
							}
						}
					}break;
					case 13:
					{  	//ввод параметров в меню Установить\Параметры\Пост
						if (CurrentState[0]==0x00){
							switch(ShiftMenu)
							{
								case 0: {WorkRate=2;SelectValue=1;InputSelectValue=0;MaxSelectValue=7;MinSelectValue=0;MassSelectValue=ParamTypeDef;} break; 	
								case 1: {WorkRate=2;SelectValue=2;InputSelectValue=0;MaxSelectValue=1;MinSelectValue=0;MassSelectValue=ParamLineDef;} break; 	
								case 2: {WorkRate=1;MaxNumberInputChar=2;ByteShift=0;MaxValue=99;MinValue=0;InputParameter=8;Discret=1;NumberTransCom=1;}break;
								case 3: {WorkRate=1;MaxNumberInputChar=2;ByteShift=0;MaxValue=18;MinValue=0;InputParameter=9;Discret=1;NumberTransCom=1;} break;
								case 4: {WorkRate=1;MaxNumberInputChar=2;ByteShift=0;MaxValue=54;MinValue=0;InputParameter=10;Discret=2;NumberTransCom=1;}break;
								case 5: {WorkRate=1;MaxNumberInputChar=2;ByteShift=0;MaxValue=20;MinValue=0;InputParameter=27;Discret=1;NumberTransCom=1;} break;
								case 6: {WorkRate=1;MaxNumberInputChar=2;ByteShift=0;MaxValue=20;MinValue=0;InputParameter=28;Discret=1;NumberTransCom=1;} break;
							}
						}
					}break;
					case 14:	//e утсановить\параметры\приемник
					{
						if (CurrentState[0] == 0x00)
						{
							switch(ShiftMenu)
							{
								case 0: {WorkRate=0x01;MaxNumberInputChar=2;ByteShift=0;MaxValue=10;MinValue=0;InputParameter=0x91;Discret=1;NumberTransCom=1;} break;
								case 1: {WorkRate=0x01;MaxNumberInputChar=4;ByteShift=0;MaxValue=1000;MinValue=0;InputParameter=0x93;Discret=50;NumberTransCom=NumberCom;} break;
								case 2: {WorkRate=0x01;MaxNumberInputChar=4;ByteShift=0;MaxValue=0;MinValue=0;InputParameter=0x94;Discret=1;NumberTransCom=1;} break;
							}
						}
					}
					break;
					case 15:	//e утсановить\параметры\передатчик
					{
						if (CurrentState[0] == 0x00)
						{
							switch(ShiftMenu)
							{
								case 0: {WorkRate=0x01;MaxNumberInputChar=2;ByteShift=0;MaxValue=10;MinValue=0;InputParameter=0xA1;Discret=1;NumberTransCom=1;} break;
								case 1: {WorkRate=0x01;MaxNumberInputChar=3;ByteShift=0;MaxValue=100;MinValue=20;InputParameter=0xA2;Discret=1;NumberTransCom=1;} break;
								case 2: {WorkRate=0x01;MaxNumberInputChar=4;ByteShift=0;MaxValue=0;MinValue=0;InputParameter=0xA5;Discret=1;NumberTransCom=1;} break;
								case 3: {WorkRate=0x01;MaxNumberInputChar=4;ByteShift=0;MaxValue=0;MinValue=0;InputParameter=0xA4;Discret=1;NumberTransCom=1;} break;
							}
						}
					}
					break;
					case 16:
					{ //меню/установить/параметры/общие
						bInpVal=true; //разрешим изменение параметра
						if ((bDef)&&(CurrentState[0]!=0x00)) bInpVal=false;
						if ((cNumComR>0)&&(CurrentState[2]!=0x00)) bInpVal=false;
						if ((cNumComT>0)&&(CurrentState[4]!=0x00)) bInpVal=false;
						if (bInpVal){
							switch(ShiftMenu){
								case 0: {WorkRate=2;SelectValue=1;InputSelectValue=0;MaxSelectValue=1;MinSelectValue=0;MassSelectValue=MenuAllSynchrTimerNum;} break;
								case 1: {WorkRate=2;SelectValue=2;InputSelectValue=0;MaxSelectValue=1;MinSelectValue=0;MassSelectValue=MenuAllSynchrTimerNum;} break;
								case 2: {WorkRate=1;MaxNumberInputChar=1;ByteShift=0;MaxValue=5;MinValue=0;InputParameter=26;Discret=1;} break;
								case 3: {WorkRate=2;SelectValue=8;InputSelectValue=0;MaxSelectValue=1;MinSelectValue=0;MassSelectValue=MenuAllSynchrTimerNum;} break;
								case 4: {WorkRate=1;MaxNumberInputChar=2;ByteShift=0;MaxValue=22;MinValue=0;InputParameter=5;Discret=1;} break;
								case 5: {WorkRate=1;MaxNumberInputChar=2;ByteShift=0;MaxValue=22;MinValue=0;InputParameter=6;Discret=1;}break;
								case 6: {WorkRate=1;MaxNumberInputChar=2;ByteShift=0;MaxValue=22;MinValue=0;InputParameter=7;Discret=1;}break;
							}
						}bInpVal=false;
					}break;
					case 19: 
					{
						if (ShiftMenu==4)
						{
							WorkRate = 2; 
							SelectValue = 3; 
							InputSelectValue = 0; 
							MaxSelectValue = 1;
							MinSelectValue = 0; 
							MinValue = 0; 
							MaxValue = 1; 
							MassSelectValue = fDopParamValue;
						}
					}break;
					case 20: 
					{
						//в тесте можно подавать сигналы только если Пост и Передатчик (или если присутствует только 1 из них) в режиме Тест
						if (CurrentState[4]<0x04) break;
						if (CurrentState[0]<0x04) break;
						
						WorkRate=2;SelectValue=ShiftMenu+1;InputSelectValue=0;
						if (ShiftMenu==0) {MaxSelectValue = 6;MinSelectValue=0;MassSelectValue=Menu20gr1;}
						if (ShiftMenu==1) {MaxSelectValue = 2;MinSelectValue=0;MassSelectValue=Menu20gr2;}
					}break;
				}
			}break;
			default: { _NOP();};
		}
	}
	
	PressKey=0xF0;
}

//опрос параметров для заполнения массива ModBus
void ModBusOpros(void)
{
	PosModBusInfill++;
	if(PosModBusInfill<0x10){ //Общие параметры
        switch(PosModBusInfill){
		case 1: TransDataInf(0x36, 0x00); break;
		case 2: TransDataInf(0x37, 0x00); break;
		case 3: TransDataInf(0x39, 0x00); break;
		case 4: TransDataInf(0x3A, 0x00); break;
		case 5: TransDataInf(0x3B, 0x00); break;
		case 6: TransDataInf(0x3C, 0x00); break;
		case 7: TransDataInf(0x3D, 0x00); break;
		case 8: TransDataInf(0xF1, 0x00); break;
		default: PosModBusInfill=0x10;
        }
	}
	if ((PosModBusInfill>=0x10)&&(PosModBusInfill<0x20)){  //параметры УМ
		switch(PosModBusInfill){
		case 0x10: TransDataByte(0x34, 0x00); break;
		case 0x11: TransDataInf(0x3B, 0x00); break;
		default: PosModBusInfill=0x20;
		}
	}
	
	if ((PosModBusInfill>=0x20)&&(PosModBusInfill<0x30)){  //параметры Защиты
		switch(PosModBusInfill){
		case 0x20: TransDataInf(0x04, 0x00); break;
		case 0x21: TransDataInf(0x06, 0x00); break;
		case 0x22: TransDataInf(0x09, 0x00); break;
		case 0x23: TransDataInf(0xC1, 0x00); break;
		case 0x24: TransDataInf(0x01, 0x00); break;
		default: PosModBusInfill=0x30;
		}
	}
	
	if (PosModBusInfill>=0x30){   //параметры прм/прд
        switch(PosModBusInfill){
		case 0x30: TransDataInf(0x11, 0x00); break;
		case 0x31: TransDataInf(0x12, 0x00); break;
		case 0x32: TransDataInf(0x13, 0x00); break;
		case 0x33: TransDataInf(0x14, 0x00); break;
		case 0x34: TransDataInf(0x21, 0x00); break;
		case 0x35: TransDataInf(0x22, 0x00); break;
		case 0x36: TransDataInf(0x24, 0x00); break;
		case 0x37: TransDataInf(0x25, 0x00); break;
		case 0x38: TransDataInf(0x26, 0x00); break;
		case 0x39: TransDataInf(0xD1, 0x00); break;
		case 0x3A: TransDataInf(0xE1, 0x00); break;
		default: PosModBusInfill=0;
		}
	}
}

#pragma vector=TIMER1_OVF_vect
__interrupt void Timer1ovf(void)
{
	_SEI();
	TCNT1H=0xF9;  //
	TCNT1L=0xF0;  //было E5 установка 0.1с при делителе 1024
	
	if (TestDelay>0) {TestDelay--; PressKey=0xF0;}
	
	//работа с ПК
	if (PCready==1) {StartTrans(PCbyte);PCready=2;}
	if (PCready==3) {StartTrans1(PCbyte);PCready=4;}
	PCtime--;
	if (PCtime==0){ if (PCready!=0) {PCready=0; EnableReceive1; FuncClearCharLCD(2,4,14);} PCtime=PC_wait;}
	
	LoopUART++; //говорим что прошло еще 100мс
	//прошла 1с
	if (LoopUART>=LoopUARTtime){
		//раз в секунду мы посылаем запрос общего текущего состояния в MenuLevel=1,  запрос параметра или отправляем его значение
		LoopUART=0; //начинаем счет 1с сначала
		if (TimeInitLCD>=ConstTimeInitLCD) {FuncInitLCD();TimeInitLCD=0;}  //инициализация ЖК-индикатора
		else TimeInitLCD++;
	}
	//проверяем прошла ли у нас 1сек
	if (LoopUART<LoopUARTtime)
    {
		if (PCready==0){  //связь с БСП
			if (bReadVers){ //если версия аппарата считана
				if (LoopUART==1)  //опрашиваем примерно раз в 0.5 сек
				{
					if (RecivVar==0) NumberLostLetter++; //если предыдущее сообщение не было получено, добавляем 1 к потеряным посылкам
					else {NumberLostLetter=0;RecivVar=0;} //иначе пишем в потерянные посылки 0 и обнуляем переменную полученных данных
					//если было введено новое значение дата/время с клавиатуры, отправим его в USP, иначе опросим USP
					if (NewTime==1)
					{
						for (uchar i = 0; i < 6; i++) 
							Tr_buf_data_uart[4 + i] = TrDataTimeMass[i];
						TransDataInf(0xB2, 6); //посылаем введеное время/дату
						NewTime = 0;
					}
					else
					{
						TransDataInf(0x32,0x00);
					}
				}
				if (LoopUART==2)  //опрашиваем примерно раз в 0.5 сек
				{
					if(RecivVar==0) NumberLostLetter++; //если предыдущее сообщение не было получено, добавляем 1 к потеряным посылкам
					else {NumberLostLetter=0;RecivVar=0;} //иначе пишем в потерянные посылки 0 и обнуляем переменную полученных данных
					TransDataInf(0x30, 0x00); //посылаем команду запроса текущего состояния
				}
				if ((LoopUART==3)||(LoopUART==7))  //опрашиваем примерно раз в 0.5 сек
				{
					if(RecivVar==0) NumberLostLetter++; //если предыдущее сообщение не было получено, добавляем 1 к потеряным посылкам
					else {NumberLostLetter=0;RecivVar=0;} //иначе пишем в потерянные посылки 0 и обнуляем переменную полученных данных
					//сделаем запрос параметра, выводимого на экран
					
					switch (DopComTrans){
					case 1: {TransDataInf(0x51,0x00);DopComTrans=0;} break; //если нажали на кнопку ПУСК Приемника
					case 2: {TransDataInf(0x9F,0x00);DopComTrans=3;} break; //сброс индикации ПРМ
					case 3: {TransDataInf(0xAA,0x00);DopComTrans=0;} break; //сброс индикации ПРД
					default:  TransDataByte(0x34, 0x00); //всегда опрашиваем все измеряемые параметры
					}
				}
				if (LoopUART==4)  //опрашиваем примерно раз в 0.5 сек
				{
					if(RecivVar==0) NumberLostLetter++; //если предыдущее сообщение не было получено, добавляем 1 к потеряным посылкам
					else {NumberLostLetter=0;RecivVar=0;} //иначе пишем в потерянные посылки 0 и обнуляем переменную полученных данных
					
					if (TrParam==0)
					{
						switch(MenuLevel)
						{
							case LVL_VIEW_DEF:
							case LVL_SETUP_DEF: 
							{
								if (ShiftMenu < 6)
									TransDataInf(ShiftMenu + 1, 0x00);
								else
								{
									TransDataInf(0x09,0x00);
								}
							}break; 
							case LVL_VIEW_PRM:		
							case LVL_SETUP_PRM:	
							{
								switch(ShiftMenu)
								{
									case 0:
									TransDataInf(0x11, 0x00);
									break;
									case 1:
									TransDataInf(0x13, 0x00);
									break;
									case 2:
									TransDataInf(0x14, 0x00);
									break;
								}
								
							}break;	
							case LVL_VIEW_PRD:	
							case LVL_SETUP_PRD:
							{
								switch(ShiftMenu)
								{
									case 0:
									case 1: TransDataInf(0x21 + ShiftMenu, 0x00); break;	
									case 2: TransDataInf(0x25, 0x00); break;
									case 3: TransDataInf(0x24, 0x00); break;	
									case 4: TransDataInf(0x26, 0x00); break;
								}
							}
							break;
							case LVL_VIEW_GLB:
							case LVL_SETUP_GLB:{//общие параметры
								switch(ShiftMenu){
									case 0:
									case 1: TransDataInf(0x36+ShiftMenu,0x00); break;
									case 2: TransDataInf(0x39,0x00); break;
									case 3: TransDataInf(0x3D,0x00); break;
									case 4:
									case 5:
									case 6: TransDataInf(0x3C,0x00); break;
								}
							}break;
							case LVL_INFO: TransDataByte(0x34, 0x00); break;  //спросим еще раз параметры
							case LVL_TEST: { //запрос параметров в меню Тест
								if ((CurrentState[2]>3)||(CurrentState[2]>3)||(CurrentState[4]>3)) TransDataByte(0x3E,0x00);
							} break;
							case LVL_JRN_VIEW:{  //если находимся в подпунктах Журнала
								if (sArchive.RecCount>0){
									if (ShiftMenu>sArchive.RecCount){ //в случае если указатель превышает кол-во записей массива
										sArchive.Data[12]=0;
										ShiftMenu=0;
									}
									Tr_buf_data_uart[5]=0;
									Tr_buf_data_uart[4]=sArchive.CurCount + ShiftMenu;
									TransDataInf(0xF2-(sArchive.Dev[sArchive.CurrDev]<<4), 0x02);
								}else
									TransDataInf(0xF1 - (sArchive.Dev[sArchive.CurrDev] << 4), 0x00);
							}break;
							
							default: {TransDataInf(0x31, 0x00);} //посылаем запрос общего текущего состояния
						}
					}
					else{
						switch(MenuLevel)
						{
							case LVL_SETUP_RGM:
							{
								switch(TrValue)
								{
									case 0:	//вывести 
									TransDataInf(0x70,0x00);
									break; 
									case 1: //ввести
									TransDataInf(0x71,0x00);
									break; 
									case 2: //тест 1
									Tr_buf_data_uart[4] = 0x00; 
									Tr_buf_data_uart[5] = 0x00; 
									TransDataInf(0x7E, 0x02); 
									break; 
									case 3:	//тест 2
									TransDataInf(0x7D,0x00);
									break;  
								}
							}break;
							case LVL_SETUP_DEF: 
							{	//установить/параметры/защита
								if (TrParam == 0x09)
								{
									TransDataByte(0x89, TrValue);
								}
								else
								{
									Tr_buf_data_uart[4] = TrValue;
									Tr_buf_data_uart[5] = NumberTransCom;
									TransDataInf(0x80 + TrParam, 2);
									NumberTransCom = 0;
								}
							}break;
							case LVL_SETUP_PRM:	//установить/парметры/прм
							{
								if (TrParam > 0x92)
								{
									Tr_buf_data_uart[4] = NumberTransCom;
									Tr_buf_data_uart[5] = TrValue;
									TransDataInf(TrParam , 2);
								}
								else
									TransDataByte(TrParam , TrValue);				
							}
							break;
							case LVL_SETUP_PRD:	//установить/параметры/прд
							{
								if ( (TrParam < 0xA4) || (TrParam == 0xA6) )
									TransDataByte(TrParam, TrValue);
								else
								{
									Tr_buf_data_uart[4] = NumberTransCom;
									Tr_buf_data_uart[5] = TrValue;
									TransDataInf(TrParam , 2);
								}				
							}
							break;
							case LVL_SETUP_GLB:
							{
								switch(TrParam){
									case 5:
									case 6:
									case 7:{
										Tr_buf_data_uart[4]=TrParam-4;
										Tr_buf_data_uart[5]=TrValue;
										TransDataInf(0xBC,2);
									}break;			
									default: TransDataByte(0xB5 + TrParam, TrValue);
								}
							}break;
							case LVL_TEST:
							{
								Tr_buf_data_uart[4] = TrParam;
								Tr_buf_data_uart[5] = TrValue;
								TransDataInf(0x7E,2);
								Tr_buf_data_uart1[4] = TrParam;
								Tr_buf_data_uart1[5] = TrValue;
								TransDataInf1(0x7E,2);
							}break;
						}
						TrValue=0;
						TrParam=0;
					}
				}
				//запросы для заполнения массива ModBus, получается что он полностью обновляется за 7 секунд.
				if ((LoopUART==9)||(LoopUART==5)||(LoopUART==0)||(LoopUART==8)||(LoopUART==6))
				{
					if (RecivVar == 0) NumberLostLetter++; //если предыдущее сообщение не было получено, добавляем 1 к потеряным посылкам
					else {NumberLostLetter=0;RecivVar=0;} //иначе пишем в потерянные посылки 0 и обнуляем переменную полученных данных
					
					ModBusOpros();
				}
			}else
			{  //если версия аппарата еще не считана
				if(RecivVar==0) NumberLostLetter++; //если предыдущее сообщение не было получено, добавляем 1 к потеряным посылкам
				else {NumberLostLetter=0;RecivVar=0;} //иначе пишем в потерянные посылки 0 и обнуляем переменную полученных данных
				
				TransDataInf(0x3F,0);
			}
		}//конец связи с БСП
		
		if (NumberLostLetter>5) NumberLostLetter=5; //если у нас уже больше 5 потерянных посылок, то приравняем их к 5
	}
	
	//это делаетс каждые 0.1с
	//теперь смотрим, надо ли нам обновить информацию на экране LCD
	bLCDwork=true;
	
	// вызываемм обработчик нажатой клавишы, примерно раз в 0.3с
	if (bReadVers){
		if (((LoopUART==0)||(LoopUART==3)||(LoopUART==6))&&(PCready==0)&&(TestDelay==0))
			if (WorkRate==0) FuncPressKey();
			else
				if (WorkRate==1) FuncInputData();
				else
					if (WorkRate==3) FuncInputDataTime();
					else FuncSelecValueInputParam();
	}
	
	//для мигания надписей, неисправность/предупреждение
	if (LoopUART==5) TimeWink=!TimeWink;
}

//вывод на экран неисправностеЙ, предупреждений, состояний в первом меню
void LCDMenu1(unsigned char NumString,unsigned char Device){
	//Device: 1-защита, 2 - ПРМ1, 3 - ПРД, 5 - ПРМ2
	unsigned int tglobal, temp;
	unsigned char i;
	unsigned char __flash* Title;
	unsigned char __flash* __flash* MassError;
	unsigned char __flash* __flash* MassStat;
	bool DevAvar, DevWarn;
	
	if (Device==1){
		Title=Menu1Def;
		MassError=Menu1PostErrorT;
		MassStat=Menu1condPOST;
		DevAvar=bDefAvar;
		DevWarn=bDefWarn;
	}else
		if ((Device==2)||(Device==5)){  //приемники
			if (Device==2){  //приемник 1 в 3-х концевой, или просто приемник в 2-х
				Title = Menu1Rec;
				MassError=Menu1PrmErrorT;
				MassStat=Menu1condPRM;
				DevAvar=bRec1Avar;
				DevWarn=bRec1Warn;
			}else{  //приемник 2 в 3-х концевой
				Title = Menu1Rec2;
				MassError=Menu1PrmErrorT;
				MassStat=Menu1condPRM;
				DevAvar=bRec2Avar;
				DevWarn=bRec2Warn;
			}
		}else
			if (Device==3){
				Title=Menu1Tran;
				MassError=Menu1PrdErrorT;
				MassStat=Menu1condPRD;
				DevAvar=bTrAvar;
				DevWarn=bTrWarn;
			}
	
	LCDprintf(NumString,1,2,Title,1);
	if (bGlobalAvar){ //общая авария
		if (!TimeWink){ //режим мигания, раз в секунду
			LCDprintf(NumString,5,2,Menu1disrepair,1);
			FuncClearCharLCD(NumString,13,8);
			LCDprintf(NumString,13,2,GlobalAvar,0);
			LCDprintHEX(NumString,15,GlobalCurrentState[12]);
			LCDprintHEX(NumString,17,GlobalCurrentState[13]);
		}else{
			tglobal = (GlobalCurrentState[12]<<8) + (GlobalCurrentState[13]);
			for(i=0, temp=1; i<15; i++, temp=temp<<1)
				if (tglobal&temp) LCDprintf(NumString,5,2,Menu1GlobalErrorT[i],1);
			//          if (tglobal&temp) LCDprintf(5,5,2,Menu1GlobalErrorT[i],1);
		}
	}else
		if (DevAvar){  //если авария устройства
			if (!TimeWink){ //режим мигания, раз в секунду
				LCDprintf(NumString,5,2,Menu1disrepair,1);
				FuncClearCharLCD(NumString,13,8);
				LCDprintf(NumString,13,2,LocalAvar,0);
				LCDprintHEX(NumString,15,GlobalCurrentState[(Device-1)*4]);
				LCDprintHEX(NumString,17,GlobalCurrentState[(Device-1)*4 + 1]);
			}else{
				tglobal = (GlobalCurrentState[(Device-1)*4]<<8) + GlobalCurrentState[(Device-1)*4 + 1];
				for(i=0, temp=1; i<15; i++, temp*=2)
					if (tglobal&temp) {LCDprintf(NumString,5,2,MassError[i],1); i=16;}
			}
		}else{  //если неисправностей нет
			if ((TimeWink)&&((bGlobalWarn)||(DevWarn))){ //если есть предупреждение и надо его вывести
				if (bGlobalWarn){ //общее предупреждение
					if ((GlobalCurrentState[14]==0x04)&&(GlobalCurrentState[15]==0x00)){
						LCDprintf(NumString,5,2,Menu1GlobalWarning1,1);
					}else{
						LCDprintf(NumString,5,2,Menu1warning,1);
						FuncClearCharLCD(NumString,13,8);
						LCDprintf(NumString,13,2,GlobalAvar,0);
						LCDprintHEX(NumString,15,GlobalCurrentState[14]);
						LCDprintHEX(NumString,17,GlobalCurrentState[15]);
					}
				}else{//предупреждение устройства
					temp=(int) (GlobalCurrentState[2+(Device-1)*4]<<8) + GlobalCurrentState[3+(Device-1)*4];  //предупреждениz
					if ((Device==1)&&(temp==1)) LCDprintf(NumString, 5, 2, Menu1PostWarning1,1);  //1 предупреждение поста
					else
						if ((Device==2)||(Device==5)&&(temp==1)) LCDprintf(NumString, 5, 2, Menu1PrmWarning1,1);  //1 предупреждение прм
						else{ //если это не ЗАЩИТА(Приемник)педупреждение 0х0001, тьо выведем код
							LCDprintf(NumString,5,2,Menu1warning,1);
							FuncClearCharLCD(NumString,13,8);
							LCDprintf(NumString,13,2,LocalAvar,0);
							LCDprintHEX(NumString,15,GlobalCurrentState[(Device-1)*4+2]);
							LCDprintHEX(NumString,17,GlobalCurrentState[(Device-1)*4+3]);
						}
				}
			}else{  //если надо вывести статус
				if (Device==5) Device=4;  //в статусе отсутствует общие данные, потому для извлечения из массива уменьшим на 1
				if (CurrentState[(Device-1)*2]<=5) LCDprintf(NumString,5,2,Menu1regime[CurrentState[(Device-1)*2]],1); //если нам известен код режима, выводим сообщение на экран
				else LCDprintf(NumString,5,2,Menu1Err,0);  //если же нам не извесен принятый код, выводим "????"
				if (CurrentState[(Device-1)*2+1]<=0x0A) LCDprintf(NumString,13,2,MassStat[CurrentState[(Device-1)*2+1]],1);//выводим состояние устройства
				else LCDprintf(NumString,13,2,Menu1Err,0);  //если же нам не извесен принятый код, выводим "????"
				//вывод на экран дополнительного байта
				if((Device==2)||(Device==4)){
					if ((CurrentState[(Device-1)*2+1]==1)||(CurrentState[(Device-1)*2+1]==2)||(CurrentState[(Device-1)*2+1]==7))  //прием КЧ, команды или блок.команды
						LCDprintDEC(NumString,19,Dop_byte[Device-1]);
				}
				if(Device==3){
					if ((CurrentState[(Device-1)*2+1]==1)||(CurrentState[(Device-1)*2+1]==2)) //передача КЧ или Команды
						LCDprintDEC(NumString,19,Dop_byte[Device-1]);
				}
			}
		}
}

void FuncPrintSignInTest(uchar param, uchar line, uchar start)
{//вывод передаваемого/принимаемого сигнала в тесте
    if (param == 0)
        LCDprintf(line , start , 2 , Menu20gr1[0] , 1);
    else
        if (param < 3)
        {
            LCDprintf(line , start , 2 , Menu20gr1[1] , 1);
            LCDprintDEC(line , (start + 2) , param);
        }
        else
            if (param < 11)
            {
                LCDprintf(line , start , 2 ,  Menu20gr1[4] , 1);
                LCDprintDEC(line , (start + 3) , (param - 2));
            }
            else
                LCDprintf(line , start , 2 , Menu11Err , 1);
}//end void FuncPrintSugnInTest( )

void LCDwork(void){
	unsigned char i;
	if (PCready==0){
		//вывод надписи "Нет связи с БСП"
		if (NumberLostLetter>4){ //если было не принято 5 посылок подряд, то выводим на экран аварию
			AvarNoUsp=1;  //говорим что у нас сейчас нет связи с USP
			if (LoopUART==1) LCDprintf(1,1,2,LCDavarNoUSP,1);  //выводим на экран сообщение "Нет связи с USP"
		}else
			if ((NumberLostLetter<5)&&(AvarNoUsp==1)){ //если же у нас наконец-то была получена посылка от USP
				AvarNoUsp=0;  //говрим что связь с USP налажена
				LCDtimerNew=1;
				LCDprintf(1,1,2,LCDavarNoUSP,2);  //стираем с экрана сообщение "Нет связи с USP"
			}
		
		if (bReadVers){  //если считана версия
			switch (MenuLevel)
			{
			case LVL_START:
			{ 
				i = 2;
				LCDMenu1(i++, 1);
				LCDMenu1(i++, 2);
				LCDMenu1(i, 3);
			}break;
			case LVL_MENU:
			{ 
				if (LCD2new==1){ //обновляем информацию на втором меню
					for(LCD2new=ShiftMenu;((LCD2new-ShiftMenu)<MaxDisplayLine);LCD2new++)
						LCDprintf(2+LCD2new-ShiftMenu,1,2,Menu2point[LCD2new],1);
					LCD2new=0;
				}
			}break;
			case LVL_DATA_TIME:
			{
				if (LCD2new==1){LCDprintf(2,1,2,Menu31,1);LCDprintf(3,1,2,Menu32,1);LCD2new=0;}
			}break;
			case LVL_JRN:
			{  
				if (LCD2new==1){
					for(i=2, LCD2new=ShiftMenu; (LCD2new<=4)&&(i<5) ; i++){
						LCDprintChar(i, 1, LCD2new+0x31);
						LCDprintChar(i, 2, '.');
						LCDprintf(i,3,2, Archive,1);
						LCDprintf(i,10,2,Menu4point[LCD2new++],1);
					}
					LCD2new=0;
				}
			}break;
			case LVL_SETUP: 
			{
				if (LCD2new==1)
				{
					if (WorkRate!=0)
					{
						LCDprintf(2,1,2, Menu5point[ShiftMenu],1);
						LCDprintf(3,1,2, Menu5point[ShiftMenu+1],1);
					}
					else
						for(LCD2new=ShiftMenu;((LCD2new-ShiftMenu)<MaxDisplayLine);LCD2new++)
						{
							LCDprintf(2+LCD2new-ShiftMenu,1,2,Menu5point[LCD2new],1);
						}
					LCD2new=0;
				}
			}
			break;
			case LVL_VIEW_PARAM: 	
			case LVL_SETUP_PARAM:	
			{  
				if (LCD2new == 1)
				{
					for(i = 0;  i < MaxDisplayLine; i++)
					{
						LCDprintf(2 + i, 2, 2, Menu6point[ShiftMenu + i], 1);
						LCDprintChar(2 + i, 1, '1' + i + ShiftMenu);
					}
					LCD2new = 0;
				}
			}break;
			case LVL_VIEW_DEF: 
			case LVL_SETUP_DEF: 
				{
					
					if (LCD2new == 1)
					{
						LCDprintf(2, 1, 2, Menu7paramPOST[ShiftMenu], 1);
						FuncClearCharLCD(3, 1, 20);
						if (ValueVsRange == 0){  //т.е. если 0, то выводим значение параметра, иначе допустимый диапазон значений
							LCDprintf(3, 1, 2, MenuValue, 1);
							switch(ShiftMenu)
							{
							case 0: LCDprintf(3, 11, 2, ParamTypeDef[MenuTypeDefend], 1); break;
							case 1: LCDprintf(3, 11, 2, ParamLineDef[MenuTypeLine], 1); break;
							case 2: LCDprint(3, 11, 2,MenuTimeNoMan, 1); break;
							case 3: LCDprint(3, 11, 2, MenuPossibleTimeSignal1, 1); break;
							case 4: LCDprint(3, 11, 2, MenuOverlapPulse, 1); break;
							case 5: 
								{
									LCDprintChar(3, 11, '-');
									LCDprint(3, 12, 2, MenuVoltageLimit, 1);
								} 
								break;
							case 6: LCDprint(3, 11, 2, MenuRZalarm, 1); break;		
							}
						}
						else{
							LCDprintf(3, 1, 2, ParamRange, 1);
							LCDprintf(3, 11, 2, Menu13ParamRange[ShiftMenu], 1);
						}
						LCD2new = 0;
					}
				}
				break;
			case LVL_VIEW_PRM:		
			case LVL_SETUP_PRM:	
			{
				if (LCD2new == 1)
				{
					LCDprintf(2, 1, 2, Menu8paramPRM[ShiftMenu], 1);
					if (ShiftMenu == 1)	//задержка на выкл, для 4 команд
					{
						LCDprintChar(2, 19, NumberCom + '0');
						fCharToTimeOff(ValuePrmTimeOff[NumberCom - 1]);						
					}
					FuncClearCharLCD(3, 1, 20);
					if (ValueVsRange == 0)
					{  //т.е. если 0, то выводим значение параметра, иначе допустимый диапазон значений					
						
						LCDprintf(3, 1, 2, MenuValue, 1);
						LCDprint(3, 11, 2, MenuParamPRM[ShiftMenu], 1);
					}                  
					else
					{                     
						LCDprintf(3, 1, 2, ParamRange, 1);                      
						LCDprintf(3, 11, 2, Menu8ParamRange[ShiftMenu], 1);                  
					}
				}
			}
			break;
			case LVL_VIEW_PRD:		
			case LVL_SETUP_PRD:	
				{
					if (LCD2new==1)
					{
						LCDprintf(2,1,2,Menu9paramPRD[ShiftMenu],1);
						FuncClearCharLCD(3,1,20);
						if (ValueVsRange==0)
						{
							//т.е. если 0, то выводим значение параметра, иначе допустимый диапазон значений
							LCDprintf(3,1,2,MenuValue,1);
							LCDprint(3,11,2, MenuParamPRD[ShiftMenu],1);
						}
						else
						{
							LCDprintf(3,1,2,ParamRange,1);
							LCDprintf(3,11,2,Menu9ParamRange[ShiftMenu],1);
						}
					}
				}
				break;
			case LVL_VIEW_GLB:  
			case LVL_SETUP_GLB: 
				{
					if (LCD2new==1)
					{
						LCDprintf(2,1,2,Menu10paramAll[ShiftMenu],1);
						FuncClearCharLCD(3,1,20);
						if (ValueVsRange==0)
						{
							LCDprintf(3,1,2,MenuValue,1);
							switch(ShiftMenu){
							case 0: LCDprintf(3,11,2,MenuAllSynchrTimerNum[MenuAllKeepComPRM],1); break;
							case 1: LCDprintf(3,11,2,MenuAllSynchrTimerNum[MenuAllKeepComPRD],1); break;
							case 2: LCDprint(3,11,2,MenuAllTimeRerun,1); break;
							case 3: LCDprintf(3,11,2,MenuAllSynchrTimerNum[MenuAllControlUout],1); break;
							case 4: LCDprint(3,11,2,MenuAllCF,1); break;
							case 5: {LCDprintChar(3,11,'-');LCDprint(3,12,2,MenuVoltageLimit1,1);} break;
							case 6: {LCDprintChar(3,11,'-');LCDprint(3,12,2,MenuVoltageLimit2,1);} break;
							}
						}
						else
						{
							LCDprintf(3,1,2,ParamRange,1);
							LCDprintf(3,11,2,Menu16ParamRange[ShiftMenu],1);
						}
					}
					LCD2new=0;
				}
				break;
				
			case LVL_SETUP_RGM:  
				{
					if (LCD2new==1)
					{
						if (TestDelay==0){
							FuncClearCharLCD(2,1,20);
							FuncClearCharLCD(3,1,20);
							//Пост
							LCDprintf(2,1,2,Menu1Def,1);
							if (CurrentState[0]==0x4E) LCDprintf(2,5,2,Menu11Err,1);
							else LCDprintf(2,5,2,Menu11var[CurrentState[0]],1);
							//ПРМ1
							LCDprintf(2,11,2,Menu1Rec,1);
							if (CurrentState[2]==0x4E) LCDprintf(2,15,2,Menu11Err,1);
							else LCDprintf(2,15,2,Menu11var[CurrentState[2]],1);
							//ПРД
							LCDprintf(3,1,2,Menu1Tran,1);
							if (CurrentState[4]==0x4E) LCDprintf(3,5,2,Menu11Err,1);
							else LCDprintf(3,5,2,Menu11var[CurrentState[4]],1);
							//ПРМ2
							//LCDprintf(3,11,2,Menu1Rec2,1);
							//if (CurrentState[6]==0x4E) LCDprintf(3,15,2,Menu11Err,1);
							//else LCDprintf(3,15,2,Menu11var[CurrentState[6]],1);
						}else{
							LCDprintf(2,1,2,TestDelayMline2,1);
							LCDprintf(3,1,2,TestDelayMline3,1);
							LCDprintDEC(3,13,(TestDelay/10)+1);
						}
						LCD2new=0;
					}
				}
				break;
			case LVL_INFO: 
			{
				LCDprintf(2, 1, 2, Menu19Param[ShiftMenu], 1);
				LCDprintf(3, 1, 2, MenuValue, 1);
				if (ShiftMenu < 3)
				{
					LCDprintHEX(3,11,(char) (MyInsertion[ShiftMenu]>>8));
					LCDprintHEX(3,14,(char) MyInsertion[ShiftMenu]);
					LCDprintChar(3,13,'.');
				}
				else
					if (ShiftMenu==3)
					{
						if (bParamView) 
							LCDprintf(3, 11, 2, fDopParamViewTrue, 1);
						else 
							LCDprintf(3, 11, 2, fDopParamViewFalse, 1);
					}
					else
						if (ShiftMenu==4)
						{
							if (bParamValue) 
								LCDprintf(3, 11, 2, fDopParamValueTrue, 1);
							else 
								LCDprintf(3, 11, 2, fDopParamValueFalse, 1);
						}
			}break; 
			case LVL_TEST: 
			{ 
				if (LCD2new==1)
					if (CurrentState[2]<0x04) {PressKey='C'; break;}   //если есть приемник, и он не в Тест1
				if (CurrentState[4]<0x04) {PressKey='C'; break;}  //если есть передатчик, и он не в Тест1
				if (CurrentState[0]<0x04) {PressKey='C'; break;}  //если есть Защита, и она не в Тест1
				
				char Test=0xFF;
				Test=CurrentState[0]-4;
				
				FuncClearCharLCD(2,1,20);
				FuncClearCharLCD(3,1,20);
				
                if (Test==0)
                {//тест передачи
                    LCDprintf(2,11,2,Menu20line2test12,1);
                    if (gr2<3)
                        LCDprintf(2,16,2,Menu20gr2[gr2],1);
                    else
                        LCDprintf(2,16,2,Menu11Err,1);
					
                    LCDprintf(2,1,2,Menu20line2test11,1);
					
                    FuncPrintSignInTest(gr1 , 2 , 5);
					
                    LCDprintf(3,1,2,MenuInputData,1);
                    LCDprintf(3,7,2,Menu20line3[ShiftMenu],1);
                }
                if (Test == 1)
                {//тест приемника
					uchar shift=6;
					
                    LCDprintf(2,1,2,Menu1Rec,1); //"ПМ1"
                    LCDprintChar(2,4,':');
                    if (((gr1==0)&&(gr2==0))||((gr1>10)&&(gr2>2)))
                        LCDprintf(2,6,2,fDopParamViewFalse,1);//"нет"
                    else{
						if (gr1>0){
							if (gr1<11) LCDprintf(2,shift,2,Menu20gr1[gr1],1);
							if ((gr2>0)&&(gr2<3)){
								if (gr1<3) shift = 12;
								else shift=13;
								LCDprintChar(2,shift-2,0xB8);
							}
						}
						
						if (gr2>0){
							if (gr2<3) LCDprintf(2,shift,2,Menu20gr2[gr2],1);
						}
                    }
                }//end if (Test == 1)
				
                if (WorkRate==0) LCDprintf(4,1,2,Menu20line4[Test],1);
				
				LCD2new=0;
				
			}break;
			case LVL_JRN_VIEW:
			{
				LCDprintf(4,1,2,Menu21e[sArchive.Dev[sArchive.CurrDev]],1); // вывод устройства
				LCDptinrArchCount(3, sArchive.RecCount, ShiftMenu);
				if (sArchive.RecCount){  //если в архиве что-то есть, будем выводить записи
					if (sArchive.Data[12]){  //если данные уже получены
						//for(char i=0; i<4; i++) LCDprintHEX(3,i*2+1, sArchive.Data[i]);
						//вывод инф-ии на экран
						unsigned __flash char* mm= 0;
						
						switch(sArchive.Data[0]){ //устройство
						case 0: mm= Menu11d; break; //пост
						case 1: mm=Menu11r; break;  //прм / пм1
						case 2: mm=Menu11r2; break;  //пм2
						case 3: mm=Menu11t; break; //прд
						case 4: mm=Menu21g; break; //общ
						}
						
						if (mm==0){
							LCDprintHEX(2,1,sArchive.Data[0]);
							LCDprintChar(2,3,'?');
						}else LCDprintf(2,1,2,mm,1);
						
						FuncClearCharLCD(2,4,4);  //очистим 4 клетки между устрйством и временем
						
						switch(sArchive.Dev[sArchive.CurrDev]){
						case 0:{  //журнал событий
							if ((sArchive.Data[1]>0)&&(sArchive.Data[1]<33)){  //тип события
								LCDprintf(3,1,2,Menu21ev[sArchive.Data[1]],1);
							}else{//если неизвестный код записи
								FuncClearCharLCD(3,1,10);
								LCDprintHEX(3,2,sArchive.Data[1]);
								LCDprintHEX(3,5,sArchive.Data[3]);
							}
							FuncClearCharLCD(3,12,1);
							FuncClearCharLCD(3,20,1);
							
							if (sArchive.Data[2]<0x04){ //значение события
								LCDprintf(3,13,2,Menu1regime[sArchive.Data[2]],1);
							}else
								if (sArchive.Data[2]==0x0A){
									LCDprintf(3,13,2,ArchEvV1,1);
								}else{
									LCDprintHEX(3,13,sArchive.Data[1]);
								}
						}break; //конец вывода ждурнала событий
						case 1: //передатчик
						case 2:{  //приемник
							mm=0;
							if (sArchive.Data[2]==0) {mm=ArchEvEnd; i=7;}
							else
								if (sArchive.Data[2]==1) {mm=ArchEvStart; i=8;}
							
							if (mm==0){
								LCDprintHEX(3,1,sArchive.Data[0]);
								FuncClearCharLCD(3,3,i-3);
							}else LCDprintf(3,1,2,mm,1);
							
							if ((sArchive.Data[1]==0)||(sArchive.Data[1]>32)){  //ошибочное значение команды
								LCDprintHEX(3,i,sArchive.Data[1]); i+=2;
							}else{
								LCDprintDEC(3,i,sArchive.Data[1]);
								if (sArchive.Data[1]<10) i+=1;
								else i+=2;
							}
							FuncClearCharLCD(3,i++,1);
							LCDprintf(3,i,2,ArchEvCom,1);
						}break; //конец вывода ждурнала передатчика/приемника
						case 3:{  //защита
							mm=0;
							//состояние защиты
							if (sArchive.Data[2]==0) mm=ArchEvV1; //перезапуск
							else
								if (sArchive.Data[2]<11) mm=Menu1condPOST[sArchive.Data[2]];
							
							if (mm==0){
								FuncClearCharLCD(3,1,10);
								LCDprintHEX(3,2,sArchive.Data[1]);
							}else LCDprintf(3,1,2,mm,1);
							
							FuncClearCharLCD(3,9,10);
							(sArchive.Data[1]&0x01) ? LCDprintChar(3,10,'1') : LCDprintChar(3,10,'0');  //пуск
							(sArchive.Data[1]&0x02) ? LCDprintChar(3,11,'1') : LCDprintChar(3,11,'0');  //стоп
							(sArchive.Data[1]&0x04) ? LCDprintChar(3,12,'1') : LCDprintChar(3,12,'0');  //ман
							
							(sArchive.Data[3]&0x02) ? LCDprintChar(3,14,'1') : LCDprintChar(3,14,'0');  //прд
							(sArchive.Data[3]&0x01) ? LCDprintChar(3,15,'1') : LCDprintChar(3,15,'0');  //прм
							(sArchive.Data[3]&0x03) ? LCDprintChar(3,16,'1') : LCDprintChar(3,16,'0'); //рз (если есть прм или прд)
						} break; //конец вывода ждурнала защиты
						}// end switch(sArchive.Dev[sArchive.CurrDev])
						
						LCDprintData(72, sArchive.Data);
						LCDprintTime(28, sArchive.Data);
						
					}else{
						FuncClearCharLCD(2,1,40); //очистка инф-ы
						LCDprintf(3,1,2,Menu2xRdArch,1);
					}
				}else{  //архив пустой
					FuncClearCharLCD(2,1,40); //очистка инф-ы
					FuncClearCharLCD(4,13,8); //очистка даты записи
				}
				LCD2new=0;
			}break;
			
			}
			
			//проверяем надоли обновить информацию дата/время  и измеряемых параметров
			//если на данный момент есть авария, то надпись будет мигать
			if (( AvarNoUsp == 1) && (LoopUART == 5) ) 
			{
				LCDtimerNew = 1; 
				LCDparam = 1; 
				LCDprintf(1, 1, 2, LCDavarNoUSP, 2);
			}
			
			if (LCDtimerNew==1)
			{
				if ( ((AvarNoUsp == 1) && (LoopUART == 5)) || (AvarNoUsp == 0) )
				{
					FuncClearCharLCD(1, 9, 2);
					if (MenuLevel != LVL_DATA_TIME)
					{
						if (LCDtimer == 1) LCDprint(1, 1, 2, DataLCD, 0);
						else
							if (LCDtimer == 2) LCDprint(1, 1, 2, TimeLCD, 0);
					}
					else
					{
						LCDprint(1, 1, 2, DataLCD, 0); 
						LCDprint(1, 13, 2, TimeLCD, 0); 
						LCDprintChar(1, 12, ' ');
					}
					LCDtimerNew = 0;
				}
			}
			
			//выводим на экран измеряемый параметр
			if ( (LCDparam == 1) && (PCready == 0) )
			{
				if ( (eNumberAskMeasuring[0] == 0 ) || (eNumberAskMeasuring[0] > 6) ) 
					eNumberAskMeasuring[0] = 1;
				NumberAskMeasuring = eNumberAskMeasuring[0];
				if ( ((AvarNoUsp == 1) && (LoopUART == 5)) || (AvarNoUsp == 0))
				{
					if (MenuLevel != LVL_DATA_TIME) 
					{
						FuncClearCharLCD(1, 11, 2);
						if (bParamValue)
							LCDprint(1, 12, 2, Measuring[NumberAskMeasuring - 1], 1);
						else
							LCDprint(1, 12, 2, MeasDop[NumberAskMeasuring - 1], 1);
					}
				}
				LCDparam = 0;
			}
		}else
			if (((AvarNoUsp==1)&&(LoopUART==5))||(AvarNoUsp==0)) {
				FuncClearCharLCD(1,1,20);
				LCDprintf(2,1,2,fNoReadVers,1);
				LCDprintHEX(4,16,Hi(MyInsertion[0]));
				LCDprintChar(4,18,'.');
				LCDprintHEX(4,19,MyInsertion[0]);
				PCconn++;
				if (PCconn>3) PCconn=1;
				for (char j = 0; j < PCconn; j++) 
					LCDprintf(2, 16 + j, 2, Point, 1);
			}
	}else{  //вывод на индикатор "Связь с ПК..."
		if ((LoopUART==0)||(LoopUART==5)){
			LCDbufClMenu();
			LCDprintf(2,4,2,PCconnect,1);
			PCconn++;
			if (PCconn>3) PCconn=1;
			for (char j = 0; j < PCconn; j++) 
				LCDprintf(2, 15 + j, 2, Point, 1);
		}
	}
	bLCDwork=false;
}


int main(void)
{
	StartSetup(); //начальные установки
	
	ClearPortError(); //очитска ошибок УАПП
	StartUART();  //запуск УАПП
	ClearPortError1(); //очитска ошибок УАПП
	StartUART1();
	LCDbufClMenu();  //очистка соджержимого буфера LCD
	FuncInitLCD();  //инициализация ЖК-индикатора
	WatchDogInit();  //инициализация сторожевого таймера
	
	_SEI(); //установка бита разрешения прерываний
	
	TCCR1B = 0x05;  //запускаем таймер1
	
	//скопируем пароль из EEPROM во флэш
	eRead=1;
	eAddressRead=0;
	eMassiveRead=ePassword;
	while(eRead);
	eRead=1;
	eAddressRead=5;
	eMassiveRead=eNumberAskMeasuring;
	while(eRead);
	bLCDwork=true;
	
	while(1)
	{
		if (bUartRcReady1) 
			DataModBus(cNumRecByte);
		if (bLCDwork) 
			LCDwork();
	}
}



